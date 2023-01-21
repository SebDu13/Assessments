#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <chrono>
#include <iostream>

#include "AsyncConnectionHandler.hpp"
#include "IDGenerator.hpp"
#include "helper.hpp"

using namespace std::chrono_literals;

namespace helper
{
    auto addEOL(const auto& array)
    {
        using ArrayType = std::remove_reference<decltype(array)>::type;
        std::array<char, std::tuple_size<ArrayType>::value+1> newArray;
        std::copy(std::begin(array), std::end(array), std::begin(newArray));
        newArray.back() = '\n';

        return newArray;
    }

    size_t countEOL(const auto& array, int bytes)
    {
        if(bytes <= 0 || bytes > array.size())
            return 0;

        size_t count = 0;
        for(int i = 0; i < bytes; ++i)
            if(array[i] == '\n')
                ++count;
        return count;
    }

    std::string toString(std::queue<int>&& queue)
    {
        std::string result;
        result.reserve(queue.size() *2);
        while(!queue.empty())
        {
            result += std::to_string(queue.back());
            result.push_back('\n');
            queue.pop();
        }
        return result;
    }
}

AsyncConnectionHandler::AsyncConnectionHandler(int socket, TcpServer& tcpServer)
: _socket(socket)
, _tcpServer(tcpServer)
, _jthread(std::jthread(&AsyncConnectionHandler::_run, this))
{
}

void AsyncConnectionHandler::_run()
{
    std::cout <<"New connection on socket " << _socket << std::endl;

    try
    {
        int flags = helper::guard(fcntl(_socket, F_GETFL), "could not get flags on TCP listening socket");
        helper::guard(fcntl(_socket, F_SETFL, flags | O_NONBLOCK), "could not set TCP listening socket to be non-blocking");
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
        return;
    }

    auto stopToken = _jthread.get_stop_token();
    while(!stopToken.stop_requested())
    {
        // generate an Id, add end of line char and send it to the client
        auto id = helper::addEOL(IDGenerator::generate());
        if( send(_socket, id.data(), id.size(), 0) < 0)
            break;

        std::array<char,100> buffer;
        int bytes = recv(_socket, buffer.data(), buffer.size(), 0);
        if(size_t count = helper::countEOL(buffer, bytes); count)
            _tcpServer.notifyBroadCast(count);

        std::queue<int> tmpQueue;
        {
            std::scoped_lock(_mutex);
            if(!_clientCountQueue.empty())
                _clientCountQueue.swap(tmpQueue);
        }

        std::string data = helper::toString(std::move(tmpQueue));
        if(send(_socket, data.data(), data.size(), 0) < 0)
            break;

        std::this_thread::sleep_for(1s);
    }

    const std::string Thanks = "Thank you\n";
    send(_socket, Thanks.data(), Thanks.size(), 0);

    std::cout <<"Closing socket " << _socket << std::endl;
    close(_socket);
    //call request_stop() in order to know if the thread is still executing or not in stopped() method
    _jthread.request_stop();
}

void AsyncConnectionHandler::stop()
{
    _jthread.request_stop();
    _jthread.join();
}

bool AsyncConnectionHandler::stopped() const
{
    return !_jthread.joinable() || _jthread.get_stop_token().stop_requested();
}

void AsyncConnectionHandler::sendCurrentClientCount(size_t count)
{
    std::scoped_lock(_mutex);
    _clientCountQueue.emplace(count);
}