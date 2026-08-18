#include "Luma/Asset/AudioClip.h"
#include "Luma/Audio/AudioDevice.h"
#include "Luma/Containers/StringConversion.h"

//#define STB_VORBIS_HEADER_ONLY
//#include <stb_vorbis.c>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>


namespace Luma
{
    FAudioClip::FAudioClip()
    {
        m_Handle = new ma_sound;
        m_Decoder = new ma_decoder;
    }

    FAudioClip::~FAudioClip()
    {
    }

    bool FAudioClip::loadFromFile(FStringView filepath)
    {
        FAudioDevice* audioDevice = FAudioDevice::getInstance();
        if (!audioDevice) return false;

        const ma_decoder_config decoderConfig = ma_decoder_config_init(ma_format_s16, 2, 44100);
        ma_result result = ma_decoder_init_file_w(*stringConvert<wchar_t, char>(filepath), &decoderConfig, m_Decoder);
        if (result != MA_SUCCESS) return false;

        //result = ma_sound_init_from_data_source(audioDevice->get, &m_Decoder, 0, nullptr, &m_Handle);
        //if (result != MA_SUCCESS) return false;

        return false;
    }

    bool FAudioClip::loadFromMemory(const void* data, size_t size)
    {
        return false;
    }

    void FAudioClip::destroy()
    {

    }

    EAssetType FAudioClip::getAssetType() const
    {
        return EAssetType::AudioClip;
    }

    float FAudioClip::getDurationSeconds() const
    {
        return 0.0f;
    }

    uint64_t FAudioClip::getDurationFrames() const
    {
        return 0;
    }
}
