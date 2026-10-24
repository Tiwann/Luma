#pragma once
#include "Luma/Containers/ArrayView.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/Ref.h"
#include "Luma/Runtime/Asset.h"

namespace Luma
{
    struct AssetImporter
    {
        virtual ~AssetImporter() = default;

        virtual Array<StringView> getSupportedExtensions() const = 0;
        virtual AssetType getAssetType() const = 0;

        virtual Ref<Asset> loadAsset(ArrayView<uint8_t> data) const = 0;
    };

    class AssetImporterRegistry
    {
    public:
        template<typename T>
        void registerAssetImporter()
        {
            for (AssetImporter* importer : m_Importers)
            {
                if (dynamic_cast<T*>(importer))
                    return;
            }

            AssetImporter* importer = new T();
            m_Importers.addUnique(importer);
        }

        template<typename T>
        void unregisterAssetImporter()
        {
            for (AssetImporter* importer : m_Importers)
            {
                if (AssetImporter* registred = dynamic_cast<T*>(importer))
                {
                    m_Importers.remove(registred);
                    delete registred;
                }
            }
        }

        void unregisterAllImporters()
        {
            for (AssetImporter* importer : m_Importers)
                delete importer;
            m_Importers.free();
        }
    private:
        Array<AssetImporter*> m_Importers;
    };
}
