#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

class tcpServer
{
public:
    tcpServer(std::string ip_add, int port);
    ~tcpServer();

    void    exitError(const std::string &str);
    void    log(const std::string &message);

private:
    std::string     _ip_add;
    int             _port;
    int             _m_socket;
    int             _m_newSocket;
    long            _incomingMessage;

    struct          sockaddr_in _socketAddr;
    unsigned int    _socketAddrLen;
    std::string     _serverMessage;   


    int             startServer();
    void            closeServer();

    void            startListen();
    void            acceptConnection(int &new_socket);

    std::string     buildResponse();
    void            sendResponse();
};

#endif
