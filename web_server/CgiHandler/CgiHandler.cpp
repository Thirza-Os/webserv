#include "CgiHandler.hpp"
#include <string>
#include <iostream>
#include <map>

CgiHandler::CgiHandler(const Location &loc): _variables(loc)
{
    std::cout << "constructor started for CGI handler " << std::endl;
}

CgiHandler::~CgiHandler()
{
}

void    CgiHandler::initialize_environment(Location loc)
{
    this->_environment["PATH_INFO"] = this->_variables.path;
    this->_environment["SCRIPT_FILENAME"] = this->_variables.path;
}