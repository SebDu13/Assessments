#pragma once
#include "TcpServer.hpp"
#include <mutex>
#include <queue>
#include <thread>

class TcpServer;

class AsyncConnectionHandler
{
public:
    AsyncConnectionHandler(int socket, TcpServer& tcpServer);

    void stop();
    bool stopped() const;
    void sendCurrentClientCount(size_t count);

private:
    void _run();

    std::jthread _jthread;
    std::queue<int> _clientCountQueue;
    std::mutex _mutex;

    TcpServer& _tcpServer;
    const int _socket;
};