#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include "httpparser/requestParser.hpp"
#include "responseBuilder.hpp"
#include "server.hpp"
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

class serverManager
{
public:

    serverManager(std::vector<serverConfig> configs);
    ~serverManager();

    void    exitError(const std::string &str);
    void    log(const std::string &message);

private:

    std::vector<serverConfig>       _configs;
    std::vector<server>             _servers;

    std::vector<struct pollfd>      _pollfds;
    std::map<int, server>           _requestServerIndex;
    std::map<int, requestParser>    _requests;


    int             startServer();
    void            closeServer();

    void            startListen();
    void            acceptConnection(int incoming);

    void            sendResponse(int socket_fd);
};

#endif
