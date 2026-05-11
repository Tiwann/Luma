#include <Luma/Containers/String.h>
#include <Luma/Containers/StringView.h>
#include <Luma/Rendering/RenderDeviceType.h>
#include <Luma/Rendering/SwpchainBuffering.h>

using namespace Luma;

uint32_t g_Samples = 1;
bool g_Vsync = false;
ESwapchainBuffering g_Buffering = ESwapchainBuffering::DoubleBuffering;
ERenderDeviceType g_DeviceType = ERenderDeviceType::Vulkan;

void printHelp()
{
    FString helpText;
    helpText.append("Usage: luma-texture [options]\n");
    helpText.append("Options:\n");
    helpText.append("    --vsync        [on|off]        Set vsync enabled\n");
    helpText.append("    --buffering    [double|triple] Set the buffering method.\n");
    helpText.append("    --graphics-api [vulkan|d3d12]  Set the graphics api for the render device.\n");
    std::cout << helpText << std::endl;
}

bool parseArgs(int argc, const char** argv)
{
    TArray<FStringView> args;
    for (int i = 1; i < argc; ++i)
        args.emplace(FStringView(argv[i]));

    const auto vsyncIt = args.find("--vsync");
    const auto bufferingIt = args.find("--buffering");
    const auto gapiIt = args.find("--graphics-api");
    const auto samplesIt = args.find("--samples");

    if (vsyncIt != -1ull)
    {
        if (vsyncIt == args.count() - 1)
        {
            printHelp();
            return false;
        }

        const auto& value = args[vsyncIt + 1];
        if (value == "on")
            g_Vsync = true;
        else if (value == "off")
            g_Vsync = false;
        else
        {
            printHelp();
            return false;
        }
    }

    if (bufferingIt != -1ull)
    {
        if (bufferingIt == args.count() - 1)
        {
            printHelp();
            return false;
        }

        const auto& value = args[bufferingIt + 1];
        if (value == "double")
            g_Buffering = ESwapchainBuffering::DoubleBuffering;
        else if (value == "triple")
            g_Buffering = ESwapchainBuffering::TripleBuffering;
        else
        {
            printHelp();
            return false;
        }
    }

    if (gapiIt != -1ull)
    {
        if (gapiIt == args.count() - 1)
        {
            printHelp();
            return false;
        }

        const auto& value = args[gapiIt + 1];
        if (value == "vulkan")
            g_DeviceType = ERenderDeviceType::Vulkan;
        else if (value == "d3d12")
            g_DeviceType = ERenderDeviceType::Vulkan;
        else
        {
            printHelp();
            return false;
        }
    }

    if (samplesIt != -1ull)
    {
        if (samplesIt == args.count() - 1)
        {
            printHelp();
            return false;
        }

        const auto& value = args[samplesIt + 1];
        const uint32_t samples = atoi(*value);
        if (samples % 2 != 0 && samples > 16)
        {
            printHelp();
            return false;
        }
        g_Samples = samples;
    }
    return true;
}