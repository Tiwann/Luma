#pragma once
#include "Luma/Audio/Export.h"
#include <cstdint>

namespace Luma
{
    struct FAudioDeviceDesc
    {
        uint32_t numChannels = 2;
        uint32_t sampleRate = 44100;
        uint32_t maxListeners = 4;
    };

    class LUMA_AUDIO_API FAudioDevice
    {
    public:
        bool initialize(const FAudioDeviceDesc& desc);
        void destroy();
    private:
        void* m_Handle = nullptr;
    };

    LUMA_AUDIO_API FAudioDevice* createAudioDevice(const FAudioDeviceDesc& desc = FAudioDeviceDesc());
}