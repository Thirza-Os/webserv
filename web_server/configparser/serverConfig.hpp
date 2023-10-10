#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    int                         _port;
    in_addr_t                   _host;
    std::string                 _serverName;
    std::string                 _methods;
    size_t                      _maxSize;
    std::map<int, std::string>  _errorPages;
    std::string                 _rootDirectory;
    std::vector<std::string>    _index;
    std::vector<Location> 		_locations;

    //struct          sockaddr_in  _socketAddr;

public:
    serverConfig(/* args */);
    ~serverConfig();
    serverConfig(const serverConfig &src);
    serverConfig &operator=(const serverConfig &src);

    void set_port(const int port);
    void set_host(in_addr_t &host);
    void set_servername(const std::string serverName);
    void set_methods(std::string &methodString);
    void set_maxsize(size_t maxSize);
    void set_error_pages(int code, std::string &errorPage);
    void set_rootdirectory(const std::string rootDirectory);
    void set_index(std::vector<std::string> &index);
    //void setLocations(const locations);
    //void setSocketAddress(const struct sockaddr_in& socketAddr);

    int         get_port() const;
    in_addr_t   get_host() const;
    std::string get_rootdirectory() const;
};

#endif