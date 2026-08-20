#pragma once


namespace dk::detail
{
    struct Device;
    struct Swapchain;
    struct MemBlock;
    struct Queue;
    struct CmdBuf;
}

typedef dk::detail::Device* DkDevice;
typedef dk::detail::Swapchain* DkSwapchain;
typedef dk::detail::MemBlock* DkMemBlock;
typedef dk::detail::Queue* DkQueue;
typedef dk::detail::CmdBuf* DkCmdBuf;
struct DkFence;
struct DkImage;
struct DkImageView;

typedef uint32_t DkBufUsage;
typedef uintptr_t DkCmdList;
