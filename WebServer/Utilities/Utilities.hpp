// Utility.hpp (Header file for utility functions)
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

namespace utility {
    void    stringTrim(std::string &str, const char *charset);
    long    getCurrentTimeinSec(void);
    std::string getMIMEType(std::string path);
    // Other utility functions if needed
}

#endif
