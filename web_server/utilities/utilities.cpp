#include "Utilities.hpp"


namespace utility {
void    stringTrim(std::string &str, const char *charset) {

    size_t firstNonSpace = str.find_first_not_of(charset);

    if (firstNonSpace == std::string::npos) {
        str.clear();
    } else {
        str.erase(0, firstNonSpace);
        size_t lastNonSpace = str.find_last_not_of(charset);
        if (lastNonSpace != std::string::npos) {
            str.erase(lastNonSpace + 1);
            }
        }
    }
};
