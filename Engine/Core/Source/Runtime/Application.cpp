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
        const ERenderDeviceType deviceType = getRenderDeviceType();

        FWindowDesc windowDesc;
        windowDesc.title = configuration.applicationName;
        windowDesc.width = configuration.windowWidth;
        windowDesc.height = configuration.windowHeight;
        windowDesc.flags = configuration.windowFlags;
        m_Window = createWindow(windowDesc);
        if (!m_Window)
        {
            destroy();
            return;
        }
        m_Window->closedEvent.bindMember(this, &IApplication::exit);
        FInput::initialize(m_Window);


        FRenderDeviceDesc rdDesc;
        rdDesc.window = m_Window;
        rdDesc.buffering = ESwapchainBuffering::DoubleBuffering;
        rdDesc.vSync = configuration.vsync;
        rdDesc.deviceType = deviceType;
        m_RenderDevice = createRenderDevice(rdDesc);
        if (!m_RenderDevice)
        {
            destroy();
            return;
        }

        FImguiRendererDesc imguiRendererDesc;
        imguiRendererDesc.device = m_RenderDevice;
        imguiRendererDesc.sampleCount = 1;
        imguiRendererDesc.window = m_Window;
        m_ImguiRenderer = createImguiRenderer(imguiRendererDesc);
        if (!m_ImguiRenderer)
        {
            destroy();
            return;
        }

        m_Renderer2D = Ref<FRenderer2D>::create();
        if (!m_Renderer2D->initialize(m_RenderDevice))
        {
            destroy();
            return;
        }

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
            FInput::update();

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
        if (m_RenderDevice->beginFrame())
        {
            ICommandBuffer* cmdBuffer = m_RenderDevice->getCommandBuffer();
            onPreRender(cmdBuffer);

            const ITextureView* swapchainTexture = m_RenderDevice->getAcquiredSwapchainTextureView();
            const FRect2<uint32_t> renderArea = {0, 0, m_Window->getWidth(), m_Window->getHeight()};

            FRenderPassAttachment colorAttachment;
            colorAttachment.type = ERenderPassAttachmentType::Color;
            colorAttachment.loadOp = ELoadOp::Clear;
            colorAttachment.storeOp = EStoreOp::Store;
            colorAttachment.clearValue.color = FColor::Black;
            colorAttachment.textureView = swapchainTexture;

            FRenderPassDesc renderPassDesc;
            renderPassDesc.renderArea = renderArea;
            renderPassDesc.colorAttachments.add(&colorAttachment);

            cmdBuffer->beginRenderPass(renderPassDesc);
            onRender(cmdBuffer);
            m_Renderer2D->render(cmdBuffer, m_Window->getWidth(), m_Window->getHeight());
            cmdBuffer->endRenderPass();


            FRenderPassAttachment imguiColorAttachment;
            imguiColorAttachment.type = ERenderPassAttachmentType::Color;
            imguiColorAttachment.loadOp = ELoadOp::Load;
            imguiColorAttachment.storeOp = EStoreOp::Store;
            imguiColorAttachment.textureView = swapchainTexture;

            FRenderPassDesc imguiRenderPassDesc;
            imguiRenderPassDesc.renderArea = renderArea;
            imguiRenderPassDesc.colorAttachments.add(&imguiColorAttachment);

            cmdBuffer->beginDebugGroup("ImGui", FColor::Purple);
            cmdBuffer->beginRenderPass(imguiRenderPassDesc);
            m_ImguiRenderer->render(cmdBuffer);
            cmdBuffer->endRenderPass();
            cmdBuffer->endDebugGroup();

            m_RenderDevice->endFrame();
            m_RenderDevice->present();
        }
    }

    void IApplication::destroy()
    {
        FInput::destroy();
        if (m_RenderDevice) m_RenderDevice->waitIdle();
        onDestroy();
        
        m_AudioDevice = nullptr;
        m_Renderer2D = nullptr;
        m_ImguiRenderer = nullptr;
        m_RenderDevice = nullptr;
        m_Window = nullptr;
    }


    float IApplication::getDeltaTime() const
    {
        return static_cast<float>(m_DeltaTime);
    }

    Ref<IWindow> IApplication::getWindow() const
    {
        return m_Window;
    }

    Ref<IAudioDevice> IApplication::getAudioDevice() const
    {
        return m_AudioDevice;
    }

    Ref<IRenderDevice> IApplication::getRenderDevice() const
    {
        return m_RenderDevice;
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
