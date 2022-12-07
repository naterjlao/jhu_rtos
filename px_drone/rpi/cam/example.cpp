#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

int main(int, char **)
{
    Mat input;
    Mat output;

    vector<int> COMPRESSION_PARAMS;
    COMPRESSION_PARAMS.push_back(IMWRITE_PNG_COMPRESSION);
    COMPRESSION_PARAMS.push_back(0);

    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
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
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    cap.read(input);
    if (input.empty())
    {
        std::cerr << "ERROR!" << std::endl;
    }

    // Raw image is upside down
    cv::rotate(input, output, ROTATE_180);

    // Temporary - testing
    cv::imwrite("camtest.png", output, COMPRESSION_PARAMS);

    return 0;
}
