#include "configParser.hpp"
#include "serverConfig.hpp"
#include "utilities/utilities.hpp"

#include <string>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

// TODO : handle missing {}

configParser::configParser(std::string path) {
    const char* charPath = path.c_str();
    if (!file_exists(charPath)) {
        throw ConfigParserException("Configuration file not found");
    }
    this->_path = path;
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

std::vector<serverConfig> configParser::getServerConfig() const {
    return _servers;
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

void    configParser::process_server_block(std::string &serverBlock) {

    serverConfig server; // Create a serverConfig object for this server block

    std::stringstream sb(serverBlock); 
    bool contentRead = false;                          // check if file is not empty
    std::string line;

    while (std::getline(sb, line)) {
        utility::stringTrim(line, " \t\n\r\f\v");
        del_comments(line);
        if (line.empty())
            continue;                                   // skip empty lines
        validate_braces(line);
        process_line(line, server);
    }
    if (!this->_braceStack.empty()) {
        throw ConfigParserException("Mismatched opening brace '{'");
    }
    this->_servers.push_back(server);
}

void configParser::process_listen(std::string &line, serverConfig &server) {
    size_t pos = line.find("listen");
    if (pos != std::string::npos) {
        std::string trimmedLine = line.substr(pos + 6);
        utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
        try {
            int intValue = std::stoi(trimmedLine);
            server.setPort(intValue);
        } catch (const std::invalid_argument& e) {
            throw ConfigParserException("Invalid port number");
        }
    }
}

void configParser::process_host(std::string &line, serverConfig &server) {
    size_t pos = line.find("host");
    if (pos != std::string::npos) {
        std::string trimmedLine = line.substr(pos + 4);
        utility::stringTrim(trimmedLine, " \t\n\r\f\v;");
        in_addr_t ipAddress = inet_addr(trimmedLine.c_str());

        if (ipAddress == INADDR_NONE)
            throw ConfigParserException("Invalid IP address");
        else
            server.setHost(ipAddress);
    }
}

void configParser::process_line(std::string &line, serverConfig &server) {
    process_listen(line, server);
    process_host(line, server);
}

