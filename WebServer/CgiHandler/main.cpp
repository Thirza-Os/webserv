#include "CgiHandler.hpp"
#include "../ConfigParser/serverConfig.hpp"
#include <map>
#include <vector>
#include <string>
#include <iostream>

int main(void)
{
    Location loc;

    #include "../ConfigParser/serverConfig.hpp"

    loc.path = "/path/to/resource";
    loc.root = "/path/to/root";
    loc.methods = std::bitset<3>("011");  // For example, allowing GET and HEAD methods
    loc.autoindex = false;  // You can set this to true or false as needed
    loc.index = "index.html";
    loc.returnPath = "/another/path";
    loc.cgiExtensions[".php"] = "./cgi/php-cgi";

    CgiHandler test(loc);

    return 0;
}