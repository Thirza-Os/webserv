#include "server.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>

server::server(serverConfig config): _config(config) {
	_socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(_config.getPort());
    _socketAddr.sin_addr.s_addr = _config.getHost();
	startServer();
}

void    server::exitError(const std::string &str)
{
    std::cerr << "Error - " << str << std::endl;
    exit(1);
}

void	server::startServer()
{
    // making the socket
    std::cout << "Starting socket" << std::endl;

    _socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if (_socket < 0)
    {
        exitError("Socket failed!");
    }
    
    int enable = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        exitError("setsockopt() failed");
    }

	// binding the socket
    if (bind(_socket, (sockaddr *)&_socketAddr, _socketAddrLen) < 0)
    {
        exitError("Cannot connect socket to address");
    }
}

	int	server::getSocket() const {
		return(this->_socket);
	}

	struct sockaddr_in	server::getSockAddr() const {
		return(this->_socketAddr);
	}
