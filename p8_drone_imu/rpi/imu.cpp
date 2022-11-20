
//https://stackoverflow.com/questions/49577244/serial-communication-between-rpi-and-arduino-using-c
//https://www.dexterindustries.com/howto/run-a-program-on-your-raspberry-pi-at-startup/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "imu_def.hpp"
#include "serial.hpp"

int main()
{
    SERIAL::USB* usb = new SERIAL::USB("/dev/ttyACM1",9600,IMU::SYNC_WORD);
    IMU::PAYLOAD imu_payload;
    int retval = 0;

    while (1)
    {
        retval = usb->read(&imu_payload,sizeof(imu_payload));
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

    if (usb > 0)
    {
        delete usb;
    }
}
