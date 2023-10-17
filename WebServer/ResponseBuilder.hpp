#ifndef RESPONSEBIULDER_HPP
# define RESPONSEBIULDER_HPP

#include "HttpParser/RequestParser.hpp"
#include "ConfigParser/ServerConfig.hpp"

#include <string>

class ResponseBuilder
{
private:
    RequestParser                       _request;
    ServerConfig                        _config;
    std::string                         _header;
    std::string                         _body;
    std::string                         _response;
    int                                 _status_code;

    void        build_response();
    void        build_header();
    std::string process_uri();

public:
    ResponseBuilder(RequestParser request, ServerConfig config);
    ResponseBuilder(const ResponseBuilder &src);
    ~ResponseBuilder();
    ResponseBuilder &operator=(const ResponseBuilder &src);

    std::string     getResponse();
    std::string     getHeader();
};

#endif
