//-----------------------------------------------------------------------------
/// @file imu.cpp
/// @author Nate Lao (nlao1@jh.edu)
/// @brief Main Driver for the FlyPi IMU process.
/// @details References:
/// https://stackoverflow.com/questions/49577244/serial-communication-between-rpi-and-arduino-using-c
/// https://www.dexterindustries.com/howto/run-a-program-on-your-raspberry-pi-at-startup/
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "imu_def.hpp"
#include "serial.hpp"
#include "protocol.hpp"
#include "processing.hpp"

//-----------------------------------------------------------------------------
/// @brief Defines the destination IP for the FlyPi data.
/// @note IP 239.255.255.250:8250 works for some reason
/// see (http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xhtml)
//-----------------------------------------------------------------------------
const char* TARGET_IP = "239.100.100.250";

//-----------------------------------------------------------------------------
/// @brief Defines the destination port for the FlyPi IMU data.
//-----------------------------------------------------------------------------
const int TARGET_PORT = 8250;

//-----------------------------------------------------------------------------
/// @brief Defines the host port for the FlyPi IMU process.
//-----------------------------------------------------------------------------
const int PROCESS_PORT = 8000;

//-----------------------------------------------------------------------------
/// @brief Main Driver for FlyPi IMU process
//-----------------------------------------------------------------------------
int main()
{
    int retval = 0;

    // ----- INPUT OBJECTS ----- //
    SERIAL::USB* usb = new SERIAL::USB("/dev/ttyACM1",9600,IMU::SYNC_WORD);
    IMU::PAYLOAD imu_payload;

    // ----- OUTPUT OBJECTS ----- //
    PROTOCOL::UDP* udp = new PROTOCOL::UDP(TARGET_IP, TARGET_PORT, PROCESS_PORT);
    PROTOCOL::IMU_DATA imu_data;

    while (1)
    {
        // ----- INPUT ----- //
        retval = usb->read(&imu_payload,sizeof(imu_payload));

        // ----- PROCESS ----- //
        if (retval > 0)
        {
            IMU::process(&imu_payload, &imu_data);
        }
        else
        {
            printf("IMU Thead: Serial Read Failure\n");
            imu_data.validity = 0x0;
        }
        
        // ----- OUTPUT ----- //
        retval = udp->transmit(&imu_data, sizeof(imu_data));
#if DEBUG
        printf("IMU Thead: UDP transmission retval=%d\n",retval);
#endif
    }

    if (usb > 0) delete usb;
    if (udp > 0) delete udp;
}

//-----------------------------------------------------------------------------
/// @brief Host-to-Network conversion for 32-bit float values.
//-----------------------------------------------------------------------------
static float htonf32(float input)
{
    float output;
    unsigned long buffer;
    memcpy(&buffer, &input, 4);
    buffer = htonl(buffer);
    memcpy(&output, &buffer, 4);
    return output;
}

//-----------------------------------------------------------------------------
/// @brief Processes the given serial input to udp output
/// @note For now, this does a straight pass-thru, with no processing.
/// If any filtering is to be performed, it should be performed here.
//-----------------------------------------------------------------------------
void IMU::process(const IMU::PAYLOAD* input, PROTOCOL::IMU_DATA* output)
{
    output->validity = 0x0;

    output->yaw = htonf32(input->yaw);
    output->validity |= 0x1 << 0;
    printf("YAW=%f ",input->yaw);
    
    output->pitch = htonf32(input->pitch);
    output->validity |= 0x1 << 1;
    printf("PITCH=%f ",input->pitch);
    
    output->roll = htonf32(input->roll);
    output->validity |= 0x1 << 2;
    printf("ROLL=%f\n",input->roll);
}