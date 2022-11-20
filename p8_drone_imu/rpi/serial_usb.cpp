#include <stdio.h>
#include <string.h>
#include <wiringSerial.h>

#include "serial.hpp"
#include "imu_def.hpp"

SERIAL::USB::USB(const char* device, int baud, uint32_t sync_word)
{
    fd = serialOpen(device, baud);
    memcpy(this->sync_word, &sync_word, SYNC_WORD_SIZE);
#if DEBUG
    if (fd < 0) printf("SERIAL::USB constructor failed.\n");
#endif
}

SERIAL::USB::~USB()
{
    if (fd >= 0)
    {
        serialClose(fd);
    }
}

int SERIAL::USB::read(void* buf_ptr, size_t buf_size)
{
    int retval = -1;

    if (fd >= 0)
    {
        // ----- SYNC READ HEAD ----- //
        /// @note This loop synchronizes the serial read head
        /// with the expected sync word.
        /// @note the Raspberry Pi and the Seeed Arduino slave are
        /// BOTH little endian. Sync will NOT work if endianess
        /// was mismatched...
        retval = 0;
        size_t idx = 0;
        while ((idx < SYNC_WORD_SIZE) && (retval >= 0))
        {
            retval = serialGetchar(fd);
            if ((retval >= 0) && (((unsigned char) retval) == sync_word[idx]))
                idx++;
        }

        if (idx == SYNC_WORD_SIZE)
        {
            memcpy(buf_ptr, sync_word, SYNC_WORD_SIZE);
            unsigned char* buffer = (unsigned char*) buf_ptr;
            while (idx < buf_size)
            {
                /// @note We are assuming that if the sync word got thru,
                /// then so did the rest... Pull in the rest of the data.
                /// @todo This could be more robust...
                /// @note The WiringPi library already does masking,
                /// casting from int to char should be OK.
                retval = serialGetchar(fd);
                buffer[idx] = (unsigned char) retval;
                idx++;
            }
            // Finally, set retval to the request size
            retval = (int) buf_size;
        }
    }

    return retval;
}