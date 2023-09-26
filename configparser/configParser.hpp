#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

class configParser
{
private:
    std::string     _path;

public:
    configParser(std::string path);
    ~configParser();
};

#endif