#pragma once
#include "Luma/Containers/StringView.h"


namespace Luma
{
    struct File
    {

    };

    class FileSystem
    {
    public:
        virtual bool mount(StringView directory);
        virtual bool mountPak(StringView filepath);


    private:
    };
}
