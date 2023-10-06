#include "requestParser.hpp"
#include "../utilities/utilities.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>

// CHECK: Check if everything looks well with the whitespaces trim, or more chars have to be added.

requestParser::requestParser(std::string request): _request(request), _status_code(200), _ParsingCompleted(false) {
    consume_request();
    print_request();
}

requestParser::requestParser(): _request(""), _status_code(200), _ParsingCompleted(false) {
    //consume_request();
    //print_request();
}

requestParser::requestParser(const requestParser &src) {
    *this = src;
}

requestParser::~requestParser() {}

void    requestParser::add_header(std::string key, std::string value) {
    this->_headers[key] = value;
}

requestParser &requestParser::operator=(const requestParser &src)
{
    if (this != &src) {
        this->_request = src._request;
        this->_method = src._method;
        this->_uri = src._uri;
        this->_protocol = src._protocol;
        this->_headers = src._headers;
        this->_body = src._body;
        this->_ParsingCompleted = src._ParsingCompleted;
        this->_status_code = src._status_code;
    }
    return *this;
}

bool    requestParser::parsingCompleted() const {
    return this->_ParsingCompleted;
}

std::string requestParser::get_method() const {
    return (this->_method);
}

std::string requestParser::get_uri() const {
    return (this->_uri);
}

std::string requestParser::get_protocol() const {
    return (this->_protocol);
}

std::string requestParser::get_body() const {
    return (this->_body);
}

std::string requestParser::get_content_type() const {
    if (_headers.count("Content-Type") > 0) {
        return _headers.at("Content-Type");
    } else {
        return "";
    }
}

size_t requestParser::get_content_length() const {
    if (_headers.count("Content-Length") > 0) {
        return std::stoi(_headers.at("Content-Length"));
    } else {
        return 0;
    }
}

int requestParser::get_status_code() const {
    return (this->_status_code);
}

void    requestParser::parse_error(const std::string &str, int code) {
    this->_status_code = code;
    std::cerr << code << " " << str << std::endl;
}

void    requestParser::tokenize(const std::string& str, std::vector<std::string>& tokens, char delimiter) {
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
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
            } else 
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
    } else
    {
        parse_error("Invalid Header Format", 400);
    }
}

bool    requestParser::validate_content(std::string line){
    if (line.empty())
        return(0);

    return(0);
}

void requestParser::consume_request(){

    std::string                 raw_request = this->_request;
    std::vector<std::string>    parsed_request;
    std::istringstream          iss(raw_request);
    std::string                 line;
    std::vector<std::string>    words;
    //size_t                      colon_pos;

    ParseState                  state = RequestLineParsing;

    if (raw_request.empty())
        parse_error("Bad Request", 400);
    //char first_char = line[0];
    

    while (std::getline(iss, line)) {
        parsed_request.push_back(line);
        utility::stringTrim(line, "\r");
        
        switch(state) {
            case RequestLineParsing:
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

            state = HeadersParsing;
            break;

            case HeadersParsing:
                if (line.empty()) {
                    state = BodyParsing;
                    break;
                } else
                    validate_header(line);
                break;

            case BodyParsing:
                if (validate_content(line))
                    break;
                this->_body += line + '\n';
                break;
        }
    }
    this->_ParsingCompleted = true;
}

void requestParser::print_request() const {
    std::cout << "Request: " << _request << std::endl;
    std::cout << "Method: " << _method << std::endl;
    std::cout << "URI: " << _uri << std::endl;
    std::cout << "Protocol: " << _protocol << std::endl;

    std::cout << "Headers:" << std::endl;
    for (const auto& header : _headers) {
        std::cout << "  " << header.first << ": " << header.second << std::endl;
    }

    std::cout << "Body: " << _body << std::endl;

    std::cout << "Content length: " << this->get_content_length() << std::endl;
    std::cout << "Content type: " << this->get_content_type() << std::endl;

}
