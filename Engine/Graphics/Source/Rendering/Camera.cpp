#include "Luma/Rendering/Camera.h"
#include "Luma/Math/MatrixProjection.hxx"
#include "Luma/Math/MatrixTransform.hxx"

namespace Luma
{
    template<FloatType F>
    const TCamera<F>::Matrix& TCamera<F>::getViewMatrix()
    {
        const auto computeView = [&]()
        {
            Matrix view = Matrix::Identity;
            view = translate(view, -m_Position);
            view = rotate(view, m_Rotation.inverted());
            return view;
        };
        return m_ViewMatrix.get(computeView);
    }

    template<FloatType F>
    const TCamera<F>::Matrix& TCamera<F>::getProjectionMatrix()
    {
        const auto computeProjection = [&]()
        {
            const F aspectRatio = (F)m_Width / m_Height;
            
            const Matrix projection = m_ProjectionMode == ECameraProjectionMode::Perspective ?
                perspective(
                m_FieldOfView,
                aspectRatio,
                m_Near,
                m_Far)

            : orthoCentered(
                (F)m_Width,
                (F)m_Height,
                m_OrthoSize,
                m_Near,
                m_Far);

            return projection;
        };
        
        return m_ProjectionMatrix.get(computeProjection);
    }

    template<FloatType F>
    const TCamera<F>::Matrix& TCamera<F>::getViewProjectionMatrix()
    {
        const auto computeViewProjection = [&]()
        {
            return getProjectionMatrix() *getViewMatrix();
        };
        return m_ViewProjectionMatrix.get(computeViewProjection);
    }

    template<FloatType F>
    void TCamera<F>::setDimensions(const uint32_t width, const uint32_t height)
    {
        m_Width = width;
        m_Height = height;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template<FloatType F>
    void TCamera<F>::setProjectionMode(const ECameraProjectionMode mode)
    {
        m_ProjectionMode = mode;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template<FloatType F>
    void TCamera<F>::setFieldOfView(F fov)
    {
        m_FieldOfView = fov;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template<FloatType F>
    void TCamera<F>::setClipPlanes(F near, F far)
    {
        m_Near = near;
        m_Far = far;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template<FloatType F>
    void TCamera<F>::setOrthographicSize(F orthoSize)
    {
        m_OrthoSize = orthoSize;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template<FloatType F>
    uint32_t TCamera<F>::getWidth() const
    {
        return m_Width;
    }

    template<FloatType F>
    uint32_t TCamera<F>::getHeight() const
    {
        return m_Height;
    }

    template<FloatType F>
    F TCamera<F>::getNearClipPlane() const
    {
        return m_Near;
    }

    template<FloatType F>
    F TCamera<F>::getFarClipPlane() const
    {
        return m_Far;
    }

    template<FloatType F>
    ECameraProjectionMode TCamera<F>::getProjectionMode() const
    {
        return m_ProjectionMode;
    }

    template<FloatType F>
    F TCamera<F>::getOrthographicSize() const
    {
        return m_OrthoSize;
    }
    
    template<FloatType F>
    F TCamera<F>::getFieldOfView() const
    {
        return m_FieldOfView;
    }

    template<FloatType F>
    void TCamera<F>::setPosition(const Vector& position)
    {
        m_Position = position;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template<FloatType F>
    void TCamera<F>::setRotation(const Quat& rotation)
    {
        m_Rotation = rotation;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template<FloatType F>
    void TCamera<F>::setRotation(const AxisAngle& axisAngle)
    {
        m_Rotation = Quat::fromAxisAngle(axisAngle);
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template struct TCamera<float>;
    template struct TCamera<double>;
}
