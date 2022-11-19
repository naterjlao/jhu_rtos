#include <Seeed_Arduino_FreeRTOS.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "include/imu_sensor.hpp"

//-----------------------------------------------------------------------------
/// @brief IMU Sensor
//-----------------------------------------------------------------------------
IMU::Sensor::Sensor(int32_t sensorID, IMU::PAYLOAD *bufferAddr)
{
    buffer = bufferAddr;
    sensor = Adafruit_BNO055(sensorID);
}

//-----------------------------------------------------------------------------
/// @brief  Initializes the IMU Sensor. Must be called before operations
/// @return true if IMU Sensor intialization was successful.
//-----------------------------------------------------------------------------
bool IMU::Sensor::begin()
{
    bool retval = sensor.begin();
    if (retval)
        sensor.setExtCrystalUse(true);
    return retval;
}

bool IMU::Sensor::frame()
{
    bool retval = sensor.getEvent(&event);

    /// @todo investigate low-pass filtering
    /// @todo the retval for BNO sensor may always be true, investigate
    /// invalid values as failure operations may never occur (even in
    /// the event of an actual failure)

    if (retval)
    {
        /// @note Mismatched rotation measurements can be
        /// tolerated. The higher priority task should
        /// not be able to interrupt during singular assignments
        /// (though they might be atomic by nature...)

        // Sync Word
        taskENTER_CRITICAL();
        buffer->sync_word = IMU::SYNC_WORD;
        taskEXIT_CRITICAL();

        // Yaw Measurements
        taskENTER_CRITICAL();
        buffer->yaw = event.orientation.x;
        taskEXIT_CRITICAL();
        
        // Pitch Measurements
        taskENTER_CRITICAL();
        buffer->pitch = event.orientation.y;
        taskEXIT_CRITICAL();
        
        // Roll Measurements
        taskENTER_CRITICAL();
        buffer->roll = event.orientation.z;
        taskEXIT_CRITICAL();
    }
    else
    {
        taskENTER_CRITICAL();
        memcpy(buffer, &IMU::FAILURE_PAYLOAD, sizeof(IMU::PAYLOAD));
        taskEXIT_CRITICAL();
    }

    return retval;
}