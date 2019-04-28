//General Include
#include "General/SHeader.h"

#include "General/Json/DOM/SJsonReader.h"
#include "General/Serialized/SSerializedMaterial.h"


class SJsonMaterialReader : public SJsonReader
{
public:
    SJsonMaterialReader(SJsonValue& rootVal, SSerializedMaterial* material);

protected:
    static void ReadMaterialProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedMaterialProperty* data);
};
