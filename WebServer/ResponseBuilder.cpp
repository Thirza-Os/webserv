#include "ResponseBuilder.hpp"
#include "CgiHandler/CgiHandler.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

ResponseBuilder::ResponseBuilder(RequestParser request, ServerConfig config): _request(request), _config(config) {
    std::cout << "Building response.." << std::endl;
    this->_status_code = this->_request.get_status_code();
    if (this->_request.get_method() == "GET"){
        build_response();
    }
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

//if a config file has 2 locations with returns pointing at each other
//that would likely cause a stack overflow because of the recursion here
//maybe try to catch that somehow?
Location    ResponseBuilder::match_location(std::string uri) {
    Location empty{};
    std::vector<Location> locations = this->_config.get_locations();
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
        if (it->path != "/") {
            if (uri.find(it->path, 0) != std::string::npos) {
                if (it->returnPath.empty()) {
                    std::cout << "Location matched" << std::endl;
                    return (*it);
                }
                else {
                    return (match_location(it->returnPath));
                }
            }
        }
    }
    //loop again and find a potential match for just a '/' which should always match if it exists
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
        if (it->path == "/") {
            if (it->returnPath.empty()) {
                std::cout << "Location matched, just /" << std::endl;
                return (*it);
            }
            else {
                return (match_location(it->returnPath));
            }
        }
    }
    std::cout << "No location match" << std::endl;
    //if there are no locations or no matches, return an empty location struct
    return (empty);
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
    else if (this->_status_code == 400)
        ss << " Bad Request\n";
    else if (this->_status_code == 401)
        ss << " Unauthorized\n";
    else if (this->_status_code == 404)
        ss << " Not Found\n";
    else if (this->_status_code == 405)
        ss << " Method Not Allowed\n";
    else
        ss << " Some Other Status\n"; //expand on this later
    if (this->_request.get_content_type().empty())
        ss << "Content-Type: text/html\n";
    else
        ss << this->_request.get_content_type() << "\n";
    ss << "Content-Length: " << this->_body.size() << "\n\n"; //header ends with an empty line
	this->_header = ss.str();
}

std::string ResponseBuilder::process_uri() {
    std::cout << "Processing uri.." << std::endl;
    std::string uri = this->_request.get_uri();
    Location matched_loc = match_location(uri);
    std::string dflt_index;
    if (!this->_config.get_index().empty()) {
        dflt_index = this->_config.get_index().front();
    }
    if (dflt_index.empty()) {
        dflt_index = "index.html";
    }
    if (!matched_loc.path.empty()) {
        if (matched_loc.methods[0]) { //check if GET method is even allowed or not
        std::cout << "matched a location " << matched_loc.path << "!" << std::endl;
        std::cout << "index: " << matched_loc.index << std::endl;
        std::cout << "requested uri: " << uri << std::endl;
            if (uri.find(matched_loc.path, 0) == std::string::npos) {
                //if uri and path don't match, this was a redirect
                //and we should change the uri accordingly
                //erase until a slash then add path instead
                //if there are no slashes, replace whole uri with path
                if (uri.front() == '/') {
                    uri.erase(0, 1);
                }
                if (uri.find_first_of('/') != std::string::npos) {
                    uri.erase(0, uri.find_first_of('/'));
                    uri.insert(0, matched_loc.path);
                }
                else {
                    uri = matched_loc.path;
                }
            }
            if (matched_loc.root.empty()) {
                uri.insert(0, this->_config.get_rootdirectory());
            }
            uri.insert(0, matched_loc.root);
            if (!matched_loc.index.empty()) {
                dflt_index = matched_loc.index;
            }
            if (uri.back() == '/') {
                uri.append(dflt_index);
            }
            if (!matched_loc.cgiExtensions.empty()) {
                std::cout << "cgi found in matched location!" << std::endl;
                CgiHandler cgi(matched_loc, this->_request);
                //the output of the script can be read from pipe_end[0],
                //and set to this->_response to be sent to the client as is
                return ("");
            }
        }
        else {
            this->_status_code = 405;
        }
    }
    else {
        uri.insert(0, "/");
        uri.insert(0, this->_config.get_rootdirectory());
        if (uri.back() == '/') {
            uri.append(dflt_index);
        }
    }
    struct stat s;
    if (lstat(uri.c_str(), &s) == 0) {
        if (S_ISDIR(s.st_mode)) {
            std::cout << "is a directory" << std::endl;
            uri.append("/");
            uri.append(dflt_index);
        }
    }
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
    if (uri.empty()) { //cgi handles response
        return ;
    }
    std::ifstream htmlFile(uri.c_str());
    if (this->_status_code == 405) {
        std::cout << "Method not allowed" << std::endl;
        htmlFile.close();
        if (this->_config.get_errorpages().count(this->_status_code)) {
            htmlFile.open(this->_config.get_errorpages().at(this->_status_code));
        }
        else {
            htmlFile.open("WebServer/ConfigParser/DefaultErrorPages/405MethodNotAllowed.html");
        }
    }
    else if (!htmlFile.good()) {
        htmlFile.close();
        std::cout << "file can't be opened" << std::endl;
        this->_status_code = 404;
        if (this->_config.get_errorpages().count(this->_status_code)) {
            htmlFile.open(this->_config.get_errorpages().at(this->_status_code));
        }
        else {
            htmlFile.open("WebServer/ConfigParser/DefaultErrorPages/404NotFound.html");
        }
    }
    if (!htmlFile.good()) {
        htmlFile.close();
        std::cout << "error page can't be opened either for some reason" << std::endl;
        build_header();
        this->_response = this->_header;
        return ;
    }
    std::stringstream buffer;
    buffer << htmlFile.rdbuf();
	htmlFile.close();
    this->_body = buffer.str();
    build_header();
	this->_response = this->_header;
	this->_response.append(this->_body);
}

std::string ResponseBuilder::get_response() {
	return (this->_response);
}

std::string ResponseBuilder::get_header() {
	return (this->_header);
}
