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

	int					get_socket() const;
	struct sockaddr_in	get_sock_addr() const;
    ServerConfig        get_config() const;

private:

    ServerConfig					_config;
    int                             _socket;

    struct sockaddr_in				_socketAddr;

    void             	start_server();
	void    			exit_error(const std::string &str);
};

#endif
