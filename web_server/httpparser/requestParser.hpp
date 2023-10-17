#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

#include <string>
#include <map>
#include <vector>

class requestParser
{
private:
    std::string                         _request;

    std::string                         _method;
    std::string                         _uri;
    std::string                         _protocol;

    std::map<std::string, std::string>  _headers;

    std::string                         _body;

    int                                 _status_code;

    bool                                _ParsingCompleted;

    void        add_header(std::string key, std::string value);
    void        consume_request();
    void        parse_error(const std::string &str, int code);
    void        tokenize(const std::string& str, std::vector<std::string>& tokens, char delimiter);
    void        validate_request_line();
    void        decode_uri();
    void        validate_header(std::string line);
    bool        validate_content(std::string line);

    void        print_request() const;

public:
    requestParser(std::string request);
    requestParser();
    requestParser(const requestParser &src);
    ~requestParser();
    requestParser &operator=(const requestParser &src);

    bool        parsing_completed() const;
    std::string get_method() const;
    std::string get_uri() const;
    std::string get_protocol() const;
    std::string get_body() const;
    std::string get_content_type() const;
    size_t      get_content_length() const;
    int         get_status_code() const;

    std::string find_header(std::string key);
};

enum ParseState {
    RequestLineParsing,
    HeadersParsing,
    BodyParsing
};

// enum httpMethod
// {
//     GET,            // Read only: The HTTP GET request method is used to request a resource from the server. 
//     POST,           // Send data to server, create new resource: create new data entry
//     DELETE
// };

#endif