#pragma once

#include "../AssetImporter.hpp"
#include <FishEngine/Prefab.hpp>

namespace FishEditor
{
    class NativeFormatImporter : public AssetImporter
    {
    public:
        InjectClassName(NativeFormatImporter, 1034);

        NativeFormatImporter() : AssetImporter(ClassID, ClassName)
        {
        }
		
        FishEngine::Object* Import();
    };
}
