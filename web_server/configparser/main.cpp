#include "configParser.hpp"
#include "serverConfig.hpp"
#include "../utilities/utilities.hpp"
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    std::cout << argc << std::endl;
    
    configParser parser(argv[1]);
    parser.read_and_parse_config();
    std::vector<serverConfig> servers = parser.get_serverconfig();

    parser.read_and_parse_config();
        for (const serverConfig& server : servers) {
        std::cout << "Server Listen: " << server.get_port() << std::endl;
        std::cout << "Server Host: " << server.get_host() << std::endl;
    }
} 