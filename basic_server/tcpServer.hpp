#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

class tcpServer
{
public:
    tcpServer(std::string ip_add, int port);
    ~tcpServer();

    void    exitError(const std::string &str);
    void    log(const std::string &message);

private:
    std::string                 _ip_add;
    int                         _port;
    int                         _socket;

    struct                      sockaddr_in _socketAddr;
    unsigned int                _socketAddrLen;
    std::string                 _serverMessage;
    std::vector<struct pollfd>  _pollfds;


    int             startServer();
    void            closeServer();

    void            startListen();
    void            acceptConnection();

    std::string     buildResponse();
    void            sendResponse(int socket_fd);
};

#endif
