#include <Luma/Audio.h>
#include <Luma/Graphics.h>
#include <cstdlib>

int main()
{
    const Luma::FAudioDevice* audioDevice = Luma::createAudioDevice(Luma::FAudioDeviceDesc());
    if (!audioDevice) return EXIT_FAILURE;

    constexpr auto renderDeviceDesc = Luma::FRenderDeviceDesc(Luma::ERenderDeviceType::WebGPU);
    const Luma::IRenderDevice* renderDevice = Luma::createRenderDevice(renderDeviceDesc);
    if (renderDevice) return EXIT_FAILURE;


    return 0;
}
