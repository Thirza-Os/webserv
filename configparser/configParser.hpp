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

    std::stack<char>            _braceStack;

public:
    configParser(std::string path);
    ~configParser();
    configParser(const configParser &src);
    configParser &operator=(const configParser &src);

    std::vector<serverConfig>           getServerConfig() const;
    void                                read_and_parse_config();
    void                                process_line(std::string &line);
    void                                validate_braces(std::string &line);
    void                                del_comments(std::string &line);
    bool                                file_exists(const char *path);
};

#endif