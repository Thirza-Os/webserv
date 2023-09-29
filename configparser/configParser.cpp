#include "configParser.hpp"
#include "serverConfig.hpp"
#include "utilities/utilities.hpp"

#include <string>
#include <iostream>
#include <sys/stat.h>
#include <fstream>

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

        bool contentRead = false;                          // check if file is not empty
        std::string line;

        while (std::getline(configFile, line)) {
            utility::stringTrim(line, " \t\n\r\f\v");
            del_comments(line);
            if (!line.empty()) {
                contentRead = true;
            } else
                continue;                                   // skip empty lines
            validate_braces(line);
            process_line(line);
        }
        if (!contentRead) {
            throw ConfigParserException("Configuration file is empty");
        }
        if (!this->_braceStack.empty()) {
            throw ConfigParserException("Mismatched opening brace '{'");
        }
    }
    else {
        throw ConfigParserException("Failed to open configuration file");
    }
    configFile.close();
}

void    configParser::process_line(std::string &line) {
    std::cout << line << std::endl;
}
