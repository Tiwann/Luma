#include "Luma/Rendering/Camera.h"
#include "Luma/Math/MatrixProjection.hxx"
#include "Luma/Math/MatrixTransform.hxx"

namespace Luma
{
    template<FloatType F>
    const TCamera<F>::Matrix& TCamera<F>::getViewMatrix() const
    {
        const auto computeView = [&]()
        {
            Matrix view = Matrix::Identity;
            view = translate(view, -m_Position);
            view = rotate(view, m_Rotation.inverted());
            return view;
        };
        TCamera& thisCamera = const_cast<TCamera&>(*this);
        return thisCamera.m_ViewMatrix.get(computeView);
    }

    template<FloatType F>
    const TCamera<F>::Matrix& TCamera<F>::getProjectionMatrix() const
    {
        const auto computeProjection = [&]() -> Matrix
        {
            const F aspectRatio = (F)m_Width / m_Height;

            Matrix projection = m_ProjectionMode == ECameraProjectionMode::Perspective ?
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

        TCamera& thisCamera = const_cast<TCamera&>(*this);
        return thisCamera.m_ProjectionMatrix.get(computeProjection);
    }

    template<FloatType F>
    const TCamera<F>::Matrix& TCamera<F>::getViewProjectionMatrix() const
    {
        const auto computeViewProjection = [&]()
        {
            return getProjectionMatrix() * getViewMatrix();
        };

        TCamera& thisCamera = const_cast<TCamera&>(*this);
        return thisCamera.m_ViewProjectionMatrix.get(computeViewProjection);
    }

    template <FloatType F>
    void TCamera<F>::setWidth(const uint32_t width)
    {
        m_Width = width;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template <FloatType F>
    void TCamera<F>::setHeight(const uint32_t height)
    {
        m_Height = height;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    template<FloatType F>
    void TCamera<F>::setSize(const uint32_t width, const uint32_t height)
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

    template <FloatType F>
    FRect2u TCamera<F>::getBounds() const
    {
        return {0, 0, m_Width, m_Height};
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

    template <FloatType F>
    FVector2<F> TCamera<F>::worldToScreen(const FVector3<F>& worldPos) const
    {
        FVector4<F> localPos = getViewMatrix() * FVector4<F>(worldPos, 1.0);
        FVector4<F> clipPos = getProjectionMatrix() * localPos;

        if (clipPos.w != 0.0f)
        {
            clipPos.x /= clipPos.w;
            clipPos.y /= clipPos.w;
            clipPos.z /= clipPos.w;
        }

        F screenX = (clipPos.x * 0.5 + 0.5) * m_Width;
        F screenY = (1.0 - (clipPos.y * 0.5 + 0.5)) * m_Height;
        return FVector2<F>(screenX, screenY);
    }

    template struct TCamera<float>;
    template struct TCamera<double>;
}
