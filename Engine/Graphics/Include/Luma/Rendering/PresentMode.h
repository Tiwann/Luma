#pragma once


namespace luma
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