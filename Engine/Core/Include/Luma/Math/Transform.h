#pragma once
#include "Concepts.h"
#include "Quaternion.h"
#include "MatrixTransform.hxx"
#include "Luma/Containers/Lazy.h"

namespace Luma
{
    template<NumberType T>
    struct TTransform
    {
        TTransform() : m_Position(FVector3<T>::Zero), m_Rotation(TQuat<T>::Identity), m_Scale(FVector3<T>::One){}

        TTransform(const FVector3<T>& position, const TQuat<T>& rotation, const FVector3<T>& scale)
            : m_Position(position), m_Rotation(rotation), m_Scale(scale){}

        FVector3<T> getPosition() const { return m_Position; }
        TQuat<T> getRotation() const { return m_Rotation; }
        FVector3<T> getScale() const { return m_Scale; }


        void setPosition(const FVector3<T>& position)
        {
            m_Position = position;
            m_Matrix.setDirty();
        }

        void setRotation(const TQuat<T>& rotation)
        {
            m_Rotation = rotation;
            m_Matrix.setDirty();
        }

        void setScale(const FVector3<T>& scale)
        {
            m_Scale = scale;
            m_Matrix.setDirty();
        }

        void translate(const FVector3<T>& translation)
        {
            m_Position += translation;
            m_Matrix.setDirty();
        }

        void rotate(const TQuat<T>& rotation)
        {
            m_Rotation *= rotation;
            m_Matrix.setDirty();
        }

        void scale(const FVector3<T>& scale)
        {
            m_Scale *= scale;
            m_Matrix.setDirty();
        }

        void scale(const T scale)
        {
            m_Scale *= scale;
            m_Matrix.setDirty();
        }

        const FMatrix4<T>& getMatrix()
        {
            const auto computeMatrix = [&]() ->  FMatrix4<T>
            {
                FMatrix4<T> matrix;
                matrix = Luma::scale(matrix, m_Scale);
                matrix = Luma::rotate(matrix, m_Rotation);
                matrix = Luma::translate(matrix, m_Position);
                return matrix;
            };
            return m_Matrix.get(computeMatrix);
        }

        const FMatrix4<T>& getInverseMatrix()
        {
            const auto computeMatrix = [&]() -> FMatrix4<T>
            {
                const auto& matrix = getMatrix();
                return matrix.inverted();
            };
            return m_Matrix.get(computeMatrix);
        }

        FVector3<T> getRight() const
        {
            return m_Rotation * FVector3<T>::Right;
        }

        FVector3<T> getUp() const
        {
            return m_Rotation * FVector3<T>::Up;
        }

        FVector3<T> getForward() const
        {
            return m_Rotation * FVector3<T>::Forward;
        }


    private:
        FVector3<T> m_Position;
        TQuat<T> m_Rotation;
        FVector3<T> m_Scale;

        TLazy<FMatrix4<T>> m_Matrix;
        TLazy<FMatrix4<T>> m_InverseMatrix;
    };
}
