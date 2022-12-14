//-----------------------------------------------------------------------------
/// @file imu.ino
/// @author Nate Lao (nlao1@jh.edu)
/// @brief Main RTOS driver for the IMU Sensor.
/// @details Designed for Seeed Studio Xiao microcontroller.
//-----------------------------------------------------------------------------

// ----- RTOS LIBRARY ----- //
#include <Seeed_Arduino_FreeRTOS.h>

// ----- LOCAL LIBRARIES ----- //
#include "include/imu_def.hpp"
#include "include/imu_sensor.hpp"

const int32_t BNO_SENSOR_ID = 55; /** @note Sensor ID, do not change */
const unsigned long TX_RATE = 50; /** @note Delay per transmission (smaller -> faster) */

IMU::PAYLOAD buffer;
IMU::Sensor *sensor_driver = 0;

// ----- RTOS THREADS ----- //

/// @note Prority Ranking (larger -> higher priority)
UBaseType_t PRIORITY_SENSOR_TASK = tskIDLE_PRIORITY + 1;
TaskHandle_t Handle_SensorTask;
static void SensorThread(void *pvParameters)
{
  sensor_driver = new IMU::Sensor(BNO_SENSOR_ID, &buffer);

  // Attempt to intialize the sensor
  // If failed, re-attempt indefinitely
  while (!sensor_driver->begin())
  {
    delay(100);
  }

  // Sensor Intialization Success - Normal Operation
  while (1)
  {
    sensor_driver->frame();
  }
}

/// @note Prority Ranking (larger -> higher priority)
UBaseType_t PRIORITY_SERIAL_TASK = tskIDLE_PRIORITY + 2;
TaskHandle_t Handle_SerialTask;
static void SerialThread(void *pvParameters)
{
  while (1)
  {
    Serial.write((const char *)&buffer, sizeof(buffer));
    delay(TX_RATE);
  }
}

// ----- MAIN ----- //
void setup(void)
{
  // Intialize the output payload to FAILURE on startup
  memcpy(&buffer, &IMU::FAILURE_PAYLOAD, sizeof(buffer));

  // Setup Serial
  Serial.begin(9600);
  vNopDelayMS(1000); // prevents usb driver crash on startup, do not omit this
  while (!Serial)
  {
  } // Wait for Serial terminal to open port before starting program

  // Spin up the threads
  xTaskCreate(SensorThread, "IMU Sensor Sampling", 256, NULL, PRIORITY_SENSOR_TASK, &Handle_SensorTask);
  xTaskCreate(SerialThread, "Serial Transmission", 256, NULL, PRIORITY_SERIAL_TASK, &Handle_SerialTask);

  // Start the RTOS, this function will never return and will schedule the tasks.
  vTaskStartScheduler();
}

void loop(void)
{
  // NOTHING
}