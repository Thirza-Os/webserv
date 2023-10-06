#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include "serverConfig.hpp"

// Call this configparser in a try/catch block. It will throw an error if something goes wrong

#include <string>
#include <stack>
#include <string>

class ConfigParserException : public std::runtime_error {
public:
    ConfigParserException(const std::string& message) : std::runtime_error(message) {}
};

class configParser
{
private:
    std::string                 _path;
	std::vector<serverConfig>	_servers;
    std::vector<std::string>    _serverBlocks;   

    std::stack<char>            _braceStack;

public:
    configParser(std::string path);
    ~configParser();
    configParser(const configParser &src);
    configParser &operator=(const configParser &src);

    std::vector<serverConfig>           getServerConfig() const;
    void                                read_and_parse_config();
    void                                process_server_block(std::string &serverBlock);
    void                                validate_braces(std::string &line);
    void                                del_comments(std::string &line);
    bool                                file_exists(const char *path);
    void                                process_listen(std::string &line, serverConfig &server);
    void                                process_host(std::string &line, serverConfig &server);
    void                                process_line(std::string &line, serverConfig &server);
};

#endif