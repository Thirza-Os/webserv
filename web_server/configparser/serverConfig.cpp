#include "serverConfig.hpp"

#include <netinet/in.h>


serverConfig::serverConfig():
    _port(80),
    _serverName(""),
    _maxSize(-1),
    _errorPages(),
    _rootDirectory("") {}

serverConfig::~serverConfig() {}

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

void serverConfig::set_port(int port) {
    this->_port = port;
}

void serverConfig::set_host(in_addr_t &host) {
    this->_host = host;
}

void serverConfig::set_servername(const std::string serverName) {
    this->_serverName = serverName;
}

void serverConfig::set_methods(std::bitset<3> &methodString) {
    this->_methods = methodString;
}

void serverConfig::set_maxsize(size_t maxSize) {
    this->_maxSize = maxSize;
}

void serverConfig::set_error_pages(int code, std::string &errorPage) {
    _errorPages[code] = errorPage;
}

void serverConfig::set_rootdirectory(const std::string rootDirectory) {
    this->_rootDirectory = rootDirectory;
}

void serverConfig::set_index(std::vector<std::string> &index) {
    this->_index = index;
}

void serverConfig::set_location(const Location& location) {
    _locations.push_back(location);
}

// void serverConfig::setSocketAddr(const struct sockaddr_in& socketAddr) {
//     _socketAddr = socketAddr;
// }

int serverConfig::get_port() const {
    return (this->_port);
}

in_addr_t   serverConfig::get_host() const {
    return (this->_host);
}

std::string   serverConfig::get_rootdirectory() const {
    return (this->_rootDirectory);
}