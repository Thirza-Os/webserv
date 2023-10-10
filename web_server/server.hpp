#ifndef SERVER_HPP
# define SERVER_HPP

#include "serverManager.hpp"

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
    server(serverConfig> config);
    ~server();

	int					getSocket() const;
	struct sockaddr_in	getSockAddr() const;

private:
    serverConfig					_config;
    int                             _socket;

    struct sockaddr_in				_socketAddr;
    unsigned int                    _socketAddrLen;
    //std::map<int, requestParser>    _requests;

    int             	startServer();
    void            	closeServer();
	void    			exitError(const std::string &str);
};

#endif
