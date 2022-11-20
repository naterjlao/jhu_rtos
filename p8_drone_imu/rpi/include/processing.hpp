#ifndef __PROCESSING_HPP__
#define __PROCESSING_HPP__

#include "imu_def.hpp"
#include "protocol.hpp"

namespace IMU
{
    void process(const IMU::PAYLOAD* input, PROTOCOL::IMU_DATA* output);
}

#endif