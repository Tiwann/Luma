#pragma once


namespace Luma
{
    enum class EPresentMode
    {
        Unknown = 0,
        Immediate,
        Mailbox,
        Fifo,
        FifoRelaxed
    };
}