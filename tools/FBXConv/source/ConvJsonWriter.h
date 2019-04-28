#pragma once

//rapid json
#include "rapidjson.h"
#include "filewritestream.h" 
#include "ostreamwrapper.h"
#include "writer.h"
#include "document.h"

//Conv Include
#include "ConvData.h"


typedef char SourceChar;
typedef char TargetChar;

typedef rapidjson::UTF8<SourceChar> SourceEncoding;
typedef rapidjson::UTF8<TargetChar> TargetEncoding;

typedef rapidjson::GenericStringRef<SourceChar> SourceStrRef;

//use FileWriteStream
//typedef rapidjson::FileWriteStream TStream;
//typedef rapidjson::EncodedOutputStream<TargetEncoding, TStream> TEncodedStream;

//use ofstream
typedef std::ofstream TStream;
typedef rapidjson::OStreamWrapper TStreamWrapper;
typedef rapidjson::EncodedOutputStream<TargetEncoding, TStreamWrapper> TEncodedStream;

typedef rapidjson::Writer<TEncodedStream, SourceEncoding, TargetEncoding> TStreamWriter;
typedef rapidjson::GenericDocument<SourceEncoding> TDocument;
typedef rapidjson::GenericValue<SourceEncoding> TValue;


class ConvJsonWriter
{
public:
    ConvJsonWriter();

    inline bool Success() { return m_success; }

protected:
    static void CreateAssetPropertyNode(const ConvAssetProperty* assetProperty, TDocument& doc, TValue& root);

    static TValue CreateStringNode(const char* str, TDocument& doc);
    static TValue CreateStringNode(const FbxString& str, TDocument& doc);

    static TValue CreateMatrix4x4ColumnNode(const FbxAMatrix& mat, TDocument& doc);
    static void FillMatrix4x4ColumnNodes(SourceStrRef name, TValue& parent, const AMatrixArray& mats, TDocument& doc);

    static TValue CreateMatrix3x4ColumnNode(const FbxAMatrix& mat, TDocument& doc);
    static void FillMatrix3x4ColumnNodes(SourceStrRef name, TValue& parent, const AMatrixArray& mats, TDocument& doc);

    static TValue CreateMatrix4x4RowNode(const FbxAMatrix& mat, TDocument& doc);
    static void FillMatrix4x4RowNodes(SourceStrRef name, TValue& parent, const AMatrixArray& mats, TDocument& doc);

    static TValue CreateMatrix3x4RowNode(const FbxAMatrix& mat, TDocument& doc);
    static void FillMatrix3x4RowNodes(SourceStrRef name, TValue& parent, const AMatrixArray& mats, TDocument& doc);

    static TValue CreateColorNode(const FbxColor& col, TDocument& doc);
    static void FillColorNodes(SourceStrRef name, TValue& parent, const ColArray& vecs, TDocument& doc);

    static TValue CreateVector2Node(const FbxVector2& vec, TDocument& doc);
    static void FillVector2Nodes(SourceStrRef name, TValue& parent, const Vec2Array& vecs, TDocument& doc);

    static TValue CreateVector3Node(const FbxVector4& vec, TDocument& doc);
    static void FillVector3Nodes(SourceStrRef name, TValue& parent, const Vec4Array& vecs, TDocument& doc);

    static TValue CreateVector4Node(const FbxVector4& vec, TDocument& doc);
    static void FillVector4Nodes(SourceStrRef name, TValue& parent, const Vec4Array& vecs, TDocument& doc);

    static TValue CreateQuaternionNode(const FbxQuaternion& quat, TDocument& doc);
    static void FillQuaternionNodes(SourceStrRef name, TValue& parent, const QuatArray& quats, TDocument& doc);

    static void FillSkinNodes(SourceStrRef name, TValue& parent, const SkinArray& vecs, TDocument& doc);

protected:
    bool m_success;
};
