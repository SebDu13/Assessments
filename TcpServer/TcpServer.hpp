#pragma once
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <netinet/in.h>
#include "AsyncConnectionHandler.hpp"

class AsyncConnectionHandler;
class TcpServer
{
public:
    TcpServer() = default;
    TcpServer(std::optional<std::string> ipAddress, std::optional<uint16_t> port);
    virtual ~TcpServer();
    void start();
    void stop();
    void notifyBroadCast(size_t count);

private:
    static constexpr uint16_t DefaultPort = 8080;
    static constexpr uint64_t DefaultIpAddress = INADDR_ANY;

    const uint16_t _port = DefaultPort;
    const std::string _ipAddress;
    const size_t _maxConnections = 6;

    std::vector<std::unique_ptr<AsyncConnectionHandler>> _connectionHandlers;
    std::atomic<bool> _shouldStop = false;

    size_t _broadcastCount = 0;
    std::mutex _mutex;
};