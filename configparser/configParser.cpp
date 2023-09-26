#include "configParser.hpp"

#include <string>

configParser::configParser(std::string path): _path(path) {
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