#ifndef RESPONSEBIULDER_HPP
# define RESPONSEBIULDER_HPP

#include "httpparser/requestParser.hpp"
#include "configparser/serverConfig.hpp"
#include <string>

class responseBuilder
{
private:
    requestParser                       _request;
    serverConfig                        _config;
    std::string                         _header;
    std::string               			_body;
    std::string                         _response;
    int                                 _status_code;


	std::string	getContentInfo(std::string content_type, std::string info);
	void		upload_file();
    void        build_response();
    void        build_header();
    std::string process_uri();

public:
    responseBuilder(requestParser request, serverConfig config);
    responseBuilder(const responseBuilder &src);
    ~responseBuilder();
    responseBuilder &operator=(const responseBuilder &src);

    std::string     getResponse();
    std::string     getHeader();
};

#endif
