#include "tcpServer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>

const int BUFFER_SIZE = 30720;

tcpServer::tcpServer(serverConfig config):_config(config), _socket(), _socketAddr(), _socketAddrLen(sizeof(_socketAddr))
{
    this->_ip_add = this->_config.getHost();
    this->_port = this->_config.getPort();
    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(_port);
    _socketAddr.sin_addr.s_addr = _ip_add;

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
            printf("Starting socket\n");

    _socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if (_socket < 0)
    {
        exitError("Socket failed!");
        return 1;
    }
    int enable = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        exitError("setsockopt(SO_REUSEADDR) failed");
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
    struct pollfd listener;

    listener.fd = _socket;
    listener.events = POLLIN;
    _pollfds.push_back(listener);
    // main webserv loop starts here, the program should never exit this loop
        while (true)
        {
            log("====== Waiting for a new event ======\n\n\n");
            if (poll(&_pollfds[0], _pollfds.size(), -1) == -1) {
                log("Error returned from poll()\n");
            }
            for (std::vector<struct pollfd>::iterator it = _pollfds.begin(); it < _pollfds.end(); it++) {
                if (it->revents & POLLIN) {
                    if (it->fd == listener.fd) {
                        acceptConnection();
                        break ;
                    }
                    else {
                        char buffer[BUFFER_SIZE] = {0};
                        bytesReceived = read(it->fd, buffer, BUFFER_SIZE);
                        if (bytesReceived < 0)
                        {
                            log("Failed to read bytes from client socket connection");
                            break ;
                        }
                        requestParser request(buffer);
                        _requests.insert({it->fd, request});

                        std::ostringstream ss;
                        ss << "------ Received Request from client ------\n\n";
                        log(ss.str());
                        it->events = POLLOUT;
                        break ;
                    }
                }
                else if (it->revents & POLLOUT) {
                    sendResponse(it->fd);
                    close(it->fd);
                    _requests.erase(it->fd);
                    it = _pollfds.erase(it);
                    break ;
                }
            }
        }

}

void tcpServer::acceptConnection()
{
    int new_socket = accept4(_socket, (sockaddr *)&_socketAddr,
                        &_socketAddrLen, SOCK_NONBLOCK);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss <<
        "Server failed to accept incoming connection from ADDRESS: "
        << inet_ntoa(_socketAddr.sin_addr) << "; PORT: "
        << ntohs(_socketAddr.sin_port);
        log(ss.str());
        return ;
    }
    struct pollfd new_socket_fd;
    new_socket_fd.fd = new_socket;
    new_socket_fd.events = POLLIN;
    _pollfds.push_back(new_socket_fd);
    std::ostringstream ss;
    ss << "------ New connection established ------\n\n";
    log(ss.str());
}

void tcpServer::sendResponse(int socket_fd)
{
    unsigned long bytesSent;
    responseBuilder response(_requests.at(socket_fd));
    std::string _serverMessage = response.getResponse();
    std::cout << response.getHeader() << std::endl;

    bytesSent = write(socket_fd, _serverMessage.c_str(), _serverMessage.size());

    if (bytesSent == _serverMessage.size())
    {
        log("------ Server Response sent to client ------\n\n");
    }
    else
    {
        log("Error sending response to client");
    }
}

