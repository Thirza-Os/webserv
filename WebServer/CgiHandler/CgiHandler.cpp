#include "CgiHandler.hpp"
#include <string>
#include <iostream>
#include <map>

CgiHandler::CgiHandler(Location const &loc, RequestParser const &httprequest) {
    std::cout << "constructor started for CGI handler " << std::endl;
    initialize_environment(loc, httprequest);
    print_env();
}

CgiHandler::~CgiHandler()
{
}

CgiHandler::CgiHandler(const CgiHandler &src) {
    *this = src;
}

CgiHandler &CgiHandler::operator=(const CgiHandler &src) {
    if (this != &src) {
        this->_environment = src._environment;
    }
    return *this;
}

void    CgiHandler::initialize_environment(Location const &loc, RequestParser const &httprequest)
{
    this->_environment["GATEWAY_INTERFACE"] = std::string("CGI/1.1");
    this->_environment["PATH_INFO"] = loc.root + loc.path;
    this->_environment["SCRIPT_FILENAME"] = loc.root + loc.path;
    this->_environment["REQUEST_METHOD"] = httprequest.get_method();
    this->_environment["REQUEST_URI"] = httprequest.get_uri();
}

std::map<std::string, std::string>  CgiHandler::get_environment() const {
    return this->_environment;
}

void    CgiHandler::print_env(){
    for (std::map<std::string, std::string>::const_iterator it = this->_environment.begin(); it != this->_environment.end(); ++it) {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }}
