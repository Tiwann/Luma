#include "Luma/Audio/AudioDevice.h"
#include <miniaudio.h>
#include <cstdlib>
#include <cstring>

#define MA_FAILED(result) ((result) != MA_SUCCESS)
#define MA_RETURN_ON_FAIL(result) if(MA_FAILED((result))) return false
#define MA_RETURN_ON_FAIL_DATA(result, data) if(MA_FAILED((result))) return (data)

namespace Luma
{
    struct FAudioDevice::Impl
    {
        ma_engine m_Engine;
        uint32_t m_Channels;
        uint32_t m_SampleRate;
        uint32_t m_ListenerCount;

        Impl() : m_Channels(0), m_SampleRate(0), m_ListenerCount(0)
        {
            memset(&m_Engine, 0, sizeof(ma_engine));
        }
        
        static void* OnMalloc(size_t size, void* userData)
        {
            return malloc(size);
        }
        
        static void* OnRealloc(void* where,size_t size, void* userData)
        {
            return realloc(where, size);
        }
        
        static void OnFree(void* ptr, void* userData)
        {
            free(ptr);
        }
        
        static void AudioProc(ma_device* device, void* output, const void* input, ma_uint32 frameCount)
        {
            ma_engine* engine = (ma_engine*)device->pUserData;
            ma_engine_read_pcm_frames(engine, output, frameCount, nullptr);
        }
        
        static void OnNotification(const ma_device_notification* notification)
        {
            
        }
        
        static void OnProcess(void* userData, float* framesOut, ma_uint64 frameCount)
        {
            
        }
    };

    FAudioDevice::FAudioDevice()
    {
        m_Pimpl = new Impl;

    }

    FAudioDevice::~FAudioDevice()
    {
        delete m_Pimpl;
    }

    bool FAudioDevice::initialize(const FAudioDeviceDesc& desc)
    {
        
        const ma_allocation_callbacks allocationCallbacks
        {
            this,
            &Impl::OnMalloc,
            &Impl::OnRealloc,
            &Impl::OnFree
        };
        
        ma_engine_config config = ma_engine_config_init();
        config.channels = desc.numChannels;
        config.sampleRate = desc.sampleRate;
        config.listenerCount = desc.maxListeners;
        config.allocationCallbacks = allocationCallbacks;
        config.dataCallback = &Impl::AudioProc;
        config.notificationCallback = &Impl::OnNotification;
        config.onProcess = &Impl::OnProcess;
        config.pProcessUserData = this;
        config.noAutoStart = true;
        
        ma_engine_stop(&m_Pimpl->m_Engine);
        ma_engine_uninit(&m_Pimpl->m_Engine);
        ma_result result = ma_engine_init(&config, &m_Pimpl->m_Engine);
        MA_RETURN_ON_FAIL(result);

        result = ma_engine_start(&m_Pimpl->m_Engine);
        MA_RETURN_ON_FAIL(result);

        m_Pimpl->m_Channels = desc.numChannels;
        m_Pimpl->m_SampleRate = desc.sampleRate;
        m_Pimpl->m_ListenerCount = desc.maxListeners;
        return true;
    }

    void FAudioDevice::destroy()
    {
        ma_engine_stop(&m_Pimpl->m_Engine);
        ma_engine_uninit(&m_Pimpl->m_Engine);
    }

    uint32_t FAudioDevice::getNumListeners() const
    {
        return m_Pimpl->m_ListenerCount;
    }

    uint32_t FAudioDevice::getNumChannels() const
    {
        return m_Pimpl->m_Channels;
    }

    uint32_t FAudioDevice::getSampleRate() const
    {
        return m_Pimpl->m_SampleRate;
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
