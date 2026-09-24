#pragma once
#include "Stream.h"
#include "Luma/Math/Vector.h"
#include "Luma/Math/Matrix.h"
#include <cstdint>

namespace Luma
{
    template<typename T>
    struct Serializer
    {
        virtual ~Serializer() = default;
        virtual void serialize(Stream& stream, const T& inData) = 0;
        virtual void deserialize(Stream& stream, T& outData) = 0;
    };

    template<NumberType T, uint32_t N>
    struct VectorSerializer : Serializer<TVector<T, N>>
    {
        void serialize(Stream& stream, const TVector<T, N>& inData) override
        {
            for (uint32_t i = 0; i < N; i++)
                stream.write(inData[i]);
        }

        void deserialize(Stream& stream, TVector<T, N>& outData) override
        {
            for (uint32_t i = 0; i < N; i++)
                stream.read(outData[i]);
        }
    };

    template<NumberType T, uint32_t I, uint32_t J>
    struct MatrixSerializer : Serializer<TMatrix<T, I, J>>
    {
        void serialize(Stream& stream, const TMatrix<T, I, J>& inData) override
        {
            const T* data = reinterpret_cast<const T*>(&inData);
            for (uint32_t n = 0; n < I * J; n++)
                stream.write(data[n]);
        }

        void deserialize(Stream& stream, TMatrix<T, I, J>& outData) override
        {
            T* data = reinterpret_cast<T*>(&outData);
            for (uint32_t n = 0; n < I * J; n++)
                stream.read(data[n]);
        }
    };
}
