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

    ServerManager(const std::vector<ServerConfig> &configs);
    ~ServerManager();

    void    exit_error(const std::string &str);
    void    log(const std::string &message);

private:

    std::vector<Server>                 _servers;

    std::vector<struct pollfd>          _pollfds;
    std::map<int, Server>               _requestServerIndex;
    std::map<int, RequestParser>        _requests;
    std::map<int, long>                 _timeOutIndex;
    std::map<int, int>                  _cgiIndex;
    std::map<int, std::string>          _cgiResponseIndex;


    int             start_server();
    void            close_server();

    void            start_listen();
    void            accept_connection(int incoming);

    int             send_response(int socket_fd);
    void            check_timeout(void);
};

#endif
