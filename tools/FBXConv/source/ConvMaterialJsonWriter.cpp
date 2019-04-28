#define _CRT_SECURE_NO_WARNINGS
#include "ConvMaterialJsonWriter.h"


ConvMaterialJsonWriter::ConvMaterialJsonWriter(const FbxString& filePath, const ConvMaterial* importedMaterial)
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

    CreateAssetPropertyNode(importedMaterial->AssetProperty.get(), doc, root);
    CreateMaterialPropertyNode(importedMaterial->MaterialProperty.get(), doc, root);

    doc.Accept(streamWriter);

    stream.close();
    //fclose(fp);

    m_success = true;
}

void ConvMaterialJsonWriter::CreateMaterialPropertyNode(const ConvMaterialProperty* materialProperty, TDocument& doc, TValue& root)
{
    TValue materialPropertyNode(rapidjson::kObjectType);

    materialPropertyNode.AddMember(U8_TEXT("vs-path"), CreateStringNode(materialProperty->VsPath, doc), doc.GetAllocator());
    materialPropertyNode.AddMember(U8_TEXT("fs-path"), CreateStringNode(materialProperty->FsPath, doc), doc.GetAllocator());
    materialPropertyNode.AddMember(U8_TEXT("tex-name"), CreateStringNode(materialProperty->TexName, doc), doc.GetAllocator());

    root.AddMember(U8_TEXT("material-property"), materialPropertyNode, doc.GetAllocator());
}
