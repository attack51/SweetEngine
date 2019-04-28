#include "General/SInclude.h"

#include "SJsonMaterialReader.h"


SJsonMaterialReader::SJsonMaterialReader(SJsonValue& rootVal, SSerializedMaterial* material)
{
    ReadAssetProperty(rootVal, U8Text("asset-property"), &material->AssetProperty);
    ReadMaterialProperty(rootVal, U8Text("material-property"), &material->MaterialProperty);
}

void SJsonMaterialReader::ReadMaterialProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedMaterialProperty* data)
{
    assert(parentVal.HasMember(nameVal));
    SJsonValue& materialPropertyVal = parentVal[nameVal];
    assert(materialPropertyVal.IsObject());

    data->VsPath = ReadCString(materialPropertyVal, U8Text("vs-path"));
    data->FsPath = ReadCString(materialPropertyVal, U8Text("fs-path"));
    data->TexName = ReadCString(materialPropertyVal, U8Text("tex-name"));
}
