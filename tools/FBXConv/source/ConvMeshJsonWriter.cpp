#define _CRT_SECURE_NO_WARNINGS
#include "ConvMeshJsonWriter.h"


ConvMeshJsonWriter::ConvMeshJsonWriter(const FbxString& filePath, const ConvMesh* importedMesh)
    : ConvJsonWriter()
{
    ///////////////////////////////////////////////
    //use FileWriteStream
    //FILE* fp = fopen(filePath, "wb"); // non-Windows use "w"
    //
    //char writeBuffer[256];
    //TStream stream(fp, writeBuffer, sizeof(writeBuffer));
    //TEncodedStream encodedStream(stream, false);

    ///////////////////////////////////////////////
    //use ofstream
    TStream stream(filePath.Buffer());
    if (!stream.is_open())
    {
        FBXSDK_printf("\nCould not open file for writing!\n\n");
        return;
    }
    TStreamWrapper streamWrapper(stream);
    TEncodedStream encodedStream(streamWrapper, false);

    //general
    TStreamWriter streamWriter(encodedStream);
    TDocument doc;

    TValue& root = doc.SetObject();

    CreateAssetPropertyNode(importedMesh->AssetProperty.get(), doc, root);
    CreateMeshPropertyNode(importedMesh->MeshProperty.get(), doc, root);
    CreateMeshGroupNodes(importedMesh->MeshGroups, doc, root);

    doc.Accept(streamWriter);

    stream.close();
    //fclose(fp);

    m_success = true;
}

void ConvMeshJsonWriter::CreateMeshPropertyNode(const ConvMeshProperty* meshProperty, TDocument& doc, TValue& root)
{
    TValue meshPropertyNode(rapidjson::kObjectType);

    meshPropertyNode.AddMember(U8_TEXT("has-col"), meshProperty->HasCol, doc.GetAllocator());
    meshPropertyNode.AddMember(U8_TEXT("has-nor"), meshProperty->HasNor, doc.GetAllocator());
    meshPropertyNode.AddMember(U8_TEXT("has-tan"), meshProperty->HasTan, doc.GetAllocator());
    meshPropertyNode.AddMember(U8_TEXT("has-uv"), meshProperty->HasUV, doc.GetAllocator());
    meshPropertyNode.AddMember(U8_TEXT("has-uv2"), meshProperty->HasUV2, doc.GetAllocator());
    meshPropertyNode.AddMember(U8_TEXT("has-skin"), meshProperty->HasSkin, doc.GetAllocator());
    meshPropertyNode.AddMember(U8_TEXT("mesh-group-count"), meshProperty->MeshGroupCount, doc.GetAllocator());

    root.AddMember(U8_TEXT("mesh-property"), meshPropertyNode, doc.GetAllocator());
}

void ConvMeshJsonWriter::CreateMeshGroupNodes(const ConvMeshGroupArray& meshGroups, TDocument& doc, TValue& root)
{
    TValue meshGroupNodes(rapidjson::kArrayType);
    for (const ConvMeshGroupSPtr& meshGroup : meshGroups)
    {
        TValue meshGroupNode(rapidjson::kObjectType);

        TValue materialSlotValue = CreateStringNode(meshGroup->MaterialSlotName, doc);
        TValue materialPathValue = CreateStringNode(meshGroup->MaterialPath, doc);

        meshGroupNode.AddMember(U8_TEXT("material-slot-name"), materialSlotValue, doc.GetAllocator());
        meshGroupNode.AddMember(U8_TEXT("material-path"), materialPathValue, doc.GetAllocator());
        meshGroupNode.AddMember(U8_TEXT("vert-count"), (int32_t)meshGroup->Pos.size(), doc.GetAllocator());
        meshGroupNode.AddMember(U8_TEXT("face-count"), (int32_t)meshGroup->Face.size(), doc.GetAllocator());

        FillVector3Nodes(U8_TEXT("pos"), meshGroupNode, meshGroup->Pos, doc);

        if (meshGroup->Col.size() > 0)
        {
            FillColorNodes(U8_TEXT("col"), meshGroupNode, meshGroup->Col, doc);
        }

        if (meshGroup->Nor.size() > 0)
        {
            FillVector3Nodes(U8_TEXT("nor"), meshGroupNode, meshGroup->Nor, doc);
        }

        if (meshGroup->Tan.size() > 0)
        {
            FillVector3Nodes(U8_TEXT("tan"), meshGroupNode, meshGroup->Tan, doc);
        }

        if (meshGroup->UV.size() > 0)
        {
            FillVector2Nodes(U8_TEXT("uv"), meshGroupNode, meshGroup->UV, doc);
        }

        if (meshGroup->UV2.size() > 0)
        {
            FillVector2Nodes(U8_TEXT("uv2"), meshGroupNode, meshGroup->UV2, doc);
        }

        if (meshGroup->Skin.size() > 0)
        {
            FillSkinNodes(U8_TEXT("skin"), meshGroupNode, meshGroup->Skin, doc);
        }

        if (meshGroup->Face.size() > 0)
        {
            FillFaceNodes(U8_TEXT("face"), meshGroupNode, meshGroup->Face, doc);
        }

        meshGroupNodes.PushBack(meshGroupNode, doc.GetAllocator());
    }
    root.AddMember(U8_TEXT("mesh-group"), meshGroupNodes, doc.GetAllocator());
}

void ConvMeshJsonWriter::FillFaceNodes(SourceStrRef name, TValue& parent, const FaceArray& faces, TDocument& doc)
{
    TValue nodes(rapidjson::kArrayType);

    for (const uint16_t& face : faces)
    {
        TValue node;
        node.SetUint(face);

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}
