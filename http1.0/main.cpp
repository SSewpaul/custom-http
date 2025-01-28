#include "tcpserver.cpp"
#include <iostream>

int main()
{
    TCPServer tcp_server;
    if (tcp_server.start_server() > 0) {
        std::cerr << "Cannot start server" << std::endl;
    }
}