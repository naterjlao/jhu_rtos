#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>

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
const int TARGET_PORT = 8251;

//-----------------------------------------------------------------------------
/// @brief Defines the host port for the FlyPi IMU process.
//-----------------------------------------------------------------------------
const int PROCESS_PORT = 8001;

//-----------------------------------------------------------------------------
/// @brief Defines the compression parameters for each frame.
/// @note Special care has to be done to ensure that the output image sent 
/// through UDP does not exceed the UDP size limit. This has to be done by a
/// case by case basis and can verified by calling buffer.size().
//-----------------------------------------------------------------------------
const char *IMAGE_ENCODING = ".jpg";
const int IMAGE_PARAM = cv::IMWRITE_JPEG_QUALITY;
const int IMAGE_QUALITY = 15;

int main(int, char **)
{
    // ----- OUTPUT OBJECTS ----- //
    PROTOCOL::UDP *udp = new PROTOCOL::UDP(TARGET_IP, TARGET_PORT, PROCESS_PORT);

    //--- INITIALIZE VIDEOCAPTURE ----- //
    cv::VideoCapture cap;
    cv::Mat frame;
    std::vector<uchar> buffer;
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    cap.open(deviceID, apiID); // open selected camera using selected API
    
    // Validate Camera open
    if (!cap.isOpened())
    {
        printf("ERROR! Unable to open camera\n");
        return -1;
    }
    else
    {
        printf("Starting camera transmission -> %s:%d\n", TARGET_IP, TARGET_PORT);
    }

    // Setup Image Compression
    std::vector<int> COMPRESSION_PARAMS;
    COMPRESSION_PARAMS.push_back(IMAGE_PARAM);
    COMPRESSION_PARAMS.push_back(IMAGE_QUALITY);

    // ----- CONTROL LOOP ----- // 
    while (true)
    {
        // ----- READ RAW CAMERA FRAME ----- //
        cap.read(frame);

        // ----- ENCODE FOOTAGE ----- //
        cv::imencode(".jpg", frame, buffer, COMPRESSION_PARAMS);

        // ----- OUTPUT ----- //
        udp->transmit(buffer.data(), buffer.size());
    }

    if (udp > 0) delete udp;
    return 0;
}
