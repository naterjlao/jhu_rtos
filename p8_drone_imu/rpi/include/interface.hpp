#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

namespace INTERFACE 
{
    class Serial_USB
    {
        Serial_USB(char* device, int baud);
        bool sync(char* symbol, size_t size);
        size_t read(void* buf_ptr, size_t buf_size);
    }

    class Serial_SPI
    {
        /// @todo to be defined
        Serial_SPI();
        bool sync(char* symbol, size_t size);
        size_t read(void* buf_ptr, size_t buf_size);
    }

    class Serial_I2C
    {
        /// @todo to be defined
        Serial_I2C();
        bool sync(char* symbol, size_t size);
        size_t read(void* buf_ptr, size_t buf_size);
    }
}

#endif