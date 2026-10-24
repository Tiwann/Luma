#pragma once
#include "Engine.h"
#include "Window.h"
#include "Luma/Asset/AudioClip.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Rendering/Device.h"

namespace Luma
{
    class GameEngine : public Engine
    {
    public:
        void initialize() override;
        void start() override;
        void update(float deltaTime) override;
        void destroy() override;

    private:
        Ref<Window> m_Window = nullptr;
        Ref<RHI::Device> m_GPUDevice = nullptr;
        Ref<Miniaudio::AudioDevice> m_AudioDevice = nullptr;
    };
}
