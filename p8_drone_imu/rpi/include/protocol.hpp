#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

namespace PROTOCOL
{
    typedef struct
    {
        float yaw;
        float pitch;
        float roll;
        unsigned char validity;
    } IMU;   
}

#endif