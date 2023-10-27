// Utility.hpp (Header file for utility functions)
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include "../HttpParser/RequestParser.hpp"

namespace utility {
    void    stringTrim(std::string &str, const char *charset);
    long    getCurrentTimeinSec(void);
	void	upload_file(RequestParser *post_request);
    // Other utility functions if needed
}

#endif
