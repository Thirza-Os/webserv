#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "HttpParser/RequestParser.hpp"
#include "ResponseBuilder.hpp"
#include "Server.hpp"
#include "ConfigParser/ServerConfig.hpp"

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

class ServerManager
{
public:

    ServerManager(std::vector<ServerConfig> configs);
    ~ServerManager();

    void    exitError(const std::string &str);
    void    log(const std::string &message);

private:

    std::vector<ServerConfig>       _configs;
    std::vector<Server>             _servers;

    std::vector<struct pollfd>      _pollfds;
    std::map<int, Server>           _requestServerIndex;
    std::map<int, RequestParser>    _requests;
    std::map<int, long>             _timeOutIndex;


    int             startServer();
    void            closeServer();

    void            startListen();
    void            acceptConnection(int incoming);

    int             sendResponse(int socket_fd);
    void            checkTimeout(void);
};

#endif
