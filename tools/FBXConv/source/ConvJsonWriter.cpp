#define _CRT_SECURE_NO_WARNINGS
#include "ConvJsonWriter.h"


ConvJsonWriter::ConvJsonWriter()
    : m_success(false)
{
}

void ConvJsonWriter::CreateAssetPropertyNode(const ConvAssetProperty* assetProperty, TDocument& doc, TValue& root)
{
    TValue assetPropertyNode(rapidjson::kObjectType);

    assetPropertyNode.AddMember(U8_TEXT("identifier"), CreateStringNode(assetProperty->Identifier, doc), doc.GetAllocator());
    assetPropertyNode.AddMember(U8_TEXT("asset-type"), CreateStringNode(assetProperty->AssetType, doc), doc.GetAllocator());

    root.AddMember(U8_TEXT("asset-property"), assetPropertyNode, doc.GetAllocator());
}

TValue ConvJsonWriter::CreateStringNode(const char* str, TDocument& doc)
{
    TValue node;
    node.SetString(str, doc.GetAllocator());
    return node;
}

TValue ConvJsonWriter::CreateStringNode(const FbxString& str, TDocument& doc)
{
    return CreateStringNode(str.Buffer(), doc);
}

// Matrix4x4Column
TValue ConvJsonWriter::CreateMatrix4x4ColumnNode(const FbxAMatrix& mat, TDocument& doc)
{
    char strBuffer[256];
 
    FbxVector4 col0 = mat.GetColumn(0);
    FbxVector4 col1 = mat.GetColumn(1);
    FbxVector4 col2 = mat.GetColumn(2);
    FbxVector4 col3 = mat.GetColumn(3);
    
    sprintf(strBuffer, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g", 
        (float)col0[0], (float)col0[1], (float)col0[2], (float)col0[3],
        (float)col1[0], (float)col1[1], (float)col1[2], (float)col1[3],
        (float)col2[0], (float)col2[1], (float)col2[2], (float)col2[3],
        (float)col3[0], (float)col3[1], (float)col3[2], (float)col3[3]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}

void ConvJsonWriter::FillMatrix4x4ColumnNodes(SourceStrRef name, TValue& parent, const AMatrixArray& mats, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxAMatrix& mat : mats)
    {
        FbxVector4 col0 = mat.GetColumn(0);
        FbxVector4 col1 = mat.GetColumn(1);
        FbxVector4 col2 = mat.GetColumn(2);
        FbxVector4 col3 = mat.GetColumn(3);

        sprintf(strBuffer, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",
            (float)col0[0], (float)col0[1], (float)col0[2], (float)col0[3],
            (float)col1[0], (float)col1[1], (float)col1[2], (float)col1[3],
            (float)col2[0], (float)col2[1], (float)col2[2], (float)col2[3],
            (float)col3[0], (float)col3[1], (float)col3[2], (float)col3[3]);

        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Matrix3x4Column
TValue ConvJsonWriter::CreateMatrix3x4ColumnNode(const FbxAMatrix& mat, TDocument& doc)
{
    char strBuffer[256];

    FbxVector4 col0 = mat.GetColumn(0);
    FbxVector4 col1 = mat.GetColumn(1);
    FbxVector4 col2 = mat.GetColumn(2);

    sprintf(strBuffer, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",
        (float)col0[0], (float)col0[1], (float)col0[2], (float)col0[3],
        (float)col1[0], (float)col1[1], (float)col1[2], (float)col1[3],
        (float)col2[0], (float)col2[1], (float)col2[2], (float)col2[3]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}

void ConvJsonWriter::FillMatrix3x4ColumnNodes(SourceStrRef name, TValue& parent, const AMatrixArray& mats, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxAMatrix& mat : mats)
    {
        FbxVector4 col0 = mat.GetColumn(0);
        FbxVector4 col1 = mat.GetColumn(1);
        FbxVector4 col2 = mat.GetColumn(2);

        sprintf(strBuffer, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",
            (float)col0[0], (float)col0[1], (float)col0[2], (float)col0[3],
            (float)col1[0], (float)col1[1], (float)col1[2], (float)col1[3],
            (float)col2[0], (float)col2[1], (float)col2[2], (float)col2[3]);

        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Matrix4x4Row
TValue ConvJsonWriter::CreateMatrix4x4RowNode(const FbxAMatrix& mat, TDocument& doc)
{
    char strBuffer[256];

    FbxVector4 row0 = mat.GetRow(0);
    FbxVector4 row1 = mat.GetRow(1);
    FbxVector4 row2 = mat.GetRow(2);
    FbxVector4 row3 = mat.GetRow(3);

    sprintf(strBuffer, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",
        (float)row0[0], (float)row0[1], (float)row0[2], (float)row0[3],
        (float)row1[0], (float)row1[1], (float)row1[2], (float)row1[3],
        (float)row2[0], (float)row2[1], (float)row2[2], (float)row2[3],
        (float)row3[0], (float)row3[1], (float)row3[2], (float)row3[3]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}

void ConvJsonWriter::FillMatrix4x4RowNodes(SourceStrRef name, TValue& parent, const AMatrixArray& mats, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxAMatrix& mat : mats)
    {
        FbxVector4 row0 = mat.GetRow(0);
        FbxVector4 row1 = mat.GetRow(1);
        FbxVector4 row2 = mat.GetRow(2);
        FbxVector4 row3 = mat.GetRow(3);

        sprintf(strBuffer, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",
            (float)row0[0], (float)row0[1], (float)row0[2], (float)row0[3],
            (float)row1[0], (float)row1[1], (float)row1[2], (float)row1[3],
            (float)row2[0], (float)row2[1], (float)row2[2], (float)row2[3],
            (float)row3[0], (float)row3[1], (float)row3[2], (float)row3[3]);

        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Matrix3x4Row
TValue ConvJsonWriter::CreateMatrix3x4RowNode(const FbxAMatrix& mat, TDocument& doc)
{
    char strBuffer[256];

    FbxVector4 row0 = mat.GetRow(0);
    FbxVector4 row1 = mat.GetRow(1);
    FbxVector4 row2 = mat.GetRow(2);
    FbxVector4 row3 = mat.GetRow(3);

    sprintf(strBuffer, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",
        (float)row0[0], (float)row0[1], (float)row0[2],
        (float)row1[0], (float)row1[1], (float)row1[2],
        (float)row2[0], (float)row2[1], (float)row2[2],
        (float)row3[0], (float)row3[1], (float)row3[2]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}

void ConvJsonWriter::FillMatrix3x4RowNodes(SourceStrRef name, TValue& parent, const AMatrixArray& mats, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxAMatrix& mat : mats)
    {
        FbxVector4 row0 = mat.GetRow(0);
        FbxVector4 row1 = mat.GetRow(1);
        FbxVector4 row2 = mat.GetRow(2);
        FbxVector4 row3 = mat.GetRow(3);

        sprintf(strBuffer, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g",
            (float)row0[0], (float)row0[1], (float)row0[2],
            (float)row1[0], (float)row1[1], (float)row1[2],
            (float)row2[0], (float)row2[1], (float)row2[2],
            (float)row3[0], (float)row3[1], (float)row3[2]);

        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Color
TValue ConvJsonWriter::CreateColorNode(const FbxColor& col, TDocument& doc)
{
    char strBuffer[256];

    sprintf(strBuffer, "%g,%g,%g,%g", (float)col[0], (float)col[1], (float)col[2], (float)col[3]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}

void ConvJsonWriter::FillColorNodes(SourceStrRef name, TValue& parent, const ColArray& vecs, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxColor& col : vecs)
    {
        sprintf(strBuffer, "%g,%g,%g,%g", (float)col[0], (float)col[1], (float)col[2], (float)col[3]);

        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Vector2
TValue ConvJsonWriter::CreateVector2Node(const FbxVector2& vec, TDocument& doc)
{
    char strBuffer[256];

    sprintf(strBuffer, "%g,%g", (float)vec[0], (float)vec[1]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}


void ConvJsonWriter::FillVector2Nodes(SourceStrRef name, TValue& parent, const Vec2Array& vecs, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxVector2& vec : vecs)
    {
        sprintf(strBuffer, "%g,%g", (float)vec[0], (float)vec[1]);

        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Vector3
TValue ConvJsonWriter::CreateVector3Node(const FbxVector4& vec, TDocument& doc)
{
    char strBuffer[256];

    sprintf(strBuffer, "%g,%g,%g", (float)vec[0], (float)vec[1], (float)vec[2]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}

void ConvJsonWriter::FillVector3Nodes(SourceStrRef name, TValue& parent, const Vec4Array& vecs, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxVector4& vec : vecs)
    {
        sprintf(strBuffer, "%g,%g,%g", (float)vec[0], (float)vec[1], (float)vec[2]);

        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Vector 4
TValue ConvJsonWriter::CreateVector4Node(const FbxVector4& vec, TDocument& doc)
{
    char strBuffer[256];

    sprintf(strBuffer, "%g,%g,%g,%g", (float)vec[0], (float)vec[1], (float)vec[2], (float)vec[3]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}

void ConvJsonWriter::FillVector4Nodes(SourceStrRef name, TValue& parent, const Vec4Array& vecs, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxVector4& vec : vecs)
    {
        sprintf(strBuffer, "%g,%g,%g,%g", (float)vec[0], (float)vec[1], (float)vec[2], (float)vec[3]);
        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Quaternion
TValue ConvJsonWriter::CreateQuaternionNode(const FbxQuaternion& quat, TDocument& doc)
{
    char strBuffer[256];

    sprintf(strBuffer, "%g,%g,%g,%g", (float)quat[0], (float)quat[1], (float)quat[2], (float)quat[3]);

    TValue node;
    node.SetString(strBuffer, doc.GetAllocator());
    return node;
}

void ConvJsonWriter::FillQuaternionNodes(SourceStrRef name, TValue& parent, const QuatArray& quats, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const FbxQuaternion& quat : quats)
    {
        sprintf(strBuffer, "%g,%g,%g,%g", (float)quat[0], (float)quat[1], (float)quat[2], (float)quat[3]);
        
        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());
}

// Skin
void ConvJsonWriter::FillSkinNodes(SourceStrRef name, TValue& parent, const SkinArray& vecs, TDocument& doc)
{
    char strBuffer[256];
    TValue nodes(rapidjson::kArrayType);

    for (const ConvSkin& vec : vecs)
    {
        //todo : SKIN_INDEX_COUNT, SKIN_WEIGHT_COUNT 값에따라 자동으로 변경 가능하면 좋겠음
        sprintf(strBuffer, "%d,%d,%d,%g,%g,%g", 
            vec.Indices[0], vec.Indices[1], vec.Indices[2],
            vec.Weights[0], vec.Weights[1], vec.Weights[2]);

        TValue node;
        node.SetString(strBuffer, doc.GetAllocator());

        nodes.PushBack(node, doc.GetAllocator());
    }
    parent.AddMember(name, nodes, doc.GetAllocator());

}
