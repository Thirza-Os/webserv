#include "configParser.hpp"
#include "serverConfig.hpp"
#include "../utilities/utilities.hpp"

#include <string>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

// TODO : handle missing {}
// MAYBE TODO: split lines on ; ??
// CHECK when to throw an exception and when to log?
// perhaps add multiple error_page in one line?
// Add regular expressions for location blocks?? ~ ^/images/ 
// Problem if location is in the name of the url?


configParser::configParser(std::string path) {

    const char* charPath = path.c_str();
    if (!file_exists(charPath)) {
        throw ConfigParserException("Configuration file not found");
    }
    this->_path = path;

    std::cout << "Config parser constructed with: " << this -> _path << "as _path" << std::endl;
}

configParser::~configParser() {
}

configParser::configParser(const configParser &src) {
    *this = src;
}

configParser &configParser::operator=(const configParser &src) {
    if (this != &src) {
        this->_path = src._path;
    }
    return *this;
}

void    configParser::validate_braces(std::string &line) {
        if (line.find('{') != std::string::npos) {
            this->_braceStack.push('{');
    }
    else if (line.find('}') != std::string::npos) {
        if (_braceStack.empty()) {
            throw ConfigParserException("Mismatched closing brace '}' ");
        } else {
            _braceStack.pop();
        }
    }
}

void    configParser::del_comments(std::string &line) {

    size_t hash = line.find('#');

    if (hash != std::string::npos) {
        line.erase(hash);
    }
}

bool configParser::file_exists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

bool configParser::index_exists(const std::string& rootDirectory, const std::string& indexFilename) {
    std::string fullPath = rootDirectory + indexFilename;
    std::ifstream file(fullPath.c_str());
    return file.good();
}

// things with braces: server, location

void configParser::process_listen(std::string &line, serverConfig &server) {
    size_t pos = line.find("listen");
    std::string trimmedLine = line.substr(pos + 7);
    utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
    size_t pos_host = trimmedLine.find(":");

    if (pos_host != std::string::npos)
    {
        std::string host = trimmedLine.substr(0, pos_host);
        in_addr_t ipAddress = inet_addr(host.c_str());
        if (ipAddress == INADDR_NONE)
            throw ConfigParserException("Invalid IP address");
        else
        server.set_host(ipAddress);
        trimmedLine = trimmedLine.substr(pos_host + 1);
    }
    try {
        int intValue = std::stoi(trimmedLine);
        server.set_port(intValue);
    } catch (const std::invalid_argument& e) {
        throw ConfigParserException("Invalid port number");
    }
}

std::bitset<3>  configParser::process_methods(std::string &line)
{
    std::bitset<3> methods;

    if (line.find("GET") != std::string::npos) {
    methods.set(0, true); // Set the first bit for GET
    }
    if (line.find("POST") != std::string::npos) {
       methods.set(1, true); // Set the second bit for POST
    }
    if (line.find("PUT") != std::string::npos) {
        methods.set(2, true); // Set the third bit for PUT
    }
    return (methods);
}

void configParser::process_servername(std::string &line, serverConfig &server) {
    size_t pos = line.find("server_name");
    if (pos != std::string::npos) {
        std::string trimmedLine = line.substr(pos + 12);
        utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
        server.set_servername(trimmedLine);
    }
}

void configParser::process_maxsize(std::string &line, serverConfig &server) {
    size_t pos = line.find("client_max_body_size");
    if (pos != std::string::npos) {
        std::string trimmedLine = line.substr(pos + 21);
        utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
    try {
        size_t sizeValue = std::stoull(trimmedLine);
        server.set_maxsize(sizeValue);
    } catch (const std::exception& e) {
        throw ConfigParserException("Invalid size");
    }}
}

void configParser::process_errorpages(std::string &line, serverConfig &server) {
    int         code;
    std::string errorPage;
    size_t pos = line.find("error_page");

    if (pos != std::string::npos) {
        std::string trimmedLine = line.substr(pos + 11);

        std::istringstream iss(trimmedLine);
        if (iss >> code >> errorPage) {
            server.set_error_pages(code, errorPage);
        } else
            throw ConfigParserException("Invalid error page");
    }
}

std::string configParser::process_rootdirectory(std::string &line) {
    size_t pos = line.find("root");
    std::string trimmedLine = line.substr(pos + 5);
    utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
    if (trimmedLine.empty() || !file_exists(trimmedLine.c_str())) {
        throw ConfigParserException("Root directory does not exist or is empty");
        return "";
    }
    return trimmedLine;
}


