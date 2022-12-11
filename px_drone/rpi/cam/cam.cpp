#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

#include "protocol.hpp"

//-----------------------------------------------------------------------------
/// @brief Defines the destination IP for the FlyPi data.
/// @note IP 239.255.255.250:8250 works for some reason
/// see (http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xhtml)
//-----------------------------------------------------------------------------
const char* TARGET_IP = "239.100.100.250";

//-----------------------------------------------------------------------------
/// @brief Defines the destination port for the FlyPi IMU data.
//-----------------------------------------------------------------------------
const int TARGET_PORT = 8251;

//-----------------------------------------------------------------------------
/// @brief Defines the host port for the FlyPi IMU process.
//-----------------------------------------------------------------------------
const int PROCESS_PORT = 8001;

int main(int, char **)
{
    // ----- OUTPUT OBJECTS ----- //
    PROTOCOL::UDP* udp = new PROTOCOL::UDP(TARGET_IP, TARGET_PORT, PROCESS_PORT);

    cv::Mat input;
    cv::Mat output;

    std::vector<int> COMPRESSION_PARAMS;
    COMPRESSION_PARAMS.push_back(cv::IMWRITE_PNG_COMPRESSION);
    COMPRESSION_PARAMS.push_back(0);

    //--- INITIALIZE VIDEOCAPTURE ----- //
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    int idx = 0;
    while(1)
    {
    cap.read(input);
    //if (input.empty())
    //{
    //    std::cerr << "ERROR!" << std::endl;
    //}

    // Raw image is upside down
    //cv::rotate(input, output, cv::ROTATE_180);
    
        // ----- OUTPUT ----- //
        retval = udp->transmit(&input, sizeof(input));
        std::cout << idx++ << retval << std::endl;
    }
    // Temporary - testing
    //cv::imwrite("camtest.png", output, COMPRESSION_PARAMS);

    return 0;
}
