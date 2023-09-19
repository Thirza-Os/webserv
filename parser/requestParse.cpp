#include "requestParser.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>



// default
// requestParser::requestParser()
// {

// }

requestParser::requestParser(std::string request, size_t len): _request(request), _len(len) {}

requestParser::requestParser(const requestParser &src) {
    
    *this = src;
}

requestParser::~requestParser() {}

void    requestParser::add_header(std::string key, std::string value) {
    
    this->_headers[key] = value;
}

void    requestParser::parse_error(const std::string &str)
{
    std::cerr << "Error - " << str << std::endl;
}

void requestParser::consume_request(){

    std::string request = this->_request;
    std::vector<std::string> lines;
    std::istringstream iss(request);
    std::string line;

    if (request.empty())
        parse_error("Empty HTTP request");
    char firstChar = line[0];
    std::string charSet = "GPD";
    if (std::find(charSet.begin(), charSet.end(), firstChar) == charSet.end()) {
        parse_error("Not a supported HTTP request");
    
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
}