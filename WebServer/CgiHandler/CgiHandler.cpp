#include "CgiHandler.hpp"
#include <string>
#include <iostream>
#include <map>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>

CgiHandler::CgiHandler(Location const &loc, RequestParser const &httprequest) {
    std::cout << "constructor started for CGI handler " << std::endl;
    this->_runLoc = "/usr/bin/php";                                       // Set this to correct location of your php runner
    initialize_environment(loc, httprequest);
    execute_script();
    print_env();
}

CgiHandler::~CgiHandler() {
}

CgiHandler::CgiHandler(const CgiHandler &src) {
    *this = src;
}

CgiHandler &CgiHandler::operator=(const CgiHandler &src) {
    if (this != &src) {
        this->_environment = src._environment;
        this->_childEnvp = src._childEnvp;
        this->_argv = src._argv;
        this->_runLoc = src._runLoc;
    }
    return *this;
}

void    CgiHandler::initialize_environment(Location const &loc, RequestParser const &httprequest)
{
    std::map<std::string, std::string>::const_iterator it = loc.cgiExtensions.begin();
    // Only accessing first key & value, in case we want to expand to multiple we can make this a loop
    const std::string& firstKey = it->first;
    const std::string& firstValue = it->second;
   
   // set environment for execve
    this->_environment["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->_environment["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_environment["REDIRECT_STATUS"] = "200";              // Hardcoded this as well, means succesful response
    this->_environment["SERVER_PORT"] = "8081";                 // hardcoded this for now
    this->_environment["SERVER_SOFTWARE"] = "cool_server1.0";
    this->_environment["PATH_INFO"] = loc.root + loc.path + "/" + firstValue;      // says so in the subject
    this->_environment["SCRIPT_NAME"] = firstValue + firstKey;
    this->_environment["SCRIPT_FILENAME"] = loc.root + loc.path + "/" + firstValue + firstKey;
    this->_environment["REQUEST_METHOD"] = httprequest.get_method();
    this->_environment["REQUEST_URI"] = httprequest.get_uri();
    this->_environment["PATH"] = "WebServer/www/penguinserv/cgiRoute"; // Replace with the actual path
    
    // for post requests
    this->_environment["CONTENT_LENGTH"] = httprequest.get_content_length();
    this->_environment["CONTENT_TYPE"] = httprequest.get_content_type();

}

void    CgiHandler::execute_script(RequestParser const &httprequest) {
    char *postBuffer = nullptr;

    try {
        // Build the argv
        char* const argv[] = {
            const_cast<char*>(this->_environment["SCRIPT_NAME"].c_str()), // The path to the script
            const_cast<char*>(this->_environment["SCRIPT_FILENAME"].c_str()), // The path to the script
            nullptr
        };



        // for post requests: set buffer
        std::vector<char> bodyVector = httprequest.get_body();
        if (!bodyVector.empty()){
            postBuffer = new char[bodyVector.size() + 1];
            std::copy(bodyVector.begin(), bodyVector.end(), postBuffer);
            postBuffer[bodyVector.size()] = '\0';
        }



        // build pipes
        if (pipe2(pipe_in, O_NONBLOCK) < 0)
            perror("pipe in failed");
        if (pipe2(pipe_out, O_NONBLOCK) < 0)
            perror("pipe out failed");
        // start fork to process in a child
        pid_t childPid = fork();
        if (childPid == -1)
            throw CgiException("Fork error");
        if (childPid == 0) {
            // child
            // Build char array from the _environment variables to pass to execve
            for (std::map<std::string, std::string>::const_iterator it = this->_environment.begin(); it != this->_environment.end(); ++it) {
                std::string envVar = it->first + "=" + it->second;
                char *env = new char[envVar.length() + 1];
                strcpy(env, envVar.c_str());
                this->_childEnvp.push_back(env);
            }
            this->_childEnvp.push_back(nullptr);
            // pipes for std: redirecting std in/out to the pipe ends and close unused
            // necessary for file upload
                close(pipe_in[0]);
                close(pipe_out[1]);

            	dup2(pipe_in[0], STDIN_FILENO);
                dup2(pipe_out[1], STDOUT_FILENO);

                // for post requests: write the buffer to the write end of the pipe
                if (postBuffer != nullptr) {
                    write(pipe_in[1], postBuffer, strlen(postBuffer));
                    close(pipe_in[1]);
                }

                close(pipe_out[0]);
                close(pipe_out[1]);
            // execve to execute the cgi program
            if (execve(this->_runLoc, argv,
                &this->_childEnvp[0]) == -1){
                    perror("execve failed");
                    throw CgiException("Execve failed");
            }
            for (size_t i = 0; i < this->_childEnvp.size(); ++i)
                delete[] this->_childEnvp[i];
        } else {
            // Parent
            int status;
            // for POST requests: should probably close this again, since this is the parent
            close(pipe_out[1]);
            waitpid(childPid, &status, 0);

            char buffer[CGI_BUFSIZE];
            std::string cgiOutput;

            // capture the output of the cgi
            while (true) {
                ssize_t bytesRead = read(pipe_out[0], buffer, CGI_BUFSIZE);
                if (bytesRead <= 0) {
                    if (bytesRead < 0)
                        throw CgiException("Read Error");
                    break;  // End of output or an error
                }
            cgiOutput.append(buffer, static_cast<size_t>(bytesRead));
        }

        }
        // for post requests: delete buffer
        if (postBuffer != nullptr)
            delete[] postBuffer;


    }   catch (const CgiException& e) {
            // HANDLE EXCEPTION: LOGGING?
            if (postBuffer != nullptr) 
                delete[] postBuffer;
            for (size_t i = 0; i < this->_childEnvp.size(); ++i)
                delete[] this->_childEnvp[i];
            std::cerr << "Error: " << e.what() << std::endl;
            exit(1);
    }

}

std::map<std::string, std::string>  CgiHandler::get_environment() const {
    return this->_environment;
}

void    CgiHandler::print_env(){
    for (std::map<std::string, std::string>::const_iterator it = this->_environment.begin(); it != this->_environment.end(); ++it) {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }}
