#pragma once
#include "Luma/Audio/AudioFormat.h"
#include "Luma/Runtime/Asset.h"
#include "Luma/Audio/Export.h"
#include "Luma/Containers/Array.h"
#include "Luma/Containers/ArrayView.h"

struct ma_sound;

namespace Luma
{
    struct AudioDevice;

    struct LUMA_AUDIO_API AudioClip : Asset
    {
        LUMA_CLASS(AudioClip, Asset)
    public:
        AudioClip() = default;

        virtual bool initialize(AudioDevice* device, const uint8_t* data, uint64_t dataSize, AudioFormat requestedFormat) = 0;

        AssetType getAssetType() const final { return AssetType::AudioClip; }

        uint64_t getDurationFrames() const { return m_DurationFrames; }
        float getDurationSeconds() const { return m_DurationFrames / static_cast<float>(m_SampleRate); }
        uint32_t getSampleRate() const { return m_SampleRate; }
        uint32_t getChannelCount() const { return m_ChannelCount; }
        ArrayView<uint8_t> getSamples() const { return m_Samples; }

        AudioDevice* getAudioDevice() const { return m_AudioDevice; }
    protected:
        AudioDevice* m_AudioDevice = nullptr;
        uint64_t m_DurationFrames = 0;
        uint32_t m_SampleRate = 0;
        uint32_t m_ChannelCount = 0;
        Array<uint8_t> m_Samples;
    };

    namespace Miniaudio
    {
        class AudioDevice;

        class LUMA_AUDIO_API AudioClip final : public Luma::AudioClip
        {
        public:
            void destroy() override;
            ma_sound* getHandle() const;
        protected:
            bool initialize(Luma::AudioDevice* device, const uint8_t* data, uint64_t dataSize, AudioFormat requestedFormat) override;
        private:
            friend AudioDevice;
            struct Impl;
            Impl* m_Pimpl = nullptr;
        };
    }
}
