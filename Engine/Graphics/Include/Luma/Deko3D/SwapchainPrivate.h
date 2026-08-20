#pragma once
#include <deko3d.h>

namespace Luma::Deko3d
{
    struct FSwapchainImpl::Private
    {
        DkImage images[3]{};
        DkImageView imageViews[3]{};
        DkMemBlock imageMemBlock = nullptr;
    };
}