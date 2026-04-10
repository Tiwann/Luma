#include "Luma/Rendering/Camera.h"

namespace luma
{
    const FMatrix4f& FCamera::getViewMatrix()
    {
        const auto computeView = [&]() -> FMatrix4f
        {
            const Transform* transform = GetTransform();
            Matrix4 view = Matrix4::Identity;
            view.Translate(-transform->GetPosition());
            view.RotateDegrees(-transform->GetRotation().ToEulerDegrees());
            return view;
        };

        return m_ViewMatrix.Get(computeView);
    }

    const FMatrix4f& FCamera::getProjectionMatrix()
    {
    }

    const FMatrix4f& FCamera::getViewProjectionMatrix()
    {
    }

    void FCamera::setDimensions(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    void FCamera::setProjectionMode(CameraProjectionMode mode)
    {
        m_ProjectionMode = mode;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    void FCamera::setFieldOfView(float fov)
    {
        m_FieldOfView = fov;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    void FCamera::setClipPlanes(float near, float far)
    {
        m_Near = near;
        m_Far = far;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    void FCamera::setOrthographicSize(float orthoSize)
    {
        m_OrthoSize = orthoSize;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    uint32_t FCamera::getWidth() const
    {
        return m_Width;
    }

    uint32_t FCamera::getHeight() const
    {
        return m_Height;
    }

    float FCamera::getNearClipPlane() const
    {
        return m_Near;
    }

    float FCamera::getFarClipPlane() const
    {
        return m_Far;
    }

    CameraProjectionMode FCamera::getProjectionMode() const
    {
        return m_ProjectionMode;
    }

    float FCamera::getOrthographicSize() const
    {
        return m_OrthoSize;
    }

    float FCamera::getFieldOfView() const
    {
        return m_FieldOfView;
    }

    void FCamera::setPosition(const FVector3f& position)
    {
        m_Position = position;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    void FCamera::setRotation(const FQuatf& rotation)
    {
        m_Rotation = rotation;
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }

    void FCamera::setRotation(const FAxisAngle2f& axisAngle)
    {
        m_Rotation = FQuatf::fromAxisAngle(axisAngle);
        m_ProjectionMatrix.setDirty();
        m_ViewProjectionMatrix.setDirty();
    }
}
