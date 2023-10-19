// Put this in  the configparser file to test

#include "../ConfigParser/ConfigParser.hpp"
#include "../ConfigParser/ServerConfig.hpp"
#include "../Utilities/Utilities.hpp"
#include "/Users/thirza/Documents/Codam/webserv/WebServer/CgiHandler/CgiHandler.hpp"
#include "/Users/thirza/Documents/Codam/webserv/WebServer/HttpParser/RequestParser.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <bitset>


int main(int argc, char *argv[]) {
    std::cout << argc << std::endl;
    
    const char* getRequest =
        "GET /path/to/resource HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: MyHttpClient\r\n"
        "Accept: */*\r\n"
        "\r\n";
        std::string requestString(getRequest); // This converts the const char* to a std::string

    RequestParser request(requestString);
    ConfigParser parser(argv[1]);
    parser.read_and_parse_config();
    std::vector<ServerConfig> servers = parser.get_serverconfig();

    // for (size_t i = 0; i < servers.size(); i++) {
    //     const ServerConfig& server = servers[i];
    //     std::cout << "Server Listen: " << server.get_port() << std::endl;
    //     std::cout << "Server Host: " << server.get_host() << std::endl;
    //     std::cout << "Server Name: " << server.get_servername() << std::endl;
    //     std::cout << "Server Maxsize: " << server.get_maxsize() << std::endl;
    //     std::cout << "Root Directory: " << server.get_rootdirectory() << std::endl;

    //     std::map<int, std::string> errorPages = server.get_errorpages();
    //     for (std::map<int, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
    //         std::cout << "Error Page Key: " << it->first << ", Error Page Value: " << it->second << std::endl;
    //     }
    //     std::cout << std::endl;
    //     std::cout << "locations: " << server.get_locations().size() << std::endl;
    //     const std::vector<Location>& locations = server.get_locations();
    //     for (size_t j = 0; j < locations.size(); j++) {
    //         const Location& location = locations[j];
    //         std::cout << "Location Path: " << location.path << std::endl;
    //         std::cout << "Location Root: " << location.root << std::endl;
    //         std::cout << "Location Methods: " << location.methods << std::endl;
    //         std::cout << "Location Autoindex: " << (location.autoindex ? "true" : "false") << std::endl;
    //         std::cout << "Location Index: " << location.index << std::endl;
    //         std::cout << "Location Return Path: " << location.returnPath << std::endl;
    //         std::cout << "CGI: " << std::endl;
    //         for (std::map<std::string, std::string>::const_iterator it = location.cgiExtensions.begin(); it != location.cgiExtensions.end(); ++it) {
    //             std::cout << "Extension: " << it->first << ", Program: " << it->second << std::endl;
    //         }

    //         std::cout << "END OF LOCATION BLOCK" << std::endl;
    //     }


    //     std::cout << "END OF SERVER" << std::endl << std::endl;
    // }
    const std::vector<Location>& locations = servers[0].get_locations();
    Location loc = locations[0];
    CgiHandler test(loc, request);

}


// #include "CgiHandler.hpp"
// #include "../ConfigParser/ServerConfig.hpp"
// #include <map>
// #include <vector>
// #include <string>
// #include <iostream>

// int main(void)
// {
//     Location loc;


//     loc.path = "/cgiRoute";
//     loc.root = "../www/penguinserv";
//     loc.methods = std::bitset<3>("011");  // For example, allowing GET and HEAD methods
//     loc.autoindex = false;  // You can set this to true or false as needed
//     loc.index = "index.html";
//     loc.returnPath = "/another/path";
//     loc.cgiExtensions[".php"] = "./cgi/php-cgi";



//     CgiHandler test(loc);

//     std::map<std::string, std::string> testenv;

//     testenv = test.get_environment();
//     for (std::map<std::string, std::string>::const_iterator it = testenv.begin(); it != testenv.end(); ++it) {
//         std::cout << "Environment Key: " << it->first << ", Environment Value: " << it->second << std::endl;
//         }

//     return 0;
// }