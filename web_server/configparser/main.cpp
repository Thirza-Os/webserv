#include "configParser.hpp"
#include "serverConfig.hpp"
#include "../utilities/utilities.hpp"
#include <iostream>
#include <vector>
#include <bitset>

int main(int argc, char *argv[])
{
    std::cout << argc << std::endl;
    
    configParser parser(argv[1]);
    parser.read_and_parse_config();
    std::vector<serverConfig> servers = parser.get_serverconfig();

    parser.read_and_parse_config();
        for (const serverConfig& server : servers) {
        // std::cout << "Server Listen: " << server.get_port() << std::endl;
        // std::cout << "Server Host: " << server.get_host() << std::endl;
        // std::cout << "Server Name: " << server.get_servername() << std::endl;
        // std::cout << "Server Maxsize: " << server.get_maxsize() << std::endl;
        // std::cout << "Root Directory: " << server.get_rootdirectory() << std::endl;

        // std::map<int, std::string>  errorPages = server.get_errorpages();
        // for (const auto& pair : errorPages) {
        //     std::cout << "Error Page Key: " << pair.first << ", Error Page Value: " << pair.second << std::endl;
        // }
            
        std::cout << "cgi:" << std::endl;
        std::map<std::string, std::string>  cgi = server.get_cgiExtensions();
        for (const auto& pair : cgi) {
            std::cout << "cgi extension: " << pair.first << ", cgi program: " << pair.second << std::endl;
        }
        
        // std::cout << "locations:" << std::endl;
        // const std::vector<Location>& locations = server.get_locations();
        // for (const Location& location : locations) {
        //     std::cout << "Location Path: " << location.path << std::endl;
        //     std::cout << "Location Root: " << location.root << std::endl;
        //     std::cout << "Location Methods: " << location.methods << std::endl;
        //     std::cout << "Location Autoindex: " << (location.autoindex ? "true" : "false") << std::endl;
        //     std::cout << "Location Index: " << location.index << std::endl;
        //     std::cout << "Location Return Path: " << location.returnPath << std::endl;
        // }

        std::cout << "END OF SERVER" << std::endl << std::endl;

    }
} 