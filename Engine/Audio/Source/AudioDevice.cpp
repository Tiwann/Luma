#include "Luma/Audio/AudioDevice.h"
#include "Luma/Asset/AudioClip.h"
#include "Luma/Containers/String.h"
#include "Luma/Containers/StringFormat.h"
#include "Luma/Runtime/FileUtils.h"
#include <miniaudio.h>
#include <cstdlib>
#include <cstring>


#define MA_FAILED(result) ((result) != MA_SUCCESS)
#define MA_RETURN_ON_FAIL(result) if(MA_FAILED((result))) return false
#define MA_RETURN_ON_FAIL_DATA(result, data) if(MA_FAILED((result))) return (data)

namespace Luma
{
    AudioClip* AudioDevice::createAudioClip(StringView filepath, AudioFormat requestedFormat)
    {
        Array<uint8_t> fileData = FileUtils::readToBuffer(filepath);
        return createAudioClip(fileData.data(), fileData.size(), requestedFormat);
    }

    AudioDevice* createAudioDevice(const AudioDeviceDesc& desc)
    {
        Luma::AudioDevice* device = new Miniaudio::AudioDevice();
        if (!device->initialize(desc))
        {
            delete device;
            return nullptr;
        }
        return device;
    }

    namespace Miniaudio
    {
        struct AudioDevice::Impl
        {
            ma_engine engine;
            ma_device* device = nullptr;

            static void* onMalloc(size_t size, void* userData)
            {
                return malloc(size);
            }

            static void* onRealloc(void* where,size_t size, void* userData)
            {
                return realloc(where, size);
            }

            static void onFree(void* ptr, void* userData)
            {
                free(ptr);
            }

            static void audioProc(ma_device* device, void* output, const void* input, ma_uint32 frameCount)
            {
                ma_engine* engine = (ma_engine*)device->pUserData;
                ma_engine_read_pcm_frames(engine, output, frameCount, nullptr);
            }

            static void onNotification(const ma_device_notification* notification)
            {

            }

            static void onProcess(void* userData, float* framesOut, ma_uint64 frameCount)
            {

            }
        };
        
        bool AudioDevice::initialize(const AudioDeviceDesc& desc)
        {
            if (m_Pimpl) delete m_Pimpl;
            m_Pimpl = new Impl();

            const ma_allocation_callbacks allocationCallbacks
            {
                this,
                &Impl::onMalloc,
                &Impl::onRealloc,
                &Impl::onFree
            };
        
            ma_engine_config config = ma_engine_config_init();
            config.channels = desc.channelCount;
            config.sampleRate = desc.sampleRate;
            config.listenerCount = desc.maxListeners;
            config.allocationCallbacks = allocationCallbacks;
            config.dataCallback = &Impl::audioProc;
            config.notificationCallback = &Impl::onNotification;
            config.onProcess = &Impl::onProcess;
            config.pProcessUserData = this;
            config.noAutoStart = true;
        
            ma_engine_stop(&m_Pimpl->engine);
            ma_engine_uninit(&m_Pimpl->engine);
            ma_result result = ma_engine_init(&config, &m_Pimpl->engine);
            MA_RETURN_ON_FAIL(result);

            result = ma_engine_start(&m_Pimpl->engine);
            MA_RETURN_ON_FAIL(result);

            auto* device = ma_engine_get_device(&m_Pimpl->engine);
            auto* context = ma_device_get_context(device);
            const char* backendName = ma_get_backend_name(context->backend);

            String infoString;
            infoString.append(strfmt("Using miniaudio with {} backend.\n", backendName));
            infoString.append(strfmt("    Channel count: {}\n", desc.channelCount));
            infoString.append(strfmt("    Sample rate: {}\n", desc.sampleRate));
            infoString.append(strfmt("    Max listeners: {}\n", desc.maxListeners));
            infoString.append(strfmt("Successfully initialized audio device!"));
            std::cout << infoString << std::endl;

            m_ChannelCount = desc.channelCount;
            m_SampleRate = desc.sampleRate;
            m_ListenerCount = desc.maxListeners;
            m_Pimpl->device = ma_engine_get_device(&m_Pimpl->engine);
            return true;
        }

        void AudioDevice::destroy()
        {
            ma_engine_stop(&m_Pimpl->engine);
            ma_engine_uninit(&m_Pimpl->engine);
            delete m_Pimpl;
        }

        Luma::AudioClip* AudioDevice::createAudioClip(const uint8_t* data, uint64_t dataSize, AudioFormat requestedFormat)
        {
            Luma::AudioClip* audioClip = new AudioClip();
            if (!audioClip->initialize(this, data, dataSize, requestedFormat))
            {
                delete audioClip;
                return nullptr;
            }
            return audioClip;
        }

        Luma::AudioClip* AudioDevice::createAudioClipFromSamples(const uint8_t* samples, uint64_t samplesSize)
        {
            return nullptr;
        }

        ma_engine* AudioDevice::getEngine() const
        {
            return &m_Pimpl->engine;
        }

        ma_device* AudioDevice::getDevice() const
        {
            return m_Pimpl->device;
        }
    }
}
