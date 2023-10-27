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

	std::string	getContentInfo(std::string content_type, std::string info);
	void		upload_file();
	void		delete_resource();

    void        build_response();
    void        build_header();
    std::string process_uri();
    Location    match_location(std::string uri);

public:
    ResponseBuilder(RequestParser request, ServerConfig config);
    ResponseBuilder(const ResponseBuilder &src);
    ~ResponseBuilder();
    ResponseBuilder &operator=(const ResponseBuilder &src);

    std::string     get_response();
    std::string     get_header();
};

#endif
