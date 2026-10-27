#pragma once
#include "Luma/Audio/Export.h"
#include "Luma/Memory/RefCounted.h"
#include "Luma/Containers/StringView.h"
#include <cstdint>

#include "Luma/Runtime/Types.h"

struct ma_engine;
struct ma_device;

namespace Luma
{
    struct AudioDeviceDesc
    {
        uint32_t channelCount = 2;
        uint32_t sampleRate = 44100;
        uint32_t maxListeners = 4;
    };

    struct AudioClip;
    struct AudioStream;
    struct AudioFormat;

    struct LUMA_AUDIO_API AudioDevice : RefCounted<AudioDevice>
    {
        ~AudioDevice() override = default;
        virtual bool initialize(const AudioDeviceDesc& desc = AudioDeviceDesc()) = 0;
        virtual void destroy() = 0;

        virtual AudioClip* createAudioClip(const uint8_t* data, uint64_t dataSize, AudioFormat requestedFormat) = 0;
        virtual AudioClip* createAudioClip(StringView filepath, AudioFormat requestedFormat);
        virtual AudioClip* createAudioClipFromSamples(const uint8_t* samples, uint64_t samplesSize) = 0;

        virtual void playSound2D(const Luma::AudioClip* audioClip, float volume, float pitch, float pan) = 0;

        uint32_t getListenerCount() const { return m_ListenerCount; }
        uint32_t getChannelCount() const { return m_ChannelCount; }
        uint32_t getSampleRate() const { return m_SampleRate; }
    protected:
        uint32_t m_ListenerCount = 0;
        uint32_t m_ChannelCount = 0;
        uint32_t m_SampleRate = 0;
    };

    namespace Miniaudio
    {
        class LUMA_AUDIO_API AudioDevice final : public Luma::AudioDevice
        {
        public:
            bool initialize(const AudioDeviceDesc& desc) override;
            void destroy() override;

            AudioClip* createAudioClip(const uint8_t* data, uint64_t dataSize, AudioFormat requestedFormat) override;
            AudioClip* createAudioClipFromSamples(const uint8_t* samples, uint64_t samplesSize) override;
            void playSound2D(const Luma::AudioClip* audioClip, float volume, float pitch, float pan) override;

            ma_engine* getEngine() const;
            ma_device* getDevice() const;
        private:
            struct Impl;
            Impl* m_Pimpl = nullptr;
        };
    }


    LUMA_AUDIO_API AudioDevice* createAudioDevice(const AudioDeviceDesc& desc = AudioDeviceDesc());
}
