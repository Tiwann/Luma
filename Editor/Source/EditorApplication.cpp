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

    }

    void FEditorApplication::onDestroy()
    {

    }
}
