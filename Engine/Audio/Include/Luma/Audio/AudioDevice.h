#pragma once
#include "Luma/Audio/Export.h"
#include "Luma/Memory/RefCounted.h"
#include <cstdint>

namespace Luma
{
    struct FAudioDeviceDesc
    {
        uint32_t numChannels = 2;
        uint32_t sampleRate = 44100;
        uint32_t maxListeners = 4;
    };

    struct LUMA_AUDIO_API IAudioDevice : IRefCounted<IAudioDevice>
    {
        ~IAudioDevice() override = default;
        virtual bool initialize(const FAudioDeviceDesc& desc = FAudioDeviceDesc()) = 0;
        virtual void destroy() = 0;
    };

    class LUMA_AUDIO_API FAudioDevice final : public IAudioDevice
    {
    public:
        FAudioDevice();
        ~FAudioDevice() override;
        bool initialize(const FAudioDeviceDesc& desc) override;
        void destroy() override;

        uint32_t getNumListeners() const;
        uint32_t getNumChannels() const;
        uint32_t getSampleRate() const;

    private:
        struct Impl;
        Impl* m_Pimpl = nullptr;
    };

    LUMA_AUDIO_API FAudioDevice* createAudioDevice(const FAudioDeviceDesc& desc = FAudioDeviceDesc());
}