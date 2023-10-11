#ifndef SERVER_HPP
# define SERVER_HPP

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

class server
{

public:
    server(serverConfig config);
    server();
    ~server();

	int					getSocket() const;
	struct sockaddr_in	getSockAddr() const;

private:

    serverConfig					_config;
    int                             _socket;

    struct sockaddr_in				_socketAddr;
    //std::map<int, requestParser>    _requests;

    void             	startServer();
    void            	closeServer();
	void    			exitError(const std::string &str);
};

#endif
