#include "tcpServer.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>

const int BUFFER_SIZE = 30720;

tcpServer::tcpServer(std::string ip_add, int port):_ip_add(ip_add), _port(port), _socket(), _newSocket(), _incomingMessage(), _socketAddr(), _socketAddrLen(sizeof(_socketAddr))
{

    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(_port);
    _socketAddr.sin_addr.s_addr = inet_addr(_ip_add.c_str());

    startServer();
}

tcpServer::~tcpServer()
{
    closeServer();
}

void    tcpServer::exitError(const std::string &str)
{
    std::cerr << "Error - " << str << std::endl;
    exit(1);
}

void    tcpServer::log(const std::string &message)
{
    std::cout << message << std::endl;
}

int tcpServer::startServer()
{
    // making the socket
            printf("Starting sockert\n");

    _socket = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket < 0)
    {
        exitError("Socket failed!");
        return 1;
    }

// binding the socket
    if (bind(_socket, (sockaddr *)&_socketAddr, _socketAddrLen) < 0)
    {
        exitError("Cannot connect socket to address");
        return 1;
    }

    startListen();
    return 0;
}

void    tcpServer::closeServer()
{
    close(_socket);
    close(_newSocket);
    exit(0);
}

void tcpServer::startListen()
{
    if (listen(_socket, 20) < 0)
    {
        exitError("Socket listen failed");
    }
    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " 
        << inet_ntoa(_socketAddr.sin_addr) 
        << " PORT: " << ntohs(_socketAddr.sin_port) 
        << " ***\n\n";
    log(ss.str());

    int bytesReceived;

            while (true)
        {
            log("====== Waiting for a new connection ======\n\n\n");
            acceptConnection(_newSocket);
            printf("socket: %i\n socket: %i\n", _socket, _newSocket);

            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(_newSocket, buffer, BUFFER_SIZE);
            printf("This int: %i \n", bytesReceived);
            if (bytesReceived < 0)
            {
                exitError("Failed to read bytes from client socket connection");
            }

            std::ostringstream ss;
            ss << "------ Received Request from client ------\n\n";
            log(ss.str());

            sendResponse();

            close(_newSocket);
        }

}

void tcpServer::acceptConnection(int &new_socket)
{
        printf("socket 1: %i\n", new_socket);
        printf("socket old: %i\n", _socket);
        printf("socket old  len: %i\n", _socketAddrLen);

        new_socket = accept(_socket, (sockaddr *)&_socketAddr, 
                        &_socketAddrLen);

        printf("socket new: %i\n", _socket);
        printf("socket new  len: %i\n", _socketAddrLen);
        printf("socket 2: %i\n", new_socket);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss << 
        "Server failed to accept incoming connection from ADDRESS: " 
        << inet_ntoa(_socketAddr.sin_addr) << "; PORT: " 
        << ntohs(_socketAddr.sin_port);
        exitError(ss.str());
    }
}

std::string tcpServer::buildResponse()
{
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;

    return ss.str();
}

void tcpServer::sendResponse()
{
    long bytesSent;

    std::string _serverMessage = buildResponse();

    bytesSent = write(_newSocket, _serverMessage.c_str(), _serverMessage.size());

    if (bytesSent == _serverMessage.size())
    {
        log("------ Server Response sent to client ------\n\n");
    }
    else
    {
        log("Error sending response to client");
    }
}
