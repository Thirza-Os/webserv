#include "CgiHandler.hpp"
#include <string>
#include <iostream>
#include <map>

CgiHandler::CgiHandler(const Location &loc): _variables(loc)
{
    std::cout << "constructor started for CGI handler " << std::endl;
    initialize_environment(this->_variables);
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
        this->_variables = src._variables;
    }
    return *this;
}

void    CgiHandler::initialize_environment(Location loc)
{
    this->_environment["PATH_INFO"] = this->_variables.root + this->_variables.path;
    this->_environment["SCRIPT_FILENAME"] = this->_variables.root + this->_variables.path;
}

std::map<std::string, std::string>  CgiHandler::get_environment() const {
    return this->_environment;
}

