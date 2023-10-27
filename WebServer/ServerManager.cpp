#include "ServerManager.hpp"
#include "Utilities/Utilities.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <algorithm>

const int BUFFER_SIZE = 30720;

ServerManager::ServerManager(std::vector<ServerConfig> configs): _configs(configs)
{
    for (std::vector<ServerConfig>::iterator it = _configs.begin(); it != _configs.end() ;it++) {
        std::cout << "server: " << it->get_servername() << ", root: " << it->get_rootdirectory() << std::endl;
        _servers.push_back(Server(*it));
    }
    start_listen();
}

ServerManager::~ServerManager(){}

void    ServerManager::exit_error(const std::string &str)
{
    std::cerr << "Error - " << str << std::endl;
    exit(1);
}

void    ServerManager::log(const std::string &message)
{
    std::cout << message << std::endl;
}

void ServerManager::start_listen()
{
    std::vector<int> listeners;
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end() ;it++) {
        if (listen(it->get_socket(), 20) < 0)
        {
            exit_error("Socket listen failed");
        }
        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " 
            << inet_ntoa(it->get_sock_addr().sin_addr) 
            << " PORT: " << ntohs(it->get_sock_addr().sin_port) 
            << " ***\n\n";
        log(ss.str());
        struct pollfd listener;

        listener.fd = it->get_socket();
        listener.events = POLLIN;
        listeners.push_back(it->get_socket());
        _pollfds.push_back(listener);
    }
    // main webserv loop starts here, the program should never exit this loop
    while (true)
    {
        log("====== Waiting for a new event ======\n\n\n");
        if (poll(&_pollfds[0], _pollfds.size(), 10000) == -1) {
            log("Error returned from poll()\n");
        }
        for (std::vector<struct pollfd>::iterator it = _pollfds.begin(); it < _pollfds.end(); it++) {
            if (it->revents & POLLIN + POLLHUP) {
                if (std::find(listeners.begin(), listeners.end(), it->fd) != listeners.end()) {
                    accept_connection(it->fd);
                    break ;
                }
                else if (this->_cgiIndex.count(it->fd)) {
                    //read cgi response
                    log("Reading cgi response");
                    char buffer[BUFFER_SIZE] = {0};
                    int bytesReceived = read(it->fd, buffer, BUFFER_SIZE);
                    if (bytesReceived < 0)
                    {
                        log("Failed to read bytes from pipe connection");
                        break ;
                    }
                    //add bytes read to string and add it to cgi response index
                    std::string cgiResponse = buffer;
                    if (_cgiResponseIndex.count(this->_cgiIndex.at(it->fd))) {
                        this->_cgiResponseIndex.at(this->_cgiIndex.at(it->fd)).append(cgiResponse);
                    }
                    else {
                        this->_cgiResponseIndex.insert({this->_cgiIndex.at(it->fd), cgiResponse});
                    }
                    //find the pollfd of cgiIndex.at(it->fd) and set events to POLLOUT
                    if (bytesReceived == 0) {
                        for (std::vector<struct pollfd>::iterator iter = _pollfds.begin(); iter < _pollfds.end(); iter++) {
                            if (iter->fd == this->_cgiIndex.at(it->fd)) {
                                iter->events = POLLOUT;
                            }
                        }
                        //close the pipe end
                        close(it->fd);
                        it = _pollfds.erase(it);
                        this->_cgiIndex.erase(it->fd);
                    }
                    break ;
                }
                else {
                    char buffer[BUFFER_SIZE] = {0};
                    int bytesReceived = read(it->fd, buffer, BUFFER_SIZE);
                    if (bytesReceived < 0)
                    {
                        log("Failed to read bytes from client socket, connection closed");
                        close(it->fd);
                        it = _pollfds.erase(it);
                        break ;
                    }
                    if (bytesReceived == 0) {
                        log("Client closed the connection");
                        close(it->fd);
                        it = _pollfds.erase(it);
                        break ;
                    }
                    RequestParser request(buffer);
                    _requests.insert({it->fd, request});
                    if (_timeOutIndex.count(it->fd)) {
                        this->_timeOutIndex.at(it->fd) = utility::getCurrentTimeinSec();
                    }

                    std::ostringstream ss;
                    ss << "------ Received Request from client ------\n\n";
                    log(ss.str());
                    it->events = POLLOUT;
                    break ;
                }
            }
            else if (it->revents & POLLOUT) {
                if (send_response(it->fd)) {
                    close(it->fd);
                    it = _pollfds.erase(it);
                }
                else {
                    for (std::map<int, int>::iterator iter = _cgiIndex.begin(); iter != _cgiIndex.end() ;iter++) {
                        if (iter->second == it->fd) {
                            it->events = 0;
                        }
                    }
                    if (it->events) {
                        it->events = POLLIN;
                    }
                }
                _requests.erase(it->fd);
                break ;
            }
        }
        check_timeout();
    }

}

