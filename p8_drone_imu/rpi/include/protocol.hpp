//-----------------------------------------------------------------------------
/// @file protocol.hpp
/// @author Nate Lao (nlao1@jh.edu)
/// @brief Defines RPi-to-Host Protocol communcations.
//-----------------------------------------------------------------------------
#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <netinet/in.h>

namespace PROTOCOL
{
    class UDP
    {
    public:
        UDP(const char* dest_ip, const int dest_port, const int process_port);
        ~UDP();
        size_t transmit(const void *buf, size_t len);
    private:
        int socket_fd;
        struct sockaddr_in dest_addr;
    };

    typedef struct
    {
        float yaw;
        float pitch;
        float roll;
        unsigned char validity;
    } IMU_DATA;   
}

#endif