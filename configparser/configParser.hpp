#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include <string>

class configParser
{
private:
    std::string     _path;

public:
    configParser(std::string path);
    ~configParser();
    configParser(const configParser &src);
    configParser &operator=(const configParser &src);
};

#endif