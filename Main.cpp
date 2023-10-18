#include "WebServer/ConfigParser/ServerConfig.hpp"
#include "WebServer/ConfigParser/ConfigParser.hpp"
#include "WebServer/ServerManager.hpp"
#include "WebServer/Server.hpp"

#include <iostream>
#include <csignal>

int main(int argc, char **argv){
    if (argc != 2) {
        std::cout << "Please provide a config file" << std::endl;
        return (0);
    }
    signal(SIGPIPE, SIG_IGN);
    try{
        ConfigParser parsed(argv[1]);
        parsed.read_and_parse_config();
        std::vector<ServerConfig> servers = parsed.get_ServerConfig();
        ServerManager manager(servers);
    }
    catch(const ConfigParserException& e) {
        std::cerr << "ConfigParser Error: " << e.what() << std::endl;
    }
}
