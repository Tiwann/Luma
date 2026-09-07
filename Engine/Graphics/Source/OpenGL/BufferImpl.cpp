#include "Luma/OpenGL/BufferImpl.h"
#include "Luma/OpenGL/GpuDeviceImpl.h"
#include <glad/glad.h>

namespace Luma::OpenGL
{
    bool FBufferImpl::initialize(const FBufferDesc& bufferDesc)
    {
        if (!bufferDesc.device) return false;
        if (bufferDesc.size <= 0) return false;
        if (bufferDesc.usage == EBufferUsage::None) return false;

        if (isHandleValid(m_Handle))
            glDeleteBuffers(1, &m_Handle);

        glCreateBuffers(1, &m_Handle);
        glNamedBufferStorage(m_Handle, bufferDesc.size, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        FGpuDeviceImpl* device = static_cast<FGpuDeviceImpl*>(bufferDesc.device);

        m_Device = device;
        m_Size = bufferDesc.size;
        m_Usage = bufferDesc.usage;
        return true;
    }

    void FBufferImpl::destroy()
    {
        glDeleteBuffers(1, &m_Handle);
    }

    void* FBufferImpl::map()
    {
        void* mapped = glMapNamedBuffer(m_Handle, GL_READ_WRITE);
        return mapped;
    }

    void FBufferImpl::unmap(const void* ptr)
    {
        (void)ptr;
        glUnmapNamedBuffer(m_Handle);
    }

    uint64_t FBufferImpl::getDeviceAddress() const
    {

    }
}