void configParser::process_index(std::string &line, serverConfig &server) {

    std::vector<std::string> index;
    size_t pos = line.find("index");
    if (pos != std::string::npos) {
        std::string trimmedLine = line.substr(pos + 6);
        utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
        std::istringstream iss(trimmedLine);
        std::string filename;

        while (std::getline(iss, filename, ' ')) {
        if (!index_exists(server.get_rootdirectory(), filename)) {
                throw ConfigParserException("Index directory does not exist");
            }
            index.push_back(filename);
        }
        server.set_index(index);
    }
}

void configParser::process_location(std::string first_line, std::stringstream& sb, serverConfig &server) {


    size_t      locationPos = first_line.find("location");
    Location    loc;
    std::string path = first_line.substr(locationPos + 9);
    utility::stringTrim(path, " \t\n\r\f\v");
    loc.path = path;

    std::string line;
    std::getline(sb, line);
    while (std::getline(sb, line)) {
        utility::stringTrim(line, " \t\n\r\f\v");
        del_comments(line);

        if (line.empty())
            continue;
        if (line.find("allow_methods") != std::string::npos)
            loc.methods = process_methods(line);
        else if (line.find("root") != std::string::npos)
            loc.root = process_rootdirectory(line);
        else if (line.find("autoindex") != std::string::npos)
        {
            size_t pos = line.find("autoindex");
            std::string trimmedLine = line.substr(pos + 9);
            utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
            if (trimmedLine == "off")
                loc.autoindex = true;
            else if (trimmedLine == "on")
                loc.autoindex = false; 
            else
                throw ConfigParserException("Route: Wrong autoindex input");
        }
        else if (line.find("index") != std::string::npos)
        {
            // std::string root = server.get_rootdirectory();
            // if (!loc.root.empty())
            //     root = loc.root;

            size_t pos = line.find("index");
            std::string trimmedLine = line.substr(pos + 6);
            utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
            // if (!index_exists(server.get_rootdirectory(), trimmedLine)) {
            //     throw ConfigParserException("Route: Index directory does not exist");
            // }
            loc.index = trimmedLine;
        }
        else if (line.find("return") != std::string::npos)
        {
            size_t pos = line.find("return");
            std::string trimmedLine = line.substr(pos + 7);
            utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
            loc.returnPath = trimmedLine;
        }
        else if (line == "}") {
            server.set_location(loc);
            _braceStack.pop();
            break; }
    }
}

void configParser::process_line(std::string &line, serverConfig &server) {
    if (line.find("listen") != std::string::npos)
        process_listen(line, server);
    else if (line.find("server_name") != std::string::npos)
        process_servername(line, server);
    else if (line.find("client_max_body_size") != std::string::npos)
         process_maxsize(line, server);
    else if (line.find("error_page") != std::string::npos)
        process_errorpages(line, server);
    else if (line.find("root") != std::string::npos)
        server.set_rootdirectory(process_rootdirectory(line));
    else if (line.find("index") != std::string::npos)
        process_index(line, server);
}


void    configParser::process_server_block(std::string &serverBlock) {

    serverConfig server; // Create a serverConfig object for this server block

    std::stringstream sb(serverBlock); 
    std::string line;

    while (std::getline(sb, line)) {
        utility::stringTrim(line, " \t\n\r\f\v");
        del_comments(line);
        if (line.empty())
            continue;                                   // skip empty lines
        validate_braces(line);
        if (line.find("location") == 0){
            process_location(line, sb, server);
        }
        else
            process_line(line, server);
    }
    if (!this->_braceStack.empty())
        throw ConfigParserException("Mismatched opening brace '{'");
    this->_servers.push_back(server);
}


void    configParser::read_and_parse_config() {

    std::ifstream configFile(this->_path);

    if (configFile.is_open()) {
        std::string configContents;
        std::string line;

        while (std::getline(configFile, line)) {
            configContents += line + "\n";
        }
        configFile.close();
        if (configContents.empty())
            throw ConfigParserException("Configuration file is empty");
        // Split the configuration string into server blocks
        size_t startPos = 0;
        size_t endPos = configContents.find("server {", startPos);

        while (endPos != std::string::npos) {
            size_t nextStartPos = configContents.find("server {", endPos + 1);
            if (nextStartPos == std::string::npos) {
                // If there are no more "server {" occurrences, use the end of the string
                nextStartPos = configContents.length();
            }
            
            // Extract the current server block
            this->_serverBlocks.push_back(configContents.substr(startPos, nextStartPos - startPos));
            
            // Update startPos and endPos for the next iteration
            startPos = nextStartPos;
            endPos = configContents.find("server {", startPos);
        }
        // Process each server block
        for (std::vector<std::string>::iterator it = _serverBlocks.begin(); it != _serverBlocks.end(); ++it) {
            std::string& serverBlock = *it;
            process_server_block(serverBlock);
        }
    }
    else {
        throw ConfigParserException("Failed to open configuration file");
    }
}

std::vector<serverConfig>   configParser::get_serverconfig(){
    return(this->_servers);
}