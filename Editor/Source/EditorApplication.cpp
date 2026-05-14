#include "EditorApplication.h"
#include "Luma/Runtime/EntryPoint.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Luma
{
    IApplication* createApplication(const int argc, char** argv)
    {
        return new FEditorApplication(argc, argv);
    }

    FApplicationConfig FEditorApplication::getConfiguration() const
    {
        FApplicationConfig config;
        config.applicationName = "Luma Editor";
        config.windowWidth = 1600;
        config.windowHeight = 900;
        config.windowFlags = EWindowCreateBits::Centered | EWindowCreateBits::Resizable;
        config.msaaSamples = 1;
        config.vsync = true;
        return config;
    }

    ERenderDeviceType FEditorApplication::getRenderDeviceType() const
    {
        return ERenderDeviceType::Vulkan;
    }

    void FEditorApplication::onInit()
    {

    }

    void FEditorApplication::onDrawGui()
    {
        /*// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const Ref<IWindow> window = getWindow();
        const bool isMaximized = window->isMaximized();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
        ImGui::Begin("LumaEditor", nullptr, window_flags);
        {
            ImGui::PopStyleColor(); // MenuBarBg
            ImGui::PopStyleVar(2);
            ImGui::PopStyleVar(2);

            if (ImGui::Begin("Viewport"))
            {

            }
            ImGui::End();
        }
        ImGui::End();*/
    }

    void FEditorApplication::onDestroy()
    {

    }
}
