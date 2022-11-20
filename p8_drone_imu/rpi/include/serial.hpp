#ifndef __SERIAL_HPP__
#define __SERIAL_HPP__

namespace SERIAL 
{
    class USB
    {
        Serial_USB(char* device, int baud);
        bool sync(char* symbol, size_t size);
        size_t read(void* buf_ptr, size_t buf_size);
    }

    class SPI
    {
        /// @todo to be defined
        Serial_SPI();
        bool sync(char* symbol, size_t size);
        size_t read(void* buf_ptr, size_t buf_size);
    }

    class I2C
    {
        /// @todo to be defined
        Serial_I2C();
        bool sync(char* symbol, size_t size);
        size_t read(void* buf_ptr, size_t buf_size);
    }
}

#endif