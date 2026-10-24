#pragma once
#include "Luma/Runtime/Types.h"

namespace Luma
{
    enum class SampleFormat
    {
        F32,
        S32,
        S24,
        S16,
        U8
    };

    struct AudioFormat
    {
        SampleFormat format;
        uint32_t channelCount;
        uint32_t sampleRate;
    };

    static uint64_t getSampleFormatSize(SampleFormat format)
    {
        switch (format)
        {
        case SampleFormat::F32: return 4;
        case SampleFormat::S32: return 4;
        case SampleFormat::S24: return 3;
        case SampleFormat::S16: return 2;
        case SampleFormat::U8: return 1;
        default: return 0;
        }
    }
}
