//-----------------------------------------------------------------------------
/// @file imu_def.hpp
/// @author Nate Lao (nlao1@jh.edu)
/// @brief Defines the IMU serial payload
//-----------------------------------------------------------------------------
#ifndef __IMU_DEF_HPP__
#define __IMU_DEF_HPP__

#if RPI
#include <netinet/in.h>
#define float_t float
#endif

namespace IMU
{
    //-----------------------------------------------------------------------------
    /// @brief Defines the serial payload buffer.
    /// @details The buffer contents are defined as follows:
    //
    /// Label            | Byte Index | Type     | Valid Values/Range  | Failure Values
    /// ---------------- | ---------- | -------- | ------------------- | ------------------
    /// Sync Word        | 0..3       | U32      | 0x444F5045          | 0x43524150
    /// Yaw (X-Rot)      | 4..7       | F32      | 0..360 (CW -> +)    | 0xFFFFFFFF
    /// Pitch (Y-Rot)    | 8..11      | F32      | -90..90 (UP -> +)   | 0xFFFFFFFF
    /// Roll (Z-Rot)     | 12..15     | F32      | -180..180 (R -> -)  | 0xFFFFFFFF
    //
    //-----------------------------------------------------------------------------
    typedef struct
    {
        uint32_t sync_word;
        float_t yaw;
        float_t pitch;
        float_t roll;
    } PAYLOAD;
    const uint32_t SYNC_WORD = 0x444F5045;
    const uint32_t FAIL_WORD = 0x43524150;
    const PAYLOAD FAILURE_PAYLOAD = {FAIL_WORD, (float_t)0xFFFFFFFF, (float_t)0xFFFFFFFF, (float_t)0xFFFFFFFF};
}
#endif