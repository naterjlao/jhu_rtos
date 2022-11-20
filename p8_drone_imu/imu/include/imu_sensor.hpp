//-----------------------------------------------------------------------------
/// @file imu_sensor.hpp
/// @author Nate Lao (nlao1@jh.edu)
/// @brief Defines the IMU sensor class.
//-----------------------------------------------------------------------------
#ifndef __IMU_SENSOR_HPP__
#define __IMU_SENSOR_HPP__

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "imu_def.hpp"

namespace IMU
{
    class Sensor
    {
    public:
        Sensor(int32_t sensorID, IMU::PAYLOAD* bufferAddr);
        bool begin();
        bool frame();
    private:
        IMU::PAYLOAD* buffer;
        Adafruit_BNO055 sensor;
        sensors_event_t event;
    };
}

#endif