#include "Luma/Rendering/ImguiRenderer.h"
#include "Luma/Rendering/RenderDevice.h"
#include <imgui.h>

#ifdef LUMA_BUILD_VULKAN
#include "Luma/Vulkan/ImGuiRendererImpl.h"
#endif

namespace Luma
{
    static void toLinear(ImVec4& color)
    {
        color.x = color.x <= 0.04045f ? color.x / 12.92f : pow((color.x + 0.055f) / 1.055f, 2.4f);
        color.y = color.y <= 0.04045f ? color.y / 12.92f : pow((color.y + 0.055f) / 1.055f, 2.4f);
        color.z = color.z <= 0.04045f ? color.z / 12.92f : pow((color.z + 0.055f) / 1.055f, 2.4f);
    }

    void lumaStyle(ImGuiStyle* style)
    {
        ImGuiStyle& styleRef = style ? *style : ImGui::GetStyle();
        ImVec4* colors = styleRef.Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        styleRef.WindowPadding = ImVec2(8.00f, 8.00f);
        styleRef.FramePadding = ImVec2(5.00f, 2.00f);
        styleRef.CellPadding = ImVec2(6.00f, 6.00f);
        styleRef.ItemSpacing = ImVec2(6.00f, 6.00f);
        styleRef.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
        styleRef.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        styleRef.IndentSpacing = 25;
        styleRef.ScrollbarSize = 15;
        styleRef.GrabMinSize = 10;
        styleRef.WindowBorderSize = 1;
        styleRef.ChildBorderSize = 1;
        styleRef.PopupBorderSize = 1;
        styleRef.FrameBorderSize = 1;
        styleRef.TabBorderSize = 1;
        styleRef.WindowRounding = 7;
        styleRef.ChildRounding = 4;
        styleRef.FrameRounding = 3;
        styleRef.PopupRounding = 4;
        styleRef.ScrollbarRounding = 9;
        styleRef.GrabRounding = 3;
        styleRef.LogSliderDeadzone = 4;
        styleRef.TabRounding = 4;

        // This is a hack to convert colors in linear space as the engine uses SRGB for the swapchain
        for (int colorIndex = 0; colorIndex < ImGuiCol_COUNT; colorIndex++) {
            toLinear(styleRef.Colors[colorIndex]);
        }
    }

    bool IImguiRenderer::initialize(const FImguiRendererDesc& rendererDesc)
    {
        if(!IMGUI_CHECKVERSION()) return false;

        m_Context = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        if(!m_Context) return false;
        

        ImGui::SetCurrentContext(m_Context);
        lumaStyle(nullptr);

        // TODO: Add imguizmo
        //ImGuizmo::SetImGuiContext(m_Context);
        return true;
    }

    IImguiRenderer* createImguiRenderer(const FImguiRendererDesc& rendererDesc)
    {
        if (!rendererDesc.window) return nullptr;
        if (!rendererDesc.device) return nullptr;
        if (rendererDesc.sampleCount % 2 != 0 && rendererDesc.sampleCount > 16) return nullptr;
        IImguiRenderer* renderer = nullptr;
        switch (rendererDesc.device->getDeviceType())
        {
        case ERenderDeviceType::None: return nullptr;
#ifdef LUMA_BUILD_VULKAN
        case ERenderDeviceType::Vulkan:
            {
                renderer = new Vulkan::FImguiRendererImpl();
                if (!renderer->initialize(rendererDesc))
                {
                    delete renderer;
                    return nullptr;
                }
                return renderer;
            }
#endif
        }
        return renderer;
    }

}
