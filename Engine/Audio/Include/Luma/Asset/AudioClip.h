#pragma once
#include "Luma/Containers/StringView.h"
#include "Luma/Runtime/Asset.h"
#include "Luma/Audio/Export.h"

struct ma_sound;
struct ma_decoder;

namespace Luma
{
    class LUMA_AUDIO_API FAudioClip final : public IAsset
    {
    public:
        FAudioClip();
        ~FAudioClip() override;

        bool loadFromFile(FStringView filepath);
        bool loadFromMemory(const void* data, size_t size);
        void destroy() override;

        EAssetType getAssetType() const override;

        float getDurationSeconds() const;
        uint64_t getDurationFrames() const;

        ma_sound* getHandle() const { return m_Handle; }
    private:
        ma_sound* m_Handle = nullptr;
        ma_decoder* m_Decoder = nullptr;
    };
}
