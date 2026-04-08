#include "Luma/Audio/AudioDevice.h"
#include <miniaudio.h>

namespace Luma
{
    bool FAudioDevice::initialize(const FAudioDeviceDesc& desc)
    {
        if (m_Handle)
        {
            ma_engine_uninit(static_cast<ma_engine*>(m_Handle));
            delete(static_cast<ma_engine*>(m_Handle));
        }
        m_Handle = new ma_engine;
        ma_engine_config config = ma_engine_config_init();
        config.channels = desc.numChannels;
        config.sampleRate = desc.sampleRate;
        config.listenerCount = desc.maxListeners;

        if (ma_engine_init(&config, static_cast<ma_engine*>(m_Handle)) != MA_SUCCESS)
            return false;
        return true;
    }

    void FAudioDevice::destroy()
    {
        if (m_Handle)
        {
            ma_engine_uninit(static_cast<ma_engine*>(m_Handle));
            delete(static_cast<ma_engine*>(m_Handle));
        }
    }

    FAudioDevice* createAudioDevice(const FAudioDeviceDesc& desc)
    {
        FAudioDevice* device = new FAudioDevice();
        if (!device->initialize(desc))
        {
            delete device;
            return nullptr;
        }
        return device;
    }
}
