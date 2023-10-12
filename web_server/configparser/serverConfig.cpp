#include "serverConfig.hpp"

#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <string>

serverConfig::serverConfig():
    _port(80),
    _serverName(""),
    _maxSize(-1),
    _errorPages(),
    _rootDirectory(""),
    _index("") {
}

serverConfig::~serverConfig() {
}

serverConfig::serverConfig(const serverConfig &src) {
    *this = src;
}

serverConfig &serverConfig::operator=(const serverConfig &src) {
    if (this != &src) {
        this->_port = src._port;
        this->_host = src._host;
        this->_serverName = src._serverName;
        this->_maxSize = src._maxSize;
        this->_errorPages = src._errorPages;
        this->_rootDirectory = src._rootDirectory;
        this->_index = src._index;
        this->_locations = src._locations;
        // ADD STRUCT COPY HERE
    }
    return *this;
}

void serverConfig::setPort(int port) {
    this->_port = port;
}

void serverConfig::setHost(in_addr_t &host) {
    this->_host = host;
}

void serverConfig::setServerName(const std::string serverName) {
    this->_serverName = serverName;
}

void serverConfig::setMethods(std::string &methodString) {
    this->_methods = methodString;
    // if (methodString.find("GET") != std::string::npos) {
    //     this->_.set(0, true); // Set the first bit for GET
    // }
    // if (methodString.find("POST") != std::string::npos) {
    //    this-> _methods.set(1, true); // Set the second bit for POST
    // }
    // if (methodString.find("PUT") != std::string::npos) {
    //     this->_methods.set(2, true); // Set the third bit for PUT
    // }
}

void serverConfig::setMaxSize(size_t maxSize) {
    this->_maxSize = maxSize;
}

void serverConfig::setErrorPages(const std::string errorPage) {
    this->_errorPages.push_back(errorPage);
}

void serverConfig::setRootDirectory(const std::string rootDirectory) {
    this->_rootDirectory = rootDirectory;
}

void serverConfig::setIndex(const std::string index) {
    this->_index = index;
}

// void serverConfig::addLocation(const Location& location) {
//     _locations.push_back(location);
// }

// void serverConfig::setSocketAddr(const struct sockaddr_in& socketAddr) {
//     _socketAddr = socketAddr;
// }

int serverConfig::getPort() const {
    return (this->_port);
}

in_addr_t   serverConfig::getHost() const {
    return (this->_host);
}
