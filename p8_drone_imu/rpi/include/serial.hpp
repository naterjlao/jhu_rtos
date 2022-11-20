//-----------------------------------------------------------------------------
/// @file serial.hpp
/// @author Nate Lao (nlao1@jh.edu)
/// @brief Defines MCU-to-RPi Serial Interfaces.
//-----------------------------------------------------------------------------
#ifndef __SERIAL_HPP__
#define __SERIAL_HPP__

#include <stddef.h>

#include "imu_def.hpp"

namespace SERIAL 
{
    class USB
    {
    public:
        USB(const char* device, int baud, uint32_t sync_word);
        ~USB();
        int read(void* buf_ptr, size_t buf_size);
    private:
        int fd;
        const static size_t SYNC_WORD_SIZE = sizeof(uint32_t);
        unsigned char sync_word[SYNC_WORD_SIZE];
    };

    class SPI
    {
    public:
        /// @todo to be defined
        SPI();
        int read(void* buf_ptr, size_t buf_size);
    };

    class I2C
    {
    public:
        /// @todo to be defined
        I2C();
        int read(void* buf_ptr, size_t buf_size);
    };
}

#endif