#include "ResponseBuilder.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>

ResponseBuilder::ResponseBuilder(RequestParser request, ServerConfig config): _request(request), _config(config) {
    this->_status_code = this->_request.get_status_code();
    build_response();
}

ResponseBuilder::ResponseBuilder(const ResponseBuilder &src) {
    *this = src;
}

ResponseBuilder::~ResponseBuilder() {}

ResponseBuilder &ResponseBuilder::operator=(const ResponseBuilder &src)
{
    if (this != &src) {
        this->_request = src._request;
        this->_header = src._header;
		this->_body = src._body;
		this->_response = src._response;
    }
    return *this;
}

void        ResponseBuilder::build_header() {
    std::ostringstream ss;
    if (this->_request.get_protocol().empty())
        ss << "HTTP/1.1";
    else
        ss << this->_request.get_protocol();
    ss << " " << this->_status_code;
    if (this->_status_code == 200)
        ss << " OK\n";
    else if (this->_status_code == 404)
        ss << " Not Found\n";
    else
        ss << " Some Other Status\n"; //expand on this later
    if (this->_request.get_content_type().empty())
        ss << "Content-Type: text/html\n";
    else
        ss << this->_request.get_content_type() << "\n";
    ss << "Content-Length: " << this->_body.size() << "\n\n";
	this->_header = ss.str();
}

std::string ResponseBuilder::process_uri() {
    std::string uri = this->_request.get_uri();
    uri.insert(0, "WebServer/www/penguinserv");
    if (uri.back() == '/') {
        uri.append("index.html"); //or whatever config file says is the default
    }
    //uri.erase(0, 1);
    std::cout << "attempting to send this uri: " << uri << std::endl;
    return (uri);
}

void	ResponseBuilder::build_response() {
    if (this->_status_code == 400) {
        std::cout << "Bad request" << std::endl;
        build_header();
        this->_response = this->_header;
        return ;
    }
    std::string uri = process_uri();
    std::ifstream htmlFile(uri.c_str());
    if (!htmlFile.good()) {
        htmlFile.close();
        std::cout << "file can't be opened" << std::endl;
        this->_status_code = 404;
        htmlFile.open("WebServer/www/penguinserv/errorPages/404Error.html");
        if (!htmlFile.good()) {
            htmlFile.close();
            std::cout << "error page can't be opened either" << std::endl;
            build_header();
            this->_response = this->_header;
            return ;
        }
    }
    std::stringstream buffer;
    buffer << htmlFile.rdbuf();
	htmlFile.close();
    this->_body = buffer.str();
    build_header();
	this->_response = this->_header;
	this->_response.append(this->_body);
}

std::string ResponseBuilder::getResponse() {
	return (this->_response);
}

std::string ResponseBuilder::getHeader() {
	return (this->_header);
}
