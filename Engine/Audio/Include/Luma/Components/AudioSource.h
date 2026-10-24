#pragma once
#include "Luma/Asset/AudioClip.h"
#include "Luma/Memory/Ref.h"

namespace Luma
{
    class AudioSource
    {
    public:
        Ref<AudioClip> getClip() const;
        void setClip(Ref<AudioClip> audioClip);

        void setVolume(float volume);
        float getVolume() const;

        void setPitch(float pitch);
        float getPitch() const;

        void play();
        void pause();
        void stop();

    private:
        Ref<AudioClip> m_AudioClip = nullptr;
        float m_Volume = 1.0f;
        float m_Pitch = 1.0f;
        bool m_Looping = false;
    };
}
