#include "Luma/Asset/AudioClip.h"
#include "Luma/Audio/AudioDevice.h"
#include "Luma/Containers/StringConversion.h"
#include <miniaudio.h>


LUMA_DEFINE_CLASS(Luma::AudioClip)

namespace Luma::Miniaudio
{
    struct AudioClip::Impl
    {
        ma_decoder decoder;
        ma_sound sound;
    };

    void AudioClip::destroy()
    {
        ma_decoder_uninit(&m_Pimpl->decoder);
        ma_sound_uninit(&m_Pimpl->sound);
        delete m_Pimpl;
    }

    static ma_format to_ma_format(SampleFormat format)
    {
        switch (format)
        {
        case SampleFormat::F32: return ma_format_f32;
        case SampleFormat::S32: return ma_format_s32;
        case SampleFormat::S24: return ma_format_s24;
        case SampleFormat::S16: return ma_format_s16;
        case SampleFormat::U8: return ma_format_u8;
        default: return ma_format_unknown;
        }
    }

    bool AudioClip::initialize(Luma::AudioDevice* device, const uint8_t* data, uint64_t dataSize, AudioFormat requestedFormat)
    {
        if (!device) return false;
        if (!data) return false;
        if (!m_Pimpl) m_Pimpl = new Impl();

        ma_decoder_uninit(&m_Pimpl->decoder);
        const ma_decoder_config decoderConfig = ma_decoder_config_init(to_ma_format(requestedFormat.format), requestedFormat.channelCount, requestedFormat.sampleRate);
        ma_result result = ma_decoder_init_memory(data, dataSize, &decoderConfig, &m_Pimpl->decoder);
        if (result != MA_SUCCESS) return false;

        AudioDevice* deviceImpl = static_cast<AudioDevice*>(device);
        ma_engine* engine = deviceImpl->getEngine();
        if (!engine) return false;

        ma_sound_uninit(&m_Pimpl->sound);
        result = ma_sound_init_from_data_source(engine, &m_Pimpl->decoder, MA_SOUND_FLAG_DECODE, nullptr, &m_Pimpl->sound);
        if (result != MA_SUCCESS) return false;

        uint64_t durationFrames = 0;
        ma_sound_get_length_in_pcm_frames(&m_Pimpl->sound, &durationFrames);
        m_Samples = Array<uint8_t>(durationFrames * requestedFormat.channelCount * getSampleFormatSize(requestedFormat.format));
        ma_uint64 framesRead = 0;
        ma_decoder_read_pcm_frames(&m_Pimpl->decoder, m_Samples.data(), m_DurationFrames, &framesRead);
        if (framesRead != m_DurationFrames)
        {
            ma_sound_uninit(&m_Pimpl->sound);
            ma_decoder_uninit(&m_Pimpl->decoder);
            m_Samples.free();
            m_DurationFrames = 0;
            m_ChannelCount = 0;
            m_SampleRate = 0;
            return false;
        }

        m_AudioDevice = device;
        m_DurationFrames = durationFrames;
        m_SampleRate = requestedFormat.sampleRate;
        m_ChannelCount = requestedFormat.channelCount;
        return true;
    }
}
