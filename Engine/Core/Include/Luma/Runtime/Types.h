#pragma once

#ifndef LUMA_PLATFORM_SWITCH
#include <cstdint>
using uint8_t = std::uint8_t;
using uint16_t = std::uint16_t;
using uint32_t = std::uint32_t;
using uint64_t = std::uint64_t;
using int8_t = std::int8_t;
using int16_t = std::int16_t;
using int32_t = std::int32_t;
using int64_t = std::int64_t;
#else
#include <switch/types.h>
using uint8_t = u8;
using uint16_t = u16;
using uint32_t = u32;
using uint64_t = u64;
using int8_t = s8;
using int16_t = s16;
using int32_t = s32;
using int64_t = s64;
#endif