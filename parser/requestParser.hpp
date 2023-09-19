#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <string>
# include <map>

// An HttpRequest object receives an incoming request, parses it, and extracts the relevant information.
// info: method, path, headers, and message body(if present). 
// If any syntax error was found in the request during parsing, error flags are set and parsing stops. 

class requestParser
{
private:
    size_t                              _len;
    std::string                         _request;

    std::string                         _method;
    std::string                         _uri;
    std::string                         _protocol;

    std::map<std::string, std::string>  _headers;

    std::string                         _body;

    void    add_header(std::string key, std::string value);
    void    consume_request();
    void    parse_error(const std::string &str);

public:
    // requestParser();
    requestParser(std::string request, size_t len);
    requestParser(const requestParser &src);
    ~requestParser();

};

// enum parseState
// {

// };

enum httpMethod
{
    GET,            // Read only: The HTTP GET request method is used to request a resource from the server. 
    POST,           // Send data to server, create new resource: create new data entry
    DELETE
};

#endif