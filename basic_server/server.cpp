#include "tcpServer.hpp"

int main(void)
{
    tcpServer server = tcpServer("127.0.0.1", 8080);
    return 0;
}
