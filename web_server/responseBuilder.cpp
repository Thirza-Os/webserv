#include "responseBuilder.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <regex>

responseBuilder::responseBuilder(requestParser request, serverConfig config): _request(request), _config(config) {
    this->_status_code = this->_request.get_status_code();
    build_response();
}

responseBuilder::responseBuilder(const responseBuilder &src) {
    *this = src;
}

responseBuilder::~responseBuilder() {}

responseBuilder &responseBuilder::operator=(const responseBuilder &src)
{
    if (this != &src) {
        this->_request = src._request;
        this->_header = src._header;
		this->_body = src._body;
		this->_response = src._response;
    }
    return *this;
}

void        responseBuilder::build_header() {
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
    ss << "Connection: close\n";
    ss << "Content-Length: " << this->_body.size() << "\n\n";
	this->_header = ss.str();
}

std::string responseBuilder::process_uri() {
    std::string uri = this->_request.get_uri();
    uri.insert(0, "web_server/www/penguinserv");
    if (uri.back() == '/') {
        uri.append("index.html"); //or whatever config file says is the default
    }
    //uri.erase(0, 1);
    std::cout << "attempting to send this uri: " << uri << std::endl;
    return (uri);
}


void responseBuilder::handle_post()
{
	size_t boundary_pos = this->_request.get_content_type().find("boundary=");
	std::string boundary = "--" + this->_request.get_content_type().substr(boundary_pos + 9, std::string::npos);
	
	char* raw_data = this->_request.get_body();

	std::istringstream body(this->_request.get_body());
	std::stringstream data;
	std::string filename;
	std::string line;

	while (std::getline(body, line))
	{
		std::ofstream newfile;
		if (line.find("Content-Disposition:") != std::string::npos)
		{
			size_t filepos = line.find("filename=\"") + 9;
			filename = line.substr(filepos, std::string::npos);
			filename.erase(std::remove(filename.begin(), filename.end(), '\"'), filename.end());
		}
		newfile.open(filename, std::ios::binary);
		//CHECKEN VOOR OPEN FILE

		if (line.find("Content-Type:") != std::string::npos)
		{
			std::getline(body,line);
			while (std::getline(body, line))
			{
				if (line.find(boundary) == std::string::npos)
				{
					data << line;
					
				}
			}
			newfile.write(data.str().c_str(), this->_request.get_content_length());
			newfile.close();
			continue;
		}
		std::cout << line.length() << "body line: " << line << std::endl;
	}
}

void	responseBuilder::build_response() {
    std::string uri = process_uri();
	if (this->_request.get_method() == "POST")
	{
		handle_post();
	}
    std::ifstream htmlFile(uri.c_str());
    if (!htmlFile.good()) {
        htmlFile.close();
        std::cout << "file can't be opened" << std::endl;
        this->_status_code = 404;
        htmlFile.open("web_server/www/penguinserv/errorPages/404Error.html");
    }
    std::stringstream buffer;
    buffer << htmlFile.rdbuf();
	htmlFile.close();
    this->_body = buffer.str();
    build_header();
	this->_response = this->_header;
	this->_response.append(this->_body);
}

std::string responseBuilder::getResponse() {
	return (this->_response);
}

std::string responseBuilder::getHeader() {
	return (this->_header);
}
