#pragma once
#include "Luma/Rendering/RenderDeviceType.h"
#include <cstdint>
#include <string>

namespace Luma
{
    struct IWindow;
    struct IAudioDevice;
    struct IRenderDevice;
    struct ICommandBuffer;

    struct FApplicationConfiguration
    {
        std::string applicationName = "Nova Application";
        uint32_t windowWidth = 600;
        uint32_t windowHeight = 400;
        bool vsync = false;
        uint32_t msaaSamples = 8;
    };

    struct IApplication
    {
        explicit IApplication(int32_t argc, char** argv) = default;
        virtual ~IApplication() = default;

        void run();
        void exit();


        virtual FApplicationConfiguration getConfiguration() const = 0;
        virtual ERenderDeviceType getRenderDeviceType() const = 0;

        virtual void onInit(){}
        virtual void onUpdate(float deltaTime){}
        virtual void onPreRender(ICommandBuffer& cmdBuffer){}
        virtual void onRender(ICommandBuffer& cmdBuffer){}
        virtual void onPostRender(ICommandBuffer& cmdBuffer){}
        virtual void onGUI(){}
        virtual void onDrawDebug(){}
        virtual void onDestroy(){}

        float getDeltaTime() const;

        const IWindow* getWindow() const { return m_Window; }
        IWindow* getWindow() { return m_Window; }

        const IRenderDevice* getRenderDevice() const { return m_RenderDevice; }
        IRenderDevice* getRenderDevice() { return m_RenderDevice; }

        const IAudioDevice* getAudioDevice() const { return m_AudioDevice; }
        IAudioDevice* getAudioDevice() { return m_AudioDevice; }

        uint32_t GetWindowWidth() const;
        uint32_t GetWindowHeight() const;

    protected:
        void update();
        void render();
        void destroy();

    private:
        IWindow* m_Window = nullptr;
        IAudioDevice* m_AudioDevice = nullptr;
        IRenderDevice* m_RenderDevice = nullptr;

        bool m_IsRunning = true;
        double m_LastTime = 0.0f;
        double m_DeltaTime = 0.0f;
    };
}
