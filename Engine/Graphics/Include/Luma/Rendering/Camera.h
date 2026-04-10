#pragma once
#include "Luma/Math/Matrix.h"
#include "Luma/Containers/Lazy.h"
#include "Luma/Math/Vector3.h"
#include "../../../../Core/Include/Luma/Math/Quaternion.h"
#include "../../../../Core/Include/Luma/Math/AxisAngle.h"
#include <cstdint>

namespace luma
{
    enum class CameraProjectionMode
    {
        Orthographic,
        Perspective
    };

    struct FCamera
    {
        explicit FCamera() = default;

        const FMatrix4f& getViewMatrix();
        const FMatrix4f& getProjectionMatrix();
        const FMatrix4f& getViewProjectionMatrix();

        void setDimensions(uint32_t width, uint32_t height);
        void setProjectionMode(CameraProjectionMode mode);
        void setFieldOfView(float fov);
        void setClipPlanes(float near, float far);
        void setOrthographicSize(float orthoSize);

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        float getNearClipPlane() const;
        float getFarClipPlane() const;
        CameraProjectionMode getProjectionMode() const;
        float getOrthographicSize() const;
        float getFieldOfView() const;

        void setPosition(const FVector3f& position);
        void setRotation(const FQuatf& rotation);
        void setRotation(const FAxisAngle2f& axisAngle);
    private:
        uint32_t m_Width = 0, m_Height = 0;
        CameraProjectionMode m_ProjectionMode = CameraProjectionMode::Perspective;
        float m_FieldOfView = 45.0f;
        float m_Near = 0.01f;
        float m_Far = 10000.0f;
        float m_OrthoSize = 1.0f;

        FVector3f m_Position = FVector3f::Zero;
        FQuatf m_Rotation = FQuatf::Identity;

        TLazy<FMatrix4f> m_ViewMatrix;
        TLazy<FMatrix4f> m_ProjectionMatrix;
        TLazy<FMatrix4f> m_ViewProjectionMatrix;
    };
}

