// #include <iostream>
// #include <unistd.h>

// int main() {
//     const char* filePath = "/usr/local/bin/php"; // Use the correct path to the PHP interpreter
//     char* const argv[] = {
//         const_cast<char*>(filePath),
//         const_cast<char*>("/Users/thirza/Documents/Codam/webserv/WebServer/www/penguinserv/cgiRoute/coolCgi.php"), // Replace with the path to your PHP script
//         nullptr
//     };

//     if (execv(filePath, argv) == -1) {
//         perror("execv");
//         return 1;
//     }

//     return 0;
// }


#include <iostream>
#include <unistd.h>

int main() {
    const char* filePath = "/usr/local/bin/php"; // Use the correct path to the PHP interpreter
    char* const argv[] = {
        const_cast<char*>("coolCgi.php"), // The first argument is typically the program name
        const_cast<char*>("/Users/thirza/Documents/Codam/webserv/WebServer/www/penguinserv/cgiRoute/coolCgi.php"), // Replace with the path to your PHP script
        nullptr
    };

    char* const envp[] = { nullptr }; // You can pass environment variables here if needed

    if (execve(filePath, argv, envp) == -1) {
        perror("execve");
        return 1;
    }

    return 0;
}
