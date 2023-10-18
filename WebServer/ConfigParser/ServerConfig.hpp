#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <netinet/in.h>
#include <arpa/inet.h>

struct Location {
    std::string                         path;
    std::string                         root;
    std::bitset<3>                      methods;
    bool                                autoindex;
    std::string                         index;
    std::string                         returnPath;
    std::map<std::string, std::string>  cgiExtensions;

    Location() : autoindex(false){};
};

class ServerConfig
{
private:
    int                                 _port;
    in_addr_t                           _host;
    std::string                         _serverName;
    size_t                              _maxSize;
    std::map<int, std::string>          _errorPages;
    std::string                         _rootDirectory;
    std::vector<std::string>            _index;
    std::map<std::string, std::string>  _cgiExtensions;

    //struct          sockaddr_in  _socketAddr;

public:
    ServerConfig();
    ~ServerConfig();
    ServerConfig(const ServerConfig &src);
    ServerConfig &operator=(const ServerConfig &src);

    std::vector<Location> 		_locations;

    void set_port(const int port);
    void set_host(in_addr_t &host);
    void set_servername(const std::string serverName);
    void set_maxsize(size_t maxSize);
    void set_error_pages(int code, std::string &errorPage);
    void set_rootdirectory(const std::string rootDirectory);
    void set_index(std::vector<std::string> &index);
    void set_location(const Location& location);
    void set_cgiExtensions(const std::string ext, const std::string program);
    //void setSocketAddress(const struct sockaddr_in& socketAddr);

    int                                     get_port() const;
    in_addr_t                               get_host() const;
    std::string                             get_servername() const;
    size_t                                  get_maxsize() const;
    std::map<int, std::string>              get_errorpages() const;
    std::string                             get_rootdirectory() const;
    std::vector<Location>                   get_locations() const;
    std::map<std::string, std::string>      get_cgiExtensions() const;
};

#endif

// if (_methods[0]) {
//     // PUT method is supported
// }
// if (_methods[1]) {
//     // POST method is supported
// }
// if (_methods[2]) {
//     // GET method is supported
// }