#include "serverManager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <algorithm>

const int BUFFER_SIZE = 30720;

serverManager::serverManager(std::vector<serverConfig> configs): _configs(configs)
{
    for (std::vector<serverConfig>::iterator it = _configs.begin(); it != _configs.end() ;it++) {
        _servers.push_back(server(*it));
    }
    startListen();
}

serverManager::~serverManager(){}

void    serverManager::exitError(const std::string &str)
{
    std::cerr << "Error - " << str << std::endl;
    exit(1);
}

void    serverManager::log(const std::string &message)
{
    std::cout << message << std::endl;
}

void serverManager::startListen()
{
    std::vector<int> listeners;
    for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end() ;it++) {
        if (listen(it->getSocket(), 20) < 0)
        {
            exitError("Socket listen failed");
        }
        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " 
            << inet_ntoa(it->getSockAddr().sin_addr) 
            << " PORT: " << ntohs(it->getSockAddr().sin_port) 
            << " ***\n\n";
        log(ss.str());
        struct pollfd listener;

        listener.fd = it->getSocket();
        listener.events = POLLIN;
        listeners.push_back(it->getSocket());
        _pollfds.push_back(listener);
    }
    // main webserv loop starts here, the program should never exit this loop
        while (true)
        {
            log("====== Waiting for a new event ======\n\n\n");
            if (poll(&_pollfds[0], _pollfds.size(), -1) == -1) {
                log("Error returned from poll()\n");
            }
            for (std::vector<struct pollfd>::iterator it = _pollfds.begin(); it < _pollfds.end(); it++) {
                if (it->revents & POLLIN) {
                    if (std::find(listeners.begin(), listeners.end(), it->fd) != listeners.end()) {
                        acceptConnection(it->fd);
                        break ;
                    }
                    else {
                        char buffer[BUFFER_SIZE] = {0};
                        int bytesReceived = read(it->fd, buffer, BUFFER_SIZE);
                        if (bytesReceived < 0)
                        {
                            log("Failed to read bytes from client socket connection");
                            break ;
                        }
                        if (bytesReceived == 0) {
                            log("Client closed the connection");
                            _requestServerIndex.erase(it->fd);
                            close(it->fd);
                            it = _pollfds.erase(it);
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
                    if (sendResponse(it->fd)) {
                        close(it->fd);
                        it = _pollfds.erase(it);
                    }
                    else {
                        it->events = POLLIN;
                    }
                    _requests.erase(it->fd);
                    break ;
                }
            }
        }

}

void serverManager::acceptConnection(int incoming)
{
    server correctServer;
    for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end() ;it++) {
        if (it->getSocket() == incoming)
            correctServer = *it;
    }
    sockaddr_in socketAddr = correctServer.getSockAddr();
    unsigned int socketAddrLen = sizeof(socketAddr);
    int new_socket = accept4(correctServer.getSocket(), (sockaddr *)&socketAddr,
                        &socketAddrLen, SOCK_NONBLOCK);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss <<
        "Server failed to accept incoming connection from ADDRESS: "
        << inet_ntoa(socketAddr.sin_addr) << "; PORT: "
        << ntohs(socketAddr.sin_port);
        log(ss.str());
        return ;
    }
    struct pollfd new_socket_fd;
    new_socket_fd.fd = new_socket;
    new_socket_fd.events = POLLIN;
    _pollfds.push_back(new_socket_fd);
    this->_requestServerIndex.insert({new_socket, correctServer});
    std::ostringstream ss;
    ss << "------ New connection established ------\n\n";
    log(ss.str());
}

//return 1 to close the connection after, 0 to keep it alive
int serverManager::sendResponse(int socket_fd)
{
    unsigned long bytesSent;
    responseBuilder response(_requests.at(socket_fd), _requestServerIndex.at(socket_fd).getConfig());
    std::string serverMessage = response.getResponse();
    std::cout << response.getHeader() << std::endl;
    bytesSent = write(socket_fd, serverMessage.c_str(), serverMessage.size());
    if (bytesSent == serverMessage.size())
    {
        log("------ Server Response sent to client ------\n\n");
    }
    else
    {
        log("Error sending response to client");
    }
    if (_requests.at(socket_fd).find_header("Connection") == " close") {
        std::cout << "Closing connection" << std::endl;
        _requestServerIndex.erase(socket_fd);
        return (1);
    }
    else {
        std::cout << "Keeping connection alive" << std::endl;
        return (0);
    }
}
