#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <netinet/in.h>

struct Location {
    std::string path;
    std::vector<std::string> allowedMethods;
    std::string index;
    //std::string cgiPath;           // CGI script path (if applicable)
    //std::vector<std::string> cgiExtensions;
    bool autoindex;
    std::string alias;
    std::string returnPath;
};

class serverConfig
{
private:
    std::vector<int>            _port;
    std::string                 _host;
    std::string                 _serverName;
    size_t                      _maxSize;
    std::vector<std::string>    _errorPages;
    std::string                 _rootDirectory;
    std::string                 _index;
    std::vector<Location> 		_locations;

    struct          sockaddr_in  _socketAddr;

public:
    serverConfig(/* args */);
    ~serverConfig();
    serverConfig(const serverConfig &src);
    serverConfig &operator=(const serverConfig &src);

    void setPort(const int port);
    void setHost(const std::string host);
    void setServerName(const std::string serverName);
    void setMaxSize(size_t maxSize);
    void setErrorPages(const std::string errorPage);
    void setRootDirectory(const std::string rootDirectory);
    void setIndex(const std::string index);
    //void setLocations(const locations);
    //void setSocketAddress(const struct sockaddr_in& socketAddr);
};

#endif