#include "General/SInclude.h"

#include "SJsonReader.h"

SJsonReader::SJsonReader()
    : m_success(false)
{
}

void SJsonReader::ReadAssetProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedAssetProperty* data)
{
    assert(parentVal.HasMember(nameVal));
    SJsonValue& assetPropertyVal = parentVal[nameVal];
    assert(assetPropertyVal.IsObject());

    data->Identifier = ReadSString(assetPropertyVal, U8Text("identifier"));
    data->AssetType = ReadAssetType(assetPropertyVal);
}

SAssetType SJsonReader::ReadAssetType(SJsonValue& parentVal)
{
    assert(parentVal.HasMember(U8Text("asset-type")));
    SJsonValue& assetTypeVal = parentVal[U8Text("asset-type")];
    assert(assetTypeVal.IsString());

    U8String assetTypeStr = assetTypeVal.GetString();
    if (assetTypeStr == U8Text("mesh"))
    {
        return SAssetType::Mesh;
    }
    if (assetTypeStr == U8Text("material"))
    {
        return SAssetType::Material;
    }
    if (assetTypeStr == U8Text("skeleton"))
    {
        return SAssetType::Skeleton;
    }
    if (assetTypeStr == U8Text("anim"))
    {
        return SAssetType::Anim;
    }

    return SAssetType::None;
}

SString SJsonReader::ReadSString(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return SText("");

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsString());

    U8String str = strVal.GetString();
    return U8StrToSStr(str);
}

CString SJsonReader::ReadCString(SJsonValue& parentVal, const U8Char* nameVal, bool convert)
{
    if (false == parentVal.HasMember(nameVal)) return CText("");

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsString());

    if (convert)
    {
        U8String str = strVal.GetString();
        return U8StrToCStr(str);
    }
    else
    {
        return CString(strVal.GetString());
    }
}

bool SJsonReader::ReadBool(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return false;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsBool());

    return strVal.GetBool();
}

int SJsonReader::ReadInt32(SJsonValue& parentVal, const U8Char* nameVal)
{
    if(false == parentVal.HasMember(nameVal)) return 0;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsInt());

    return strVal.GetInt();
}

int64_t SJsonReader::ReadInt64(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return 0;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsInt64());

    return strVal.GetInt64();
}

uint32_t SJsonReader::ReadUInt32(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return 0;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsUint());

    return strVal.GetUint();
}

uint16_t SJsonReader::ReadUInt16(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return 0;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsUint());

    return static_cast<uint16_t>(strVal.GetUint());
}

float SJsonReader::ReadFloat(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return 0.0f;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsFloat());

    return strVal.GetFloat();
}

double SJsonReader::ReadDouble(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return 0.0;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsDouble());

    return strVal.GetDouble();
}

SMatrix4x3 SJsonReader::ReadMatrix4x3(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return SMatrix4x3::Identity;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsString());

    U8String str = strVal.GetString();

    vector<U8String> subString = U8String_split(str, U8Text(','));
    if (subString.size() != 12) return SMatrix4x3::Identity;

    SMatrix4x3 result;
    for (int col = 0; col < 3; ++col)
    {
        float x = static_cast<float>(U8AtoF(subString[col * 4 + 0].c_str()));
        float y = static_cast<float>(U8AtoF(subString[col * 4 + 1].c_str()));
        float z = static_cast<float>(U8AtoF(subString[col * 4 + 2].c_str()));
        float w = static_cast<float>(U8AtoF(subString[col * 4 + 3].c_str()));

        result.SetCol(col, SVector4(x,y,z,w));
    }
    return result;
}

SVector2 SJsonReader::ReadVector2(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return SVector2::Zero;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsString());

    U8String str = strVal.GetString();

    vector<U8String> subString = U8String_split(str, U8Text(','));
    if (subString.size() != 2) return SVector2::Zero;

    return SVector2(
        static_cast<float>(U8AtoF(subString[0].c_str())),
        static_cast<float>(U8AtoF(subString[1].c_str())));
}

SVector SJsonReader::ReadVector(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return SVector::Zero;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsString());

    U8String str = strVal.GetString();

    vector<U8String> subString = U8String_split(str, U8Text(','));
    if (subString.size() != 3) return SVector::Zero;

    return SVector(
        static_cast<float>(U8AtoF(subString[0].c_str())),
        static_cast<float>(U8AtoF(subString[1].c_str())),
        static_cast<float>(U8AtoF(subString[2].c_str())));
}

SVector4 SJsonReader::ReadVector4(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return SVector4::Zero;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsString());

    U8String str = strVal.GetString();

    vector<U8String> subString = U8String_split(str, U8Text(','));
    if (subString.size() != 4) return SVector4::Zero;

    return SVector4(
        static_cast<float>(U8AtoF(subString[0].c_str())),
        static_cast<float>(U8AtoF(subString[1].c_str())),
        static_cast<float>(U8AtoF(subString[2].c_str())),
        static_cast<float>(U8AtoF(subString[3].c_str())));
}

