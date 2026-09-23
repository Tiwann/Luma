#pragma once
#include "Luma/Containers/Lazy.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Quaternion.h"
#include "Luma/Math/AxisAngle.h"
#include "Luma/Math/Matrix4.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include <cstdint>


namespace Luma
{
    enum class ECameraProjectionMode
    {
        Orthographic,
        Perspective
    };

    template<FloatType F>
    struct TCamera
    {
        using Matrix = FMatrix4<F>;
        using Vector = FVector3<F>;
        using Quat = TQuat<F>;
        using AxisAngle = TAxisAngle<F>;

        TCamera() = default;

        const Matrix& getViewMatrix() const;
        const Matrix& getProjectionMatrix() const;
        const Matrix& getViewProjectionMatrix() const;

        void setWidth(uint32_t width);
        void setHeight(uint32_t height);
        void setSize(uint32_t width, uint32_t height);
        void setProjectionMode(ECameraProjectionMode mode);
        void setFieldOfView(F fov);
        void setClipPlanes(F near, F far);
        void setOrthographicSize(F orthoSize);

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        FRect2u getBounds() const;
        F getNearClipPlane() const;
        F getFarClipPlane() const;
        ECameraProjectionMode getProjectionMode() const;
        F getOrthographicSize() const;
        F getFieldOfView() const;

        void setPosition(const Vector& position);
        void setRotation(const Quat& rotation);
        void setRotation(const AxisAngle& axisAngle);

        FVector2<F> worldToScreen(const FVector3<F>& worldPos) const;
    private:
        uint32_t m_Width = 0, m_Height = 0;
        ECameraProjectionMode m_ProjectionMode = ECameraProjectionMode::Perspective;
        F m_FieldOfView = F(45.0);
        F m_Near = F(0.01);
        F m_Far = F(1000.0);
        F m_OrthoSize = F(100.0);
        TVector<F, 2> m_ClipSpaceOffset = TVector<F, 2>::Zero;

        Vector m_Position = Vector::Zero;
        Quat m_Rotation = Quat::Identity;

        TLazy<Matrix> m_ViewMatrix{};
        TLazy<Matrix> m_ProjectionMatrix{};
        TLazy<Matrix> m_ViewProjectionMatrix{};
    };

    using FCamera = TCamera<float>;
}

