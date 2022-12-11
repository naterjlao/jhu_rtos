// Based on https://www.electronicwings.com/raspberry-pi/gps-module-interfacing-with-raspberry-pi
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "protocol.hpp"

//-----------------------------------------------------------------------------
/// @brief Defines the destination IP for the FlyPi data.
/// @note IP 239.255.255.250:8250 works for some reason
/// see (http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xhtml)
//-----------------------------------------------------------------------------
const char *TARGET_IP = "239.100.100.250";

//-----------------------------------------------------------------------------
/// @brief Defines the destination port for the FlyPi IMU data.
//-----------------------------------------------------------------------------
const int TARGET_PORT = 8252;

//-----------------------------------------------------------------------------
/// @brief Defines the host port for the FlyPi IMU process.
//-----------------------------------------------------------------------------
const int PROCESS_PORT = 8002;

int main()
{
    // ----- OUTPUT OBJECTS ----- //
    PROTOCOL::UDP *udp = new PROTOCOL::UDP(TARGET_IP, TARGET_PORT, PROCESS_PORT);

    int serial_port;
    char dat, buff[100], GGA_code[3];
    unsigned char IsitGGAstring = 0;
    unsigned char GGA_index = 0;
    unsigned char is_GGA_received_completely = 0;

    if ((serial_port = serialOpen("/dev/ttyACM0", 9600)) < 0) /* open serial port */
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    if (wiringPiSetup() == -1) /* initializes wiringPi setup */
    {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
        return 1;
    }

    // Ensure clearout of transmit buffer
    memset(buff, 0, sizeof(buff));
    while (1)
    {

        if (serialDataAvail(serial_port)) /* check for any data available on serial port */
        {
            dat = serialGetchar(serial_port); /* receive character serially */
            if (dat == '$')
            {
                IsitGGAstring = 0;
                GGA_index = 0;
            }
            else if (IsitGGAstring == 1)
            {
                buff[GGA_index++] = dat;
                if (dat == '\r')
                    is_GGA_received_completely = 1;
            }
            else if (GGA_code[0] == 'G' && GGA_code[1] == 'G' && GGA_code[2] == 'A')
            {
                IsitGGAstring = 1;
                GGA_code[0] = 0;
                GGA_code[0] = 0;
                GGA_code[0] = 0;
            }
            else
            {
                GGA_code[0] = GGA_code[1];
                GGA_code[1] = GGA_code[2];
                GGA_code[2] = dat;
            }
        }
        if (is_GGA_received_completely == 1)
        {
            printf("GGA: %s", buff);
            udp->transmit(buff, sizeof(buff));
            is_GGA_received_completely = 0;
        }
    }
    return 0;
}