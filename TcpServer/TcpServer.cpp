#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <chrono>
#include <iostream>
#include <optional>
#include "TcpServer.hpp"
#include "helper.hpp"

using namespace std::chrono_literals;


TcpServer::TcpServer(std::optional<std::string> ipAddress, std::optional<uint16_t> port)
: _ipAddress(ipAddress.value_or(""))
, _port(port.value_or(DefaultPort))
{
}

TcpServer::~TcpServer()
{
    for(auto& handler : _connectionHandlers)
        handler->stop();
}

void TcpServer::start()
{
    std::cout << "Starting TCP server on ip address: " << (_ipAddress.empty() ? "any" : _ipAddress ) << " and port: " << _port << std::endl;
    
    int listen_socket_fd = helper::guard(socket(AF_INET, SOCK_STREAM, 0), "could not create TCP listening socket");
    int flags = helper::guard(fcntl(listen_socket_fd, F_GETFL), "could not get flags on TCP listening socket");
    helper::guard(fcntl(listen_socket_fd, F_SETFL, flags | O_NONBLOCK), "could not set TCP listening socket to be non-blocking");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);

    if(_ipAddress.empty())
        addr.sin_addr.s_addr = htonl(DefaultIpAddress);
    else
        addr.sin_addr.s_addr = inet_addr(_ipAddress.c_str());

    helper::guard(bind(listen_socket_fd, (struct sockaddr *) &addr, sizeof(addr)), "could not bind");
    helper::guard(listen(listen_socket_fd, _maxConnections), "could not listen");

    while(!_shouldStop) 
    {
        if (int clientSocket = accept(listen_socket_fd, NULL, NULL); clientSocket == -1) 
        {
            if(errno != EWOULDBLOCK) 
                throw(std::runtime_error("error when accepting connection"));
        }
        else 
        {
            if(_connectionHandlers.size() < _maxConnections)
                _connectionHandlers.emplace_back(std::make_unique<AsyncConnectionHandler>(clientSocket, *this));
            else
            {
                const std::string msg = "Max connection reached, closing socket\n";
                send(clientSocket, msg.data(), msg.size(), 0);
                close(clientSocket);
            }
        }

        // remove old connection handlers
        _connectionHandlers.erase(
        std::remove_if(
            _connectionHandlers.begin(), 
            _connectionHandlers.end(),
            [](const auto& handler) {  return (handler ? handler->stopped() : true);}
            ), 
            _connectionHandlers.end()
        );

        size_t tmpBroadcastCount = 0;
        {
            std::scoped_lock(_mutex);
            tmpBroadcastCount = std::exchange(_broadcastCount, 0);
        }

        for(auto& handler : _connectionHandlers)
        {
            for(int i = 0; i < tmpBroadcastCount; ++i)
                if(handler)
                    handler->sendCurrentClientCount(_connectionHandlers.size());
        }
        std::this_thread::sleep_for(1s);
    }
    close(listen_socket_fd);
    std::cout << " TcpServer stopped" << std::endl;
}

void TcpServer::stop()
{
    _shouldStop = true;
}

void TcpServer::notifyBroadCast(size_t count)
{
    std::scoped_lock(_mutex);
    _broadcastCount += count;
}