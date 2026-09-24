#pragma once
#include "Luma/Audio/AudioDevice.h"
#include "Luma/Containers/String.h"
#include "Luma/Math/Vector.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/DeviceType.h"
#include "Luma/Rendering/Renderer2D.h"
#include "Luma/Rendering/ImguiRenderer.h"
#include "Window.h"

#include <cstdint>

namespace Luma
{
    struct FApplicationConfig
    {
        String applicationName = "Luma Application";
        uint32_t windowWidth = 600;
        uint32_t windowHeight = 400;
        WindowOptionsFlags windowFlags = 0;
        bool vsync = false;
        uint32_t msaaSamples = 8;
    };

    struct IApplication
    {
        explicit IApplication(int32_t argc, char** argv);
        virtual ~IApplication() = default;

        void run();
        void exit();

        virtual FApplicationConfig getConfiguration() const = 0;
        virtual DeviceType getGpuDeviceType() const = 0;

        virtual void onInit(){}
        virtual void onUpdate(float deltaTime){}
        virtual void onPreRender(RHI::CommandBuffer* cmdBuffer){}
        virtual void onRender(RHI::CommandBuffer* cmdBuffer){}
        virtual void onPostRender(RHI::CommandBuffer* cmdBuffer){}
        virtual void onDrawGui(){}
        virtual void onDrawDebug(){}
        virtual void onDestroy(){}

        float getDeltaTime() const;
        Ref<Window> getWindow() const;
        Ref<IAudioDevice> getAudioDevice() const;
        Ref<RHI::Device> getDevice() const;
        Ref<Renderer2D> getRenderer2D() const;

        FRect2<uint32_t> getWindowBounds() const;
        FVector2<uint32_t> getWindowSize() const;
    protected:
        void update();
        void render();
        void destroy();

    private:
        Ref<Window> m_Window = nullptr;
        Ref<RHI::Device> m_Device = nullptr;
        Ref<ImguiRenderer> m_ImguiRenderer = nullptr;
        Ref<Renderer2D> m_Renderer2D = nullptr;
        Ref<IAudioDevice> m_AudioDevice = nullptr;

        uint32_t m_IsRunning = true;
        double m_LastTime = 0.0f;
        double m_DeltaTime = 0.0f;
    };
}
