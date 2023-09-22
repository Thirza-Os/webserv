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
    const char* getRequest =
        "GET /path/to/resource HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: MyHttpClient\r\n"
        "Accept: */*\r\n"
        "\r\n";

    const char* postRequest =
        "POST /api/endpoint HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: MyHttpClient\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 19\r\n"
        "\r\n"
        "param1=value1&param2=value2";

    const char* deleteRequest =
        "DELETE /resource/123 HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: MyHttpClient\r\n"
        "\r\n";

    const char* encodedRequest =
        "GET /path/to/resource%20with%20spaces?param1=value%21%22 HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: MyHttpClient\r\n"
        "Accept: */*\r\n"
        "\r\n";

    const char* bodyRequest =
        "POST /path/to/resource%20with%20spaces?param1=value%21%22 HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: MyHttpClient\r\n"
        "Accept: */*\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "\r\n"
        "key1=value1&key2=value2&key3=value3\n"
        "Additional line in the body\n"
        "Yet another line in the body\n";



    requestParser parser(bodyRequest, strlen(bodyRequest));

}