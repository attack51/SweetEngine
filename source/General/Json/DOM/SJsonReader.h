#pragma once

//General Include
#include "General/SHeader.h"

#include "General/Serialized/SSerializedAsset.h"

//rapid json
#include "rapidjson.h"
//#include "filewritestream.h"
//#include "ostreamwrapper.h"
//#include "writer.h"
#include "document.h"

//C++ Include
#include <iostream>
#include <fstream>
#include <memory>
#include <functional>
#include <cstdio>

typedef rapidjson::UTF8<> SJsonEncoding;
typedef rapidjson::GenericDocument<SJsonEncoding> SJsonDocument;
typedef rapidjson::GenericValue<SJsonEncoding> SJsonValue;


class SJsonReader
{
public:
    SJsonReader();

    inline bool Success() { return m_success; }

protected:
    static void ReadAssetProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedAssetProperty* data);

    static SAssetType ReadAssetType(SJsonValue& parentVal);
    static SString ReadSString(SJsonValue& parentVal, const U8Char* nameVal);
    static CString ReadCString(SJsonValue& parentVal, const U8Char* nameVal, bool convert=false);

    static bool ReadBool(SJsonValue& parentVal, const U8Char* nameVal);

    static int ReadInt32(SJsonValue& parentVal, const U8Char* nameVal);
    static int64_t ReadInt64(SJsonValue& parentVal, const U8Char* nameVal);

    static uint32_t ReadUInt32(SJsonValue& parentVal, const U8Char* nameVal);
    static uint16_t ReadUInt16(SJsonValue& parentVal, const U8Char* nameVal);

    static float ReadFloat(SJsonValue& parentVal, const U8Char* nameVal);
    static double ReadDouble(SJsonValue& parentVal, const U8Char* nameVal);

    static SMatrix4x3 ReadMatrix4x3(SJsonValue& parentVal, const U8Char* nameVal);

    static SVector2 ReadVector2(SJsonValue& parentVal, const U8Char* nameVal);
    static SVector ReadVector(SJsonValue& parentVal, const U8Char* nameVal);
    static SVector4 ReadVector4(SJsonValue& parentVal, const U8Char* nameVal);
    static SQuaternion ReadQuaternion(SJsonValue& parentVal, const U8Char* nameVal);

    static void ReadVector2Array(SJsonValue& parentVal, const U8Char* nameVal, vector<SVector2>& vec);
    static void ReadVector3Array(SJsonValue& parentVal, const U8Char* nameVal, vector<SVector>& vec);
    static void ReadVector4Array(SJsonValue& parentVal, const U8Char* nameVal, vector<SVector4>& vec);
    static void ReadQuaternionArray(SJsonValue& parentVal, const U8Char* nameVal, vector<SQuaternion>& vec);
    static void ReadSkinArray(SJsonValue& parentVal, const U8Char* nameVal, vector<SSkin>& skins);
    static void ReadFaceArray(SJsonValue& parentVal, const U8Char* nameVal, vector<uint16_t>& vec);

    template<typename DataT>
    static void ReadArray(
        SJsonValue& parentVal,
        const U8Char* nameVal,
        vector<DataT>& vec,
        std::function<void(SJsonValue&, DataT&)> readFunc)
    {
        if (false == parentVal.HasMember(nameVal)) return;

        SJsonValue& arrVal = parentVal[nameVal];
        assert(arrVal.IsArray());

        vec.resize(arrVal.Size());

        int count = 0;
        for (auto iter = arrVal.Begin(); iter != arrVal.End(); ++iter, ++count)
        {
            readFunc(*iter, vec[count]);
        }
    }

protected:
    bool m_success;
};
