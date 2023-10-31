#include "Utilities.hpp"

#include <iostream>
#include <map>
#include <sys/time.h>
#include <sstream>
#include <fstream>
#include <iostream>

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
	std::string getContentInfo(std::string header, std::string info)
	{
		std::stringstream ss(header);
		std::string line;
		while (getline(ss, line, ' '))
		{
			if (line.find(info) != std::string::npos){
				break;
			}
		}
		line.erase(0, info.length());
		return (line);
	
	}

	void upload_file(RequestParser *post_request)
	{
		
		std::vector<char> body = post_request->get_body();
		std::vector<char>::iterator it = body.begin();
		std::string filename;
		std::string boundary;
		int newlines_found = 0;
		int i = 0;
		int length;

		//skip the first few lines because they are content-type, disposition and boundary.
		//only do this if 
		if (!post_request->get_content_disposition().empty())
		{
			while (it != body.end())
			{
				if (*it == '\n')
					newlines_found++;
				it++;
				i++;	
				if (newlines_found == 6)
					break;
			}
			//get the filename and boundary
			filename = getContentInfo(post_request->get_content_disposition(), "filename=");
			boundary = "--" + getContentInfo(post_request->get_content_type(), "boundary=") + "--";

			filename.erase(0, 1);
			filename.erase(filename.size() - 1);
			//length = body.size() - boundary.size() - 4;
			length = post_request->get_content_length() - boundary.size();
		}
		else {
			filename = "unknown_file";
			length = post_request->get_content_length();
			advance(it, 4);
		}

		std::ofstream newfile("WebServer/Uploaded_files/" + filename, std::ios::out | std::ios::binary);
		//write all bytes to file except final boundary
		if (newfile.is_open()) {
			while (i < length)
			{
				i++;
				newfile << *it;
				it++;
			}
			newfile.close();
		}//else return error?
	}
};
