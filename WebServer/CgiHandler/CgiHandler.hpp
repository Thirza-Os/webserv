#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include "../ConfigParser/ServerConfig.hpp"
#include <string>
#include <map>

class CgiException : public std::runtime_error {
public:
    CgiException(const std::string& message) : std::runtime_error(message) {}
};

class CgiHandler
{
private:
    Location                                _variables;
    std::map<std::string, std::string>	    _environment;

    void                                    initialize_environment(Location loc);

public:
    CgiHandler(const Location &loc);
    ~CgiHandler();
    CgiHandler(const CgiHandler &src);
    CgiHandler &operator=(const CgiHandler &src);

    std::map<std::string, std::string>      get_environment() const;
};

//void    execute_script();

#endif