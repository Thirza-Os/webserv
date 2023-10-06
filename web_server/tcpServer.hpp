#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

#include "httpparser/requestParser.hpp"
#include "responseBuilder.hpp"
#include "configparser/serverConfig.hpp"

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

class tcpServer
{
public:
    tcpServer(serverConfig config);
    ~tcpServer();

    void    exitError(const std::string &str);
    void    log(const std::string &message);

private:
    serverConfig                    _config;
    in_addr_t                       _ip_add;
    int                             _port;
    int                             _socket;

    struct                          sockaddr_in _socketAddr;
    unsigned int                    _socketAddrLen;
    std::string                     _serverMessage;
    std::vector<struct pollfd>      _pollfds;
    std::map<int, requestParser>    _requests;


    int             startServer();
    void            closeServer();

    void            startListen();
    void            acceptConnection();

    //std::string     buildResponse(std::string uri);
    void            sendResponse(int socket_fd);
};

#endif
