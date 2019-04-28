#include "General/SInclude.h"

#include "SJsonMeshReader.h"


SJsonMeshReader::SJsonMeshReader(SJsonValue& rootVal, SSerializedMesh* mesh)
{
    ReadAssetProperty(rootVal, U8Text("asset-property"), &mesh->AssetProperty);
    ReadMeshProperty(rootVal, U8Text("mesh-property"), &mesh->MeshProperty);
    ReadMeshGroups(rootVal, U8Text("mesh-group"), mesh->MeshGroups);
}

void SJsonMeshReader::ReadMeshProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedMeshProperty* data)
{
    assert(parentVal.HasMember(nameVal));
    SJsonValue& meshPropertyVal = parentVal[nameVal];
    assert(meshPropertyVal.IsObject());

    data->HasCol = ReadBool(meshPropertyVal, U8Text("has-col"));
    data->HasNor = ReadBool(meshPropertyVal, U8Text("has-nor"));
    data->HasTan = ReadBool(meshPropertyVal, U8Text("has-tan"));
    data->HasUV = ReadBool(meshPropertyVal, U8Text("has-uv"));
    data->HasUV2 = ReadBool(meshPropertyVal, U8Text("has-uv2"));
    data->HasSkin = ReadBool(meshPropertyVal, U8Text("has-skin"));
    data->MeshGroupCount = ReadUInt32(meshPropertyVal, U8Text("mesh-group-count"));
}

void SJsonMeshReader::ReadMeshGroups(SJsonValue& parentVal, const U8Char* nameVal, vector<SSerializedMeshGroup>& vec)
{
    ReadArray<SSerializedMeshGroup>(parentVal, nameVal, vec,
        [](SJsonValue& val, SSerializedMeshGroup& data)
    {
        data.MaterialSlotName = ReadSString(val, U8Text("material-slot-name"));
        data.MaterialPath = ReadCString(val, U8Text("material-path"));
        data.VertCount = ReadUInt32(val, U8Text("vert-count"));
        data.FaceCount = ReadUInt32(val, U8Text("face-count"));

        ReadVector3Array(val, U8Text("pos"), data.Pos);
        ReadVector3Array(val, U8Text("nor"), data.Nor);
        ReadVector2Array(val, U8Text("uv"), data.UV);
        ReadSkinArray(val, U8Text("skin"), data.Skin);
        ReadFaceArray(val, U8Text("face"), data.Face);
    });
}
