#include "CgiHandler.hpp"
#include "../ConfigParser/ServerConfig.hpp"
#include <map>
#include <vector>
#include <string>
#include <iostream>

int main(void)
{
    Location loc;


    loc.path = "/cgiRoute";
    loc.root = "../www/penguinserv/";
    loc.methods = std::bitset<3>("011");  // For example, allowing GET and HEAD methods
    loc.autoindex = false;  // You can set this to true or false as needed
    loc.index = "index.html";
    loc.returnPath = "/another/path";
    loc.cgiExtensions[".php"] = "./cgi/php-cgi";



    CgiHandler test(loc);

    std::map<std::string, std::string> testenv;

    testenv = test.get_environment();
    for (std::map<std::string, std::string>::const_iterator it = testenv.begin(); it != testenv.end(); ++it) {
        std::cout << "Environment Key: " << it->first << ", Environment Value: " << it->second << std::endl;
        }

    return 0;
}