#include "serverConfig.hpp"

serverConfig::serverConfig() {
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
    this->_port.push_back(port);
}

void serverConfig::setHost(const std::string host) {
    _host = host;
}

void serverConfig::setServerName(const std::string serverName) {
    _serverName = serverName;
}

void serverConfig::setMaxSize(size_t maxSize) {
    _maxSize = maxSize;
}

void serverConfig::setErrorPages(const std::string errorPage) {
    _errorPages.push_back(errorPage);
}

void serverConfig::setRootDirectory(const std::string rootDirectory) {
    _rootDirectory = rootDirectory;
}

void serverConfig::setIndex(const std::string index) {
    _index = index;
}

// void serverConfig::addLocation(const Location& location) {
//     _locations.push_back(location);
// }

// void serverConfig::setSocketAddr(const struct sockaddr_in& socketAddr) {
//     _socketAddr = socketAddr;
// }