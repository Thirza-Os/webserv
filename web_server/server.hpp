#ifndef SERVER_HPP
# define SERVER_HPP

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

class server
{

public:
    server(serverConfig config);
    server();
    ~server();

	int					getSocket() const;
	struct sockaddr_in	getSockAddr() const;
    serverConfig        getConfig() const;

private:

    serverConfig					_config;
    int                             _socket;

    struct sockaddr_in				_socketAddr;

    void             	startServer();
    void            	closeServer();
	void    			exitError(const std::string &str);
};

#endif
