#include "requestParser.hpp"

#include <iostream>


// Request Line:
//      what-where-which protocol
//      HTTP-METHOD URI HTTP-PROTOCOL
// Request Header:
//      name and value, metadata of request
//      NAME : VALUE
// Request Body


int main(int, char**)
{
    const char text[] =
        "GET /index HTTP/1.1"
        "Host: 192.168.0.199:80"
        "Connection: keep-alive"
;

    requestParser parser(text, strlen(text));

    // HttpResponseParser::ParseResult res = parser.parse(response, text, text + strlen(text));

    // if( res == HttpResponseParser::ParsingCompleted )
    // {
    //     std::cout << response.inspect() << std::endl;
    //     return EXIT_SUCCESS;
    // }
    // else
    // {
    //     std::cerr << "Parsing failed" << std::endl;
    //     return EXIT_FAILURE;
    // }
}