#include "configParser.hpp"
#include "serverConfig.hpp"
#include "../Utilities/Utilities.hpp"
#include <iostream>
#include <vector>
#include <bitset>


int main(int argc, char *argv[]) {
    std::cout << argc << std::endl;
    
    configParser parser(argv[1]);
    parser.read_and_parse_config();
    std::vector<serverConfig> servers = parser.get_serverconfig();

    for (size_t i = 0; i < servers.size(); i++) {
        const serverConfig& server = servers[i];
        std::cout << "Server Listen: " << server.get_port() << std::endl;
        std::cout << "Server Host: " << server.get_host() << std::endl;
        std::cout << "Server Name: " << server.get_servername() << std::endl;
        std::cout << "Server Maxsize: " << server.get_maxsize() << std::endl;
        std::cout << "Root Directory: " << server.get_rootdirectory() << std::endl;

        std::map<int, std::string> errorPages = server.get_errorpages();
        for (std::map<int, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
            std::cout << "Error Page Key: " << it->first << ", Error Page Value: " << it->second << std::endl;
        }
        
        std::cout << "locations: " << server.get_locations().size() << std::endl;
        const std::vector<Location>& locations = server.get_locations();
        for (size_t j = 0; j < locations.size(); j++) {
            const Location& location = locations[j];
            std::cout << "Location Path: " << location.path << std::endl;
            std::cout << "Location Root: " << location.root << std::endl;
            std::cout << "Location Methods: " << location.methods << std::endl;
            std::cout << "Location Autoindex: " << (location.autoindex ? "true" : "false") << std::endl;
            std::cout << "Location Index: " << location.index << std::endl;
            std::cout << "Location Return Path: " << location.returnPath << std::endl;
            std::cout << "CGI: " << std::endl;
            for (std::map<std::string, std::string>::const_iterator it = location.cgiExtensions.begin(); it != location.cgiExtensions.end(); ++it) {
                std::cout << "Extension: " << it->first << ", Program: " << it->second << std::endl;
            }

            std::cout << "END OF LOCATION BLOCK" << std::endl;
        }


        std::cout << "END OF SERVER" << std::endl << std::endl;
    }
}