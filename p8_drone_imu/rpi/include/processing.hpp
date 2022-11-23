//-----------------------------------------------------------------------------
/// @file processing.hpp
/// @author Nate Lao (nlao1@jh.edu)
/// @brief Defines Processing Functions for FlyPi operations.
//-----------------------------------------------------------------------------
#ifndef __PROCESSING_HPP__
#define __PROCESSING_HPP__

#include "imu_def.hpp"
#include "protocol.hpp"

namespace IMU
{
    void process(const IMU::PAYLOAD* input, PROTOCOL::IMU_DATA* output);
}

#endif