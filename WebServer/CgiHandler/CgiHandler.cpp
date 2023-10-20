#include "CgiHandler.hpp"
#include <string>
#include <iostream>
#include <map>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>

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
    std::map<std::string, std::string>::const_iterator it = loc.cgiExtensions.begin();
    // Only accessing first key & value, in case we want to expand to multiple we can make this a loop
    const std::string& firstKey = it->first;
    const std::string& firstValue = it->second;
   
    this->_environment["GATEWAY_INTERFACE"] = std::string("CGI/1.1");
    this->_environment["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_environment["REDIRECT_STATUS"] = "200";              // Hardcoded this as well, means succesfull response
    this->_environment["SERVER_PORT"] = "8081";                 // hardcoded this for now
    this->_environment["SERVER_SOFTWARE"] = "cool_server1.0";
    this->_environment["PATH_INFO"] = loc.root + loc.path;      // says so in the subject
    this->_environment["SCRIPT_NAME"] = firstValue;
    this->_environment["SCRIPT_FILENAME"] = loc.root + loc.path;
    this->_environment["REQUEST_METHOD"] = httprequest.get_method();
    this->_environment["REQUEST_URI"] = httprequest.get_uri();
}

void    CgiHandler::execute_script() {
    try {
        pid_t childPid = fork();
        if (childPid == -1) {
            throw CgiException("Fork error");
        }
        if (childPid == 0) {
            // child
            for (std::map<std::string, std::string>::const_iterator it = this->_environment.begin(); it != this->_environment.end(); ++it) {
                std::string envVar = it->first + "=" + it->second;
            char *env = new char[envVar.length() + 1];
            strcpy(env, envVar.c_str());
            this->_childEnvp.push_back(env);
        }
        this->_childEnvp.push_back(nullptr);

        if (execve(this->_environment["SCRIPT_FILENAME"].c_str(), nullptr, &this->_childEnvp[0]) == -1);
            throw CgiException("Execve failed");
        } else {
        int status;
        waitpid(childPid, &status, 0);
        }
    }   catch (const CgiException& e) {
            // HANDLE EXCEPTION: LOGGING?
            std::cerr << "Error: " << e.what() << std::endl;
            exit(1);
    }
}

std::map<std::string, std::string>  CgiHandler::get_environment() const {
    return this->_environment;
}

void    CgiHandler::print_env(){
    for (std::map<std::string, std::string>::const_iterator it = this->_environment.begin(); it != this->_environment.end(); ++it) {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }}
