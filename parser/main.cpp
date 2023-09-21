#include "requestParser.hpp"

#include <iostream>
#include <cstring>

// Request Line:
//      what-where-which protocol
//      HTTP-METHOD URI HTTP-PROTOCOL
// Request Header:
//      name and value, metadata of request
//      NAME : VALUE
// Request Body


int main(int, char**)
{
const char* httpRequest = "GET /index%21 HTTP/1.1\r\n"
                          "Host: 192.168.0.199:80\r\n"
                          "Connection: keep-alive\r\n\r\n";

    requestParser parser(httpRequest, strlen(httpRequest));

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