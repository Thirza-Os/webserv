#include "web_server/configparser/serverConfig.hpp"
#include "web_server/configparser/configParser.hpp"
#include "web_server/serverManager.hpp"
#include "web_server/server.hpp"

#include <iostream>
#include <csignal>

int main(int argc, char **argv){
    if (argc != 2) {
        std::cout << "Please provide a config file" << std::endl;
        return (0);
    }
    signal(SIGPIPE, SIG_IGN);
    try{
        configParser parsed(argv[1]);
        parsed.read_and_parse_config();
        std::vector<serverConfig> servers = parsed.get_serverconfig();
        // for (std::vector<serverConfig>::iterator it = servers.begin(); it != servers.end() ;it++) {
        //     std::cout << "Host: " << it->getHost() << " Port: " << it->getPort() << std::endl;
        // }
        serverManager manager(servers);
    }
    catch(const ConfigParserException& e) {
        std::cerr << "ConfigParser Error: " << e.what() << std::endl;
    }
}
