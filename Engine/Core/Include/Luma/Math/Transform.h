#pragma once
#include "Concepts.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "MatrixTransform.hxx"
#include "Luma/Containers/Lazy.h"

namespace Luma
{
    template<NumberType T>
    struct TTransform
    {
        using Vector = FVector3<T>;
        using Matrix = FMatrix4<T>;
        using Quat = TQuat<T>;

        TTransform() : m_Position(Vector::Zero), m_Rotation(Quat::Identity), m_Scale(Vector::One){}

        TTransform(const Vector& position, const Quat& rotation, const Vector& scale)
            : m_Position(position), m_Rotation(rotation), m_Scale(scale){}

        Vector getPosition() const { return m_Position; }
        Quat getRotation() const { return m_Rotation; }
        Vector getScale() const { return m_Scale; }


        void setPosition(const Vector& position)
        {
            m_Position = position;
            m_Matrix.setDirty();
        }

        void setRotation(const Quat& rotation)
        {
            m_Rotation = rotation;
            m_Matrix.setDirty();
        }

        void setScale(const Vector& scale)
        {
            m_Scale = scale;
            m_Matrix.setDirty();
        }

        void translate(const Vector& translation)
        {
            m_Position += translation;
            m_Matrix.setDirty();
        }

        void rotate(const Quat& rotation)
        {
            m_Rotation *= rotation;
            m_Matrix.setDirty();
        }

        void scale(const Vector& scale)
        {
            m_Scale *= scale;
            m_Matrix.setDirty();
        }

        void scale(const T scale)
        {
            m_Scale *= scale;
            m_Matrix.setDirty();
        }

        const Matrix& getMatrix()
        {
            const auto computeMatrix = [&]() -> Matrix
            {
                Matrix matrix;
                matrix = Luma::scale(matrix, m_Scale);
                matrix = Luma::rotate(matrix, m_Rotation);
                matrix = Luma::translate(matrix, m_Position);
                return matrix;
            };
            return m_Matrix.get(computeMatrix);
        }

        Vector getRight() const
        {
            return m_Rotation * Vector::Right;
        }

        Vector getUp() const
        {
            return m_Rotation * Vector::Up;
        }

        Vector getForward() const
        {
            return m_Rotation * Vector::Forward;
        }


    private:
        Vector m_Position;
        Quat m_Rotation;
        Vector m_Scale;

        TLazy<Matrix> m_Matrix;
        TLazy<Matrix> m_InverseMatrix;
    };
}
