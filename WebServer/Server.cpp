#include "Server.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>

Server::Server(ServerConfig config): _config(config) {
	_socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(_config.get_port());
    _socketAddr.sin_addr.s_addr = _config.get_host();
	start_server();
}

Server::Server(){}

Server::~Server(){}

void    Server::exit_error(const std::string &str)
{
    std::cerr << "Error - " << str << std::endl;
    exit(1);
}

void	Server::start_server()
{
    // making the socket
    std::cout << "Starting socket" << std::endl;

    _socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if (_socket < 0)
    {
        exit_error("Socket failed!");
    }
    
    int enable = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        exit_error("setsockopt() failed");
    }

	// binding the socket
    if (bind(_socket, (sockaddr *)&_socketAddr, sizeof(_socketAddr)) < 0)
    {
        exit_error("Cannot connect socket to address");
    }
}

int Server::get_socket() const {
    return(this->_socket);
}

struct sockaddr_in  Server::get_sock_addr() const {
    return(this->_socketAddr);
}

ServerConfig        Server::get_config() const {
    return(this->_config);
}
