
//https://stackoverflow.com/questions/49577244/serial-communication-between-rpi-and-arduino-using-c
//https://www.dexterindustries.com/howto/run-a-program-on-your-raspberry-pi-at-startup/
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
        retval = usb->read(&imu_payload,sizeof(imu_payload));
        printf("%d\n",udp->transmit(&imu_payload, sizeof(imu_payload)));
        if (retval > 0)
        {
            printf("0x%x ",imu_payload.sync_word);
            printf("%f ",imu_payload.yaw);
            printf("%f ",imu_payload.pitch);
            printf("%f\n",imu_payload.roll);
        }
        else
        {
            printf("INVALID\n");
        }
    }

    if (usb > 0) delete usb;
    if (udp > 0) delete udp;
}
