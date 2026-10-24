#pragma once
#include "Luma/Containers/HashMap.h"
#include "Luma/Containers/String.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Asset.h"

namespace Luma
{
    class AssetDatabase
    {
    public:
        template<typename T>
        Ref<T> createAsset(const String& name)
        {
            m_Assets[name] = Ref<T>::create();
            return m_Assets[name];
        }

        void addAsset(Ref<Asset> asset, const String& name)
        {
            m_Assets[name] = std::move(asset);
        }

        bool unloadAsset(const String& name)
        {
            if (!m_Assets.find(name)) return false;
            m_Assets[name]->destroy();
            m_Assets.remove(name);
            return true;
        }

        void unloadAll()
        {
            for (auto& [name, asset] : m_Assets)
                unloadAsset(name);
            m_Assets.free();
        }
    private:
        HashMap<String, Ref<Asset>> m_Assets;
    };
}