SQuaternion SJsonReader::ReadQuaternion(SJsonValue& parentVal, const U8Char* nameVal)
{
    if (false == parentVal.HasMember(nameVal)) return SQuaternion::Identity;

    SJsonValue& strVal = parentVal[nameVal];
    assert(strVal.IsString());

    U8String str = strVal.GetString();

    vector<U8String> subString = U8String_split(str, U8Text(','));
    if (subString.size() != 4) return SQuaternion::Identity;

    return SQuaternion(
        static_cast<float>(U8AtoF(subString[0].c_str())),
        static_cast<float>(U8AtoF(subString[1].c_str())),
        static_cast<float>(U8AtoF(subString[2].c_str())),
        static_cast<float>(U8AtoF(subString[3].c_str())));
}

void SJsonReader::ReadVector2Array(SJsonValue& parentVal, const U8Char* nameVal, vector<SVector2>& vec)
{
    ReadArray<SVector2>(parentVal, nameVal, vec,
        [](SJsonValue& val, SVector2& data)
        {
            assert(val.IsString());
            U8String str = val.GetString();

            vector<U8String> subString = U8String_split(str, U8Text(','));
            if (subString.size() != 2) return;

            data.x = static_cast<float>(U8AtoF(subString[0].c_str()));
            data.y = static_cast<float>(U8AtoF(subString[1].c_str()));
        });
}

void SJsonReader::ReadVector3Array(SJsonValue& parentVal, const U8Char* nameVal, vector<SVector>& vec)
{
    ReadArray<SVector>(parentVal, nameVal, vec,
        [](SJsonValue& val, SVector& data)
    {
        assert(val.IsString());
        U8String str = val.GetString();

        vector<U8String> subString = U8String_split(str, U8Text(','));
        if (subString.size() != 3) return;

        data.x = static_cast<float>(U8AtoF(subString[0].c_str()));
        data.y = static_cast<float>(U8AtoF(subString[1].c_str()));
        data.z = static_cast<float>(U8AtoF(subString[2].c_str()));
    });
}

void SJsonReader::ReadSkinArray(SJsonValue& parentVal, const U8Char* nameVal, vector<SSkin>& skins)
{
    ReadArray<SSkin>(parentVal, nameVal, skins,
        [](SJsonValue& val, SSkin& data)
    {
        assert(val.IsString());
        U8String str = val.GetString();

        vector<U8String> subString = U8String_split(str, U8Text(','));
        if (subString.size() != 6) return;

        data.Indices[0] = static_cast<uint8_t>(U8AtoI(subString[0].c_str()));
        data.Indices[1] = static_cast<uint8_t>(U8AtoI(subString[1].c_str()));
        data.Indices[2] = static_cast<uint8_t>(U8AtoI(subString[2].c_str()));

        data.Weights[0] = static_cast<float>(U8AtoF(subString[3].c_str()));
        data.Weights[1] = static_cast<float>(U8AtoF(subString[4].c_str()));
        data.Weights[2] = static_cast<float>(U8AtoF(subString[5].c_str()));
    });
}

void SJsonReader::ReadVector4Array(SJsonValue& parentVal, const U8Char* nameVal, vector<SVector4>& vec)
{
    ReadArray<SVector4>(parentVal, nameVal, vec,
        [](SJsonValue& val, SVector4& data)
    {
        assert(val.IsString());
        U8String str = val.GetString();

        vector<U8String> subString = U8String_split(str, U8Text(','));
        if (subString.size() != 4) return;

        data.x = static_cast<float>(U8AtoF(subString[0].c_str()));
        data.y = static_cast<float>(U8AtoF(subString[1].c_str()));
        data.z = static_cast<float>(U8AtoF(subString[2].c_str()));
        data.w = static_cast<float>(U8AtoF(subString[3].c_str()));
    });
}

void SJsonReader::ReadQuaternionArray(SJsonValue& parentVal, const U8Char* nameVal, vector<SQuaternion>& vec)
{
    ReadArray<SQuaternion>(parentVal, nameVal, vec,
        [](SJsonValue& val, SQuaternion& data)
    {
        assert(val.IsString());
        U8String str = val.GetString();

        vector<U8String> subString = U8String_split(str, U8Text(','));
        if (subString.size() != 4) return;

        data.x = static_cast<float>(U8AtoF(subString[0].c_str()));
        data.y = static_cast<float>(U8AtoF(subString[1].c_str()));
        data.z = static_cast<float>(U8AtoF(subString[2].c_str()));
        data.w = static_cast<float>(U8AtoF(subString[3].c_str()));
    });
}

void SJsonReader::ReadFaceArray(SJsonValue& parentVal, const U8Char* nameVal, vector<uint16_t>& vec)
{
    ReadArray<uint16_t>(parentVal, nameVal, vec,
        [](SJsonValue& val, uint16_t& data)
    {
        data = static_cast<uint16_t>(val.GetUint());
    });
}
