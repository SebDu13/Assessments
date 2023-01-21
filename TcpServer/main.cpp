#include <signal.h>
#include <functional>
#include <iostream>
#include <optional>
#include "TcpServer.hpp"
#include "helper.hpp"

namespace 
{
std::function<void(int)> shutdownHandler;
void signal_handler(int signal) 
{ 
    if(SIGINT == signal)
        shutdownHandler(signal); 
}
} 

//TODO CATCH exception
int main(int argc, char const* argv[])
{
    const char* ipAddress = helper::getCmdOption(argv, argv + argc, "-a");
    const char* port = helper::getCmdOption(argv, argv + argc, "-p");

    try
    {
        TcpServer tcpServer(helper::convertIpAddress(ipAddress), helper::convertPort(port));

        shutdownHandler = [&tcpServer](int){
            tcpServer.stop();
        };

        signal (SIGINT, signal_handler);
        signal (SIGPIPE, signal_handler);
        tcpServer.start();
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}