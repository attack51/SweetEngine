//General Include
#include "General/SHeader.h"

#include "General/Json/DOM/SJsonReader.h"
#include "General/Serialized/SSerializedSkeleton.h"

//C++ Include
#include <vector>


class SJsonSkeletonReader : public SJsonReader
{
public:
    SJsonSkeletonReader(SJsonValue& rootVal, SSerializedSkeleton* skeleton);

protected:
    static void ReadSkeletonProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedSkeletonProperty* data);
    static void ReadBones(SJsonValue& parentVal, const U8Char* nameVal, vector<SSerializedBone>& vec);
};
