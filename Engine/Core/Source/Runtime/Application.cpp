#include "Luma/Runtime/Application.h"
#include "Luma/Rendering/RenderPassDesc.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Renderer2D.h"
#include "Luma/Runtime/Time.h"
#include "Luma/Runtime/Window.h"
#include "Luma/Input/Immediate.h"
#include <imgui.h>


namespace Luma
{
    IApplication::IApplication(int32_t argc, char** argv)
    {
        (void)argc;
        (void)argv;
    }

    void IApplication::run()
    {
        const FApplicationConfig configuration = getConfiguration();
        const DeviceType deviceType = getGpuDeviceType();

        WindowDesc windowDesc;
        windowDesc.title = configuration.applicationName;
        windowDesc.width = configuration.windowWidth;
        windowDesc.height = configuration.windowHeight;
        windowDesc.options = configuration.windowFlags;
        m_Window = createWindow(windowDesc);
        if (!m_Window)
        {
            destroy();
            return;
        }
        m_Window->closedEvent.bindMember(this, &IApplication::exit);

        RHI::DeviceDesc rdDesc;
        rdDesc.window = m_Window;
        rdDesc.buffering = SwapchainBuffering::DoubleBuffering;
        rdDesc.vSync = configuration.vsync;
        rdDesc.deviceType = deviceType;
        m_Device = RHI::createDevice(rdDesc);
        if (!m_Device)
        {
            destroy();
            return;
        }

        ImguiRendererDesc imguiRendererDesc;
        imguiRendererDesc.device = m_Device;
        imguiRendererDesc.sampleCount = 1;
        imguiRendererDesc.window = m_Window;
        m_ImguiRenderer = createImguiRenderer(imguiRendererDesc);
        if (!m_ImguiRenderer)
        {
            destroy();
            return;
        }

        /*m_Renderer2D = Ref<FRenderer2D>::create();
        if (!m_Renderer2D->initialize(m_GpuDevice))
        {
            destroy();
            return;
        }*/

        FAudioDeviceDesc audioDeviceDesc;
        audioDeviceDesc.sampleRate = 44100;
        audioDeviceDesc.numChannels = 2;
        audioDeviceDesc.maxListeners = 4;
        m_AudioDevice = createAudioDevice(audioDeviceDesc);
        if (!m_AudioDevice)
        {
            destroy();
            return;
        }

        onInit();
        update();
        destroy();
    }

    void IApplication::exit()
    {
        m_IsRunning = false;
    }

    void IApplication::update()
    {
        while (m_IsRunning)
        {
            const double currentTime = FTime::getTime();
            m_DeltaTime = currentTime - m_LastTime;
            m_LastTime = currentTime;
            m_Window->pollEvents();

            m_Renderer2D->begin();
            onUpdate(static_cast<float>(m_DeltaTime));
            onDrawGui();
            m_Renderer2D->end();

            m_ImguiRenderer->beginFrame();
            ImGui::DockSpaceOverViewport(ImGui::GetID("Dockspace"), ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
            m_ImguiRenderer->endFrame();

            render();
        }
    }

    void IApplication::render()
    {
        if (m_Device->beginFrame())
        {
            RHI::CommandBuffer* cmdBuffer = m_Device->getCommandBuffer();
            onPreRender(cmdBuffer);

            const RHI::TextureView* swapchainTexture = m_Device->getAcquiredSwapchainTextureView();
            const FRect2<uint32_t> renderArea = {0, 0, m_Window->getWidth(), m_Window->getHeight()};

            RHI::RenderPassTarget colorAttachment;
            colorAttachment.type = RHI::RenderPassTargetType::Color;
            colorAttachment.loadOp = LoadOp::Clear;
            colorAttachment.storeOp = StoreOp::Store;
            colorAttachment.clearValue.color = Color::Black;
            colorAttachment.textureView = swapchainTexture;

            RHI::RenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = renderArea;
            renderPassDesc.colorTargets.add(&colorAttachment);

            cmdBuffer->beginRenderPass(renderPassDesc);
            onRender(cmdBuffer);
            //m_Renderer2D->render();
            cmdBuffer->endRenderPass();


            RHI::RenderPassTarget imguiColorAttachment;
            imguiColorAttachment.type = RHI::RenderPassTargetType::Color;
            imguiColorAttachment.loadOp = LoadOp::Load;
            imguiColorAttachment.storeOp = StoreOp::Store;
            imguiColorAttachment.textureView = swapchainTexture;

            RHI::RenderPassDesc imguiRenderPassDesc;
            imguiRenderPassDesc.renderArea = renderArea;
            imguiRenderPassDesc.colorTargets.add(&imguiColorAttachment);

            cmdBuffer->beginDebugGroup("ImGui", Color::Purple);
            cmdBuffer->beginRenderPass(imguiRenderPassDesc);
            m_ImguiRenderer->render(cmdBuffer);
            cmdBuffer->endRenderPass();
            cmdBuffer->endDebugGroup();

            m_Device->endFrame();
            m_Device->present();
        }
    }

    void IApplication::destroy()
    {
        if (m_Device) m_Device->waitIdle();
        onDestroy();
        
        m_AudioDevice = nullptr;
        m_Renderer2D = nullptr;
        m_ImguiRenderer = nullptr;
        m_Device = nullptr;
        m_Window = nullptr;
    }


    float IApplication::getDeltaTime() const
    {
        return static_cast<float>(m_DeltaTime);
    }

    Ref<Window> IApplication::getWindow() const
    {
        return m_Window;
    }

    Ref<IAudioDevice> IApplication::getAudioDevice() const
    {
        return m_AudioDevice;
    }

    Ref<RHI::Device> IApplication::getDevice() const
    {
        return m_Device;
    }

    Ref<FRenderer2D> IApplication::getRenderer2D() const
    {
        return m_Renderer2D;
    }

    FRect2<uint32_t> IApplication::getWindowBounds() const
    {
        return m_Window->getBounds();
    }

    FVector2<uint32_t> IApplication::getWindowSize() const
    {
        return FVector2<uint32_t>(m_Window->getWidth(), m_Window->getHeight());
    }
}
