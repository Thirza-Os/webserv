#include "Utilities.hpp"

#include <iostream>
#include <map>
#include <sys/time.h>


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

    long	getCurrentTimeinSec(void) {
        struct timeval	current_time;

        gettimeofday(&current_time, NULL);
        return (current_time.tv_sec);
    }

    std::string getMIMEType(std::string filepath) {
        const static std::map<std::string, std::string> lookup {
            {"html", "text/html"},
            {"htm", "text/html"},
            {"css", "text/css"},
            {"js", "text/javascript"},
            {"txt", "text/plain"},
            {"ico", "image/x-icon"},
            {"jpg", "image/jpeg"},
            {"jpeg", "image/jpeg"},
            {"gif", "image/gif"},
            {"png", "image/png"},
            {"bmp", "image/bmp"},
            {"webm", "video/webm"},
            {"mp4", "video/mp4"},
            {"mp3", "audio/mp3"},
            {"wav", "audio/wav"},
            {"zip", "application/zip"},
            {"pdf", "application/pdf"},
            {"xml", "application/xml"},
        };
        std::size_t pos = filepath.find_last_of(".");
        if (pos == std::string::npos) {
            std::cout << "No extension!" << std::endl;
            return ("text/plain");
        }
        std::string extension = filepath.substr(pos + 1);
        std::cout << "extension is " << extension << std::endl;
        if (lookup.count(extension)) {
            return (lookup.at(extension));
        }
        return ("text/plain");
    }
};
