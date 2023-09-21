#include "requestParser.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>



// default
// requestParser::requestParser()
// {

// }

requestParser::requestParser(std::string request, size_t len): _request(request), _len(len) {
    consume_request();
}

requestParser::requestParser(const requestParser &src) {
    
    *this = src;
}

requestParser::~requestParser() {}

void    requestParser::add_header(std::string key, std::string value) {
    
    this->_headers[key] = value;
}

void    requestParser::parse_error(const std::string &str, int code) {
    std::cerr << code << " " << str << std::endl;
}

void    requestParser::tokenize(const std::string& str, std::vector<std::string>& tokens, char delimiter) {
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
}

void    requestParser::stringTrim(std::string &str){
    // possibly more chars?
    static const char* spaces = "\r";
    str.erase(0, str.find_first_not_of(spaces));
    str.erase(str.find_last_not_of(spaces) + 1);
}

void    requestParser::validate_request_line(){
    if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")
        parse_error("Method Not Allowed", 405);
    // not sure what to check here
    // else if ()
    //         parse_error("Bad Request", 400);
    else if (this->_protocol != "HTTP/1.1")
            parse_error("HTTP Version Not Supported", 505);

}

void    requestParser::decode_uri(){

    std::string     decoded;
    size_t          len = this->_uri.length();

    for (size_t i = 0; i < len; i++)
    {
        // check if % is available and at least two more ints behind it
        if (_uri[i] == '%' && i + 2 < len && isxdigit(_uri[i + 1]) && isxdigit(_uri[i + 2]))
        {
            int hex;
            // check if its a valid hex representation
            if (sscanf(this->_uri.substr(i + 1, 2).c_str(), "%x", &hex) == 1) 
            {
                decoded += static_cast<char>(hex);
                i += 2;
            }
            else 
                decoded += this->_uri[i];
        }
        // check for spaces
        else if (this->_uri[i] == '+')
            decoded += ' ';
        else
            decoded += this->_uri[i];
    }
    this->_uri = decoded;
}

void    requestParser::validate_header(std::string line){

    size_t colon_pos = line.find(':');

    if (colon_pos != std::string::npos)
    {
        std::string header_name = line.substr(0, colon_pos);
        std::string header_value = line.substr(colon_pos + 1);

        this->_headers.insert(std::make_pair(header_name, header_value));
    }
    else
    {
        parse_error("Invalid Header Format", 400);
    }
}

void requestParser::consume_request(){

    std::string                 raw_request = this->_request;
    std::vector<std::string>    parsed_request;
    std::istringstream          iss(raw_request);
    std::string                 line;
    std::vector<std::string>    words;
    size_t                      colon_pos;

    bool requestLineProcessed = false;

    if (raw_request.empty())
        parse_error("Bad Request", 400);
    char first_char = line[0];
    
    while (std::getline(iss, line)) {
        parsed_request.push_back(line);
        if (!requestLineProcessed) {
            stringTrim(line);
            tokenize(line, words, ' ');

            if (words.size() >= 3) {
                this->_method = words[0];
                this->_uri = words[1];
                this->_protocol = words[2];
                decode_uri();
                validate_request_line();
            } else {
                parse_error("Invalid Request Format", 400);
            }
        requestLineProcessed = true;
        }
        else
        {
            validate_header(line);
        std::map<std::string, std::string>::iterator it = this->_headers.begin();
                std::advance(it, 1); // Advance by 1 to reach the second element.
        std::cout << "First Key: " << it->first << std::endl;
        std::cout << "First Value: " << it->second << std::endl;        }
    }
}