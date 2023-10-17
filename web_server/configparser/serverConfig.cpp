#include "serverConfig.hpp"

#include <netinet/in.h>
#include <iostream>

serverConfig::serverConfig():
    _port(80),
    _host(0),
    _serverName(""),
    _maxSize(0),
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
        this->_cgiExtensions = src._cgiExtensions;
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

void serverConfig::set_maxsize(size_t maxSize) {
    this->_maxSize = maxSize;
}

void serverConfig::set_error_pages(int code, std::string &errorPage) {
   this->_errorPages[code] = errorPage;
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

void serverConfig::set_cgiExtensions(const std::string ext, const std::string program) {
    this->_cgiExtensions[ext] = program;
        std::cout << "SETTER" <<std::endl << std::endl;
    std::map<std::string, std::string> cgi = this->_cgiExtensions;
            for (std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it) {
            std::cout << "cgi extension: " << it->first << ", cgi program: " << it->second << std::endl;
        }

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

std::string   serverConfig::get_servername() const {
    return (this->_serverName);
}

size_t    serverConfig::get_maxsize() const {
    return (this->_maxSize);
}

std::map<int, std::string>    serverConfig::get_errorpages() const {
    return (this->_errorPages);
}

std::string   serverConfig::get_rootdirectory() const {
    return (this->_rootDirectory);
}

std::vector<Location>   serverConfig::get_locations() const {
    return (this->_locations);
}

std::map<std::string, std::string>   serverConfig::get_cgiExtensions() const {
    std::cout << "GETTER" <<std::endl << std::endl;
    std::map<std::string, std::string> cgi = this->_cgiExtensions;
            for (std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it) {
            std::cout << "cgi extension: " << it->first << ", cgi program: " << it->second << std::endl;
        }
    return (this->_cgiExtensions);
}