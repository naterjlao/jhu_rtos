#ifndef __ENDIAN_HPP__
#define __ENDIAN_HPP__

/// @brief Quick and dirty endian check
/// @return true if the endianess of the host system is big endian, false otherwise
/// @note this should only be called once during intialization
inline bool big_endian()
{
    uint32_t STANDARD = 0xAABBCCDD;
    char mem[sizeof(STANDARD)];
    memcpy(mem,&STANDARD,sizeof(STANDARD));
    return (mem[0] & 0xFF) == 0xAA;
}

#endif