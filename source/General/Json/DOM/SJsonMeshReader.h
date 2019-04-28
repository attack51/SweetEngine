//General Include
#include "General/SHeader.h"

#include "General/Json/DOM/SJsonReader.h"
#include "General/Serialized/SSerializedMesh.h"

//C++ Include
#include <vector>


class SJsonMeshReader : public SJsonReader
{
public:
    SJsonMeshReader(SJsonValue& rootVal, SSerializedMesh* mesh);

protected:
    static void ReadMeshProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedMeshProperty* data);
    static void ReadMeshGroups(SJsonValue& parentVal, const U8Char* nameVal, vector<SSerializedMeshGroup>& vec);
};
