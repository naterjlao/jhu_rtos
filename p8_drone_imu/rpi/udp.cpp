#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "protocol.hpp"

PROTOCOL::UDP::UDP(const char* dest_ip, const int dest_port, const int process_port)
{
    bool status = true;
    
    // ----- ALLOCATE SOCKET ----- //
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
    {
        printf("PROTOCOL::UDP socket allocation failed\n");
        status = false;
    }
#if DEBUG
    else printf("PROTOCOL::UDP socket 0x%x\n",socket_fd);
#endif

    // ----- ENABLE BROADCAST ----- //
    if (status)
    {
        int enable = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable)) != 0)
        {
            printf("PROTOCOL::UDP setsockopt failed\n");
            status = false;
        }
#if DEBUG
        else printf("PROTOCOL::UDP setsockopt success\n",socket_fd);
#endif
    }

    // ----- BIND ----- //
    if (status)
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(process_port);
        if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
        {
            printf("PROTCOL::UDP bind failed\n");
            status = false;
        }
#if DEBUG
        else printf("PROTOCOL::UDP bind success\n",socket_fd);
#endif
    }

    // ----- SET TARGET IP and PORT ----- //
    if (status)
    {
        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_addr.s_addr = inet_addr(dest_ip);
        dest_addr.sin_port = htons(dest_port);
    }
}

PROTOCOL::UDP::~UDP()
{
    if (socket_fd >= 0) close(socket_fd);
}

size_t PROTOCOL::UDP::transmit(const void *buf, size_t len)
{
    return sendto(socket_fd, buf, len, 0, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
}

#if 0
/// @note IP 239.255.255.250:8250
/// works for some reason (http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xhtml)
const char* MULTICAST_IP = "239.100.100.250";
const int MULTICAST_PORT = 8250;
const int SERVER_PORT = 8000;

int main(void)
{
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("socket 0x%x\n",socket_fd);

    int enable = 1;
    int ret=setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
    printf("setsockopt %d\n",ret);

    struct sockaddr_in host;
    memset(&host, 0, sizeof(host));
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = htonl(INADDR_ANY);
    host.sin_port = htons(SERVER_PORT);

    ret = bind(socket_fd, (struct sockaddr*)&host, sizeof(host));
    printf("bind %d\n",ret);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MULTICAST_IP);
    addr.sin_port = htons(MULTICAST_PORT);

    const char *message = "Hello, World!";
    int nbytes;
    while(1)
    {
        nbytes = sendto(socket_fd, message, strlen(message),0,(struct sockaddr*) &addr, sizeof(addr));
        //printf("transmit %d\n",nbytes);
        //sleep(1);
    }

    return 0;
}
#endif