void ServerManager::accept_connection(int incoming)
{
    Server correctServer;
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end() ;it++) {
        if (it->get_socket() == incoming)
            correctServer = *it;
    }
    sockaddr_in socketAddr = correctServer.get_sock_addr();
    unsigned int socketAddrLen = sizeof(socketAddr);
    int new_socket = accept4(correctServer.get_socket(), (sockaddr *)&socketAddr,
                        &socketAddrLen, SOCK_NONBLOCK);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss <<
        "Server failed to accept incoming connection from ADDRESS: "
        << inet_ntoa(socketAddr.sin_addr) << "; PORT: "
        << ntohs(socketAddr.sin_port);
        log(ss.str());
        return ;
    }
    struct pollfd new_socket_fd;
    new_socket_fd.fd = new_socket;
    new_socket_fd.events = POLLIN;
    this->_pollfds.push_back(new_socket_fd);
    this->_requestServerIndex.insert({new_socket, correctServer});
    this->_timeOutIndex.insert({new_socket, utility::getCurrentTimeinSec()});
    std::ostringstream ss;
    ss << "------ New connection established ------\n\n";
    log(ss.str());
}

//return 1 to close the connection after, 0 to keep it alive
int ServerManager::send_response(int socket_fd)
{
    bool errorOccurred = false;
    unsigned long bytesSent;
    std::string serverMessage;
    if (this->_cgiResponseIndex.count(socket_fd)) {
        //send cgi response instead of normal responsebuilder stuff
        serverMessage = this->_cgiResponseIndex.at(socket_fd);
    }
    else if (_requests.count(socket_fd)) {
        ResponseBuilder response(_requests.at(socket_fd), _requestServerIndex.at(socket_fd).get_config());
        if (response.get_cgiPipeFd()) {
            //we need to read from the pipe and send that instead
            struct pollfd cgi_fd;
            cgi_fd.fd = response.get_cgiPipeFd();
            cgi_fd.events = POLLIN;
            this->_pollfds.push_back(cgi_fd);
            //map the socket_fd to send the read output to the cgi_fd
            _cgiIndex.insert({cgi_fd.fd, socket_fd});
            _requests.erase(socket_fd);
            return (0);
        }
        serverMessage = response.get_response();
        std::cout << response.get_header() << std::endl;
    }
    bytesSent = write(socket_fd, serverMessage.c_str(), serverMessage.size());
    if (bytesSent == serverMessage.size())
    {
        log("------ Server Response sent to client ------\n\n");
    }
    else
    {
        log("Error sending response to client");
        errorOccurred = true;
    }
    if (bytesSent == 0) {
        log("Error sending response to client");
        errorOccurred = true;
    }
    if (this->_cgiResponseIndex.count(socket_fd)) {
        std::cout << "cgi response sent, closing connection" << std::endl;
        this->_cgiResponseIndex.erase(socket_fd);
        return (1);
    }
    if (_requests.count(socket_fd)) {
        if (_requests.at(socket_fd).find_header("Connection") == " close" || errorOccurred) {
            std::cout << "Closing connection" << std::endl;
            _requestServerIndex.erase(socket_fd);
            return (1);
        }
    }
    std::cout << "Keeping connection alive" << std::endl;
    return (0);
}

//Close connections that are idle for 30 seconds or more
void ServerManager::check_timeout(void)
{
    long current_time = utility::getCurrentTimeinSec();
    for(std::map<int, long>::iterator it = this->_timeOutIndex.begin(); it != this->_timeOutIndex.end(); it++) {
        long time_elapsed = current_time - it->second;
        if (time_elapsed >= 30) {
            std::cout << "Connection timed out after " << time_elapsed << "seconds" << std::endl;
            close(it->first);
            for (std::vector<struct pollfd>::iterator it2 = _pollfds.begin(); it2 != _pollfds.end() ; it2++) {
                if (it2->fd == it->first) {
                    it2 = _pollfds.erase(it2);
                    break;
                }
            }
            _requestServerIndex.erase(it->first);
            it = _timeOutIndex.erase(it);
            check_timeout();
            break ;
        }
    }
}
