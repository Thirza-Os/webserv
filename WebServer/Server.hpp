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

class Server
{

public:
    Server(ServerConfig config);
    Server();
    ~Server();

	int					getSocket() const;
	struct sockaddr_in	getSockAddr() const;
    ServerConfig        getConfig() const;

private:

    ServerConfig					_config;
    int                             _socket;

    struct sockaddr_in				_socketAddr;

    void             	startServer();
    void            	closeServer();
	void    			exitError(const std::string &str);
};

#endif
