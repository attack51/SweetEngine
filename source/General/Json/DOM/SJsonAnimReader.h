//General Include
#include "General/SHeader.h"

#include "General/Json/DOM/SJsonReader.h"
#include "General/Serialized/SSerializedAnim.h"

//C++ Include
#include <vector>


class SJsonAnimReader : public SJsonReader
{
public:
    SJsonAnimReader(SJsonValue& rootVal, SSerializedAnim* anim);

protected:
    static void ReadAnimProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedAnimProperty* data);
    static void ReadAnimBones(SJsonValue& parentVal, const U8Char* nameVal, vector<SSerializedAnimBone>& vec);
};
