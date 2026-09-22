#pragma once
#include "Stream.h"
#include "Luma/Math/Vector.h"
#include "Luma/Math/Matrix.h"
#include <cstdint>

namespace Luma
{
    template<typename T>
    struct ISerializer
    {
        virtual ~ISerializer() = default;
        virtual void serialize(IStream& stream, const T& inData) = 0;
        virtual void deserialize(IStream& stream, T& outData) = 0;
    };

    template<NumberType T, uint32_t N>
    struct TVectorSerializer : ISerializer<TVector<T, N>>
    {
        void serialize(IStream& stream, const TVector<T, N>& inData) override
        {
            for (uint32_t i = 0; i < N; i++)
                stream.write(inData[i]);
        }

        void deserialize(IStream& stream, TVector<T, N>& outData) override
        {
            for (uint32_t i = 0; i < N; i++)
                stream.read(outData[i]);
        }
    };

    template<NumberType T, uint32_t I, uint32_t J>
    struct TMatrixSerializer : ISerializer<TMatrix<T, I, J>>
    {
        void serialize(IStream& stream, const TMatrix<T, I, J>& inData) override
        {
            const T* data = reinterpret_cast<const T*>(&inData);
            for (uint32_t n = 0; n < I * J; n++)
                stream.write(data[n]);
        }

        void deserialize(IStream& stream, TMatrix<T, I, J>& outData) override
        {
            T* data = reinterpret_cast<T*>(&outData);
            for (uint32_t n = 0; n < I * J; n++)
                stream.read(data[n]);
        }
    };
}
