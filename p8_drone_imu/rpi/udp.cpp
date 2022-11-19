#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char* MULTICAST_IP = "192.168.4.100";
const int MULTICAST_PORT = 8266;

int main(void)
{
    int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("socket 0x%x\n",socket_fd);

    int enable = 1;
    int ret=setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
    printf("setsockopt %d\n",ret);

    struct sockaddr_in host;
    memset(&host, 0, sizeof(host));
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = htonl(INADDR_ANY);
    host.sin_port = htons(1900);

    ret = bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr));
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