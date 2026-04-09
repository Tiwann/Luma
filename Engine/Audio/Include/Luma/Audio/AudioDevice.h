#pragma once
#include "Luma/Audio/Export.h"
#include "Luma/Core/RTTI.h"
#include <cstdint>

namespace luma
{
    struct FAudioDeviceDesc
    {
        uint32_t numChannels = 2;
        uint32_t sampleRate = 44100;
        uint32_t maxListeners = 4;
    };

    struct LUMA_AUDIO_API IAudioDevice
    {
        virtual ~IAudioDevice() = default;
        virtual bool initialize(const FAudioDeviceDesc& desc = FAudioDeviceDesc()) = 0;
        virtual void destroy() = 0;
    };

    class LUMA_AUDIO_API FAudioDevice : public IAudioDevice
    {
    public:
        bool initialize(const FAudioDeviceDesc& desc) override;
        void destroy() override;

        static constexpr const rtti::FClass* staticClass()
        {
            static constexpr rtti::FClass instance("AudioDevice", nullptr);
            return &instance;
        }
    private:
        void* m_Handle = nullptr;
    };

    LUMA_AUDIO_API FAudioDevice* createAudioDevice(const FAudioDeviceDesc& desc = FAudioDeviceDesc());
}