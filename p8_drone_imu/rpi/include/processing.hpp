#ifndef __PROCESSING_HPP__
#define __PROCESSING_HPP__

#include "imu_def.hpp"
#include "protocol.hpp"

namespace PROCESSING
{
    namespace IMU_PROCESS
    {
        void process(IMU::PAYLOAD* input, PROTOCOL::IMU_DATA* output);
    }
}

#endif