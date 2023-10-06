#include "web_server/configparser/serverConfig.hpp"
#include "web_server/configparser/configParser.hpp"
#include "web_server/tcpServer.hpp"

#include <iostream>

int main(int argc, char **argv){
    if (argc != 2) {
        std::cout << "Please provide a config file" << std::endl;
        return (0);
    }
    try{
        configParser parsed(argv[1]);
        parsed.read_and_parse_config();
        std::vector<serverConfig> servers = parsed.getServerConfig();

        // std::cout << "Server Name: " << server.getserverName() << std::endl;
        for (std::vector<serverConfig>::iterator it = servers.begin(); it != servers.end() ;it++) {
            tcpServer tcpserver = tcpServer(*it);
        }
    }
    catch(const ConfigParserException& e) {
        std::cerr << "ConfigParser Error: " << e.what() << std::endl;
    }
}
