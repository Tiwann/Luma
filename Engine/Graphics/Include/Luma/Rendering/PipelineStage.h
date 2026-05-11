#pragma once
#include "Luma/Runtime/Flags.h"

namespace Luma
{
    enum class EPipelineStageBits
    {
        None = 0,
        TopOfPipe = 0x00000001,
        DrawIndirect = 0x00000002,
        VertexInput = 0x00000004,
        VertexShader = 0x00000008,
        TesselationControlShader = 0x00000010,
        TesselationEvalShader = 0x00000020,
        GeometryShader = 0x00000040,
        FragmentShader = 0x00000080,
        EarlyFragmentTests = 0x00000100,
        LateFragmentTests = 0x00000200,
        ColorTargetOutput = 0x00000400,
        ComputeShader = 0x00000800,
        Copy = 0x00001000,
        BottomOfPipe = 0x00002000,
        Host = 0x00004000,
        AllRender = 0x00008000,
        AllCommands = 0x00010000,
        RayTracingShader = 0x00200000,
        TaskShader = 0x00080000,
        MeshShader = 0x00100000,
    };

    using FPipelineStageFlags = TFlags<EPipelineStageBits>;
}
