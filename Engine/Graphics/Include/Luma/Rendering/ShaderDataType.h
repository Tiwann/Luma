#pragma once
#include "Luma/Runtime/Format.h"

namespace Luma
{
    enum class EShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Mat2,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        UInt,
        UInt2,
        UInt3,
        UInt4,
    };

    inline uint32_t getDataTypeSize(const EShaderDataType type)
    {
        switch(type)
        {
        case EShaderDataType::Float: return 1 * 4;
        case EShaderDataType::Float2: return 2 * 4;
        case EShaderDataType::Float3: return 3 * 4;
        case EShaderDataType::Float4: return 4 * 4;
        case EShaderDataType::Mat2: return 2 * 2 * 4;
        case EShaderDataType::Mat3: return 3 * 3 * 4;
        case EShaderDataType::Mat4: return 4 * 4 * 4;
        case EShaderDataType::Int: return  1 * 4;
        case EShaderDataType::Int2: return 2 * 4;
        case EShaderDataType::Int3: return 3 * 4;
        case EShaderDataType::Int4: return 4 * 4;
        case EShaderDataType::UInt: return 1 * 4;
        case EShaderDataType::UInt2: return 2 * 4;
        case EShaderDataType::UInt3: return 3 * 4;
        case EShaderDataType::UInt4: return 4 * 4;
        default: return 0;
        }
    }

    inline EFormat getFormat(const EShaderDataType type)
    {
        switch(type)
        {
        case EShaderDataType::Float: return EFormat::Float;
        case EShaderDataType::Float2: return EFormat::Float2;
        case EShaderDataType::Float3: return EFormat::Float3;
        case EShaderDataType::Float4: return EFormat::Float4;
        case EShaderDataType::Mat2:
        case EShaderDataType::Mat3:
        case EShaderDataType::Mat4: return EFormat::None;
        case EShaderDataType::Int: return EFormat::R32_SINT;
        case EShaderDataType::Int2: return EFormat::R32G32_SINT;
        case EShaderDataType::Int3: return EFormat::R32G32B32_SINT;
        case EShaderDataType::Int4: return EFormat::R32G32B32A32_SINT;
        case EShaderDataType::UInt: return EFormat::R32_UINT;
        case EShaderDataType::UInt2: return EFormat::R32G32_UINT;
        case EShaderDataType::UInt3: return EFormat::R32G32B32_UINT;
        case EShaderDataType::UInt4: return EFormat::R32G32B32A32_UINT;
        default: return EFormat::None;
        }
    }
}
