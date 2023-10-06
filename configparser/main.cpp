#include "serverConfig.hpp"
#include "configParser.hpp"
#include "serverConfig.hpp"

#include <iostream>

int main(int argc, char **argv){
    try{
    configParser parsed(argv[1]);
    parsed.read_and_parse_config();
    std::vector<serverConfig> server = parsed.getServerConfig();

    // std::cout << "Server Name: " << server.getserverName() << std::endl;
    }
    catch(const ConfigParserException& e) {
        std::cerr << "ConfigParser Error: " << e.what() << std::endl;
    }
}