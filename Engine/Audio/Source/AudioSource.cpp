#include "Luma/Components/AudioSource.h"

namespace Luma
{
    Ref<AudioClip> AudioSource::getClip() const
    {
        return m_AudioClip;
    }

    void AudioSource::setClip(Ref<AudioClip> audioClip)
    {
        m_AudioClip = audioClip;
    }

    void AudioSource::setVolume(float volume)
    {
        m_Volume = volume;
    }

    float AudioSource::getVolume() const
    {
        return m_Volume;
    }

    void AudioSource::setPitch(float pitch)
    {
        m_Pitch = pitch;
    }

    float AudioSource::getPitch() const
    {
        return m_Pitch;
    }

    void AudioSource::play()
    {
        AudioDevice* device = m_AudioClip->getAudioDevice();
    }

    void AudioSource::pause()
    {
    }

    void AudioSource::stop()
    {
    }
}
