#pragma once
#include "Luma/Runtime/Application.h"

namespace Luma
{
    class FEditorApplication : public IApplication
    {
    public:
        FEditorApplication(const int32_t argc, char** argv) : IApplication(argc, argv){}
        FApplicationConfig getConfiguration() const override;
        ERenderDeviceType getRenderDeviceType() const override;

        void onInit() override;
        void onDrawGui() override;
        void onDestroy() override;
    };
}
