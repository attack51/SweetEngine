//rapid json
#include "rapidjson.h"
#include "ostreamwrapper.h"
#include "writer.h"
#include "document.h"
//fbx sdk
#include <fbxsdk.h>

//fbx converter
#include "FbxCommon.h"

#include "ConvData.h"

#include "ConvMeshImporter.h"
#include "ConvSkeletonImporter.h"
#include "ConvAnimImporter.h"

#include "ConvSkeletonJsonWriter.h"
#include "ConvMeshJsonWriter.h"
#include "ConvMaterialJsonWriter.h"
#include "ConvAnimJsonWriter.h"


//C++ Include
#include <memory>


enum ARG_MODE
{
    ARG_MODE_NONE,
    ARG_MODE_INPUT,
};

bool FetchFromArgs(
    int argc,
    char** argv,
    FbxString& inputFilePath,
    uint32_t& meshExtractElms,
    uint32_t& exportFormat)
{
    ARG_MODE mode = ARG_MODE_NONE;

    for (int i = 1, c = argc; i < c; ++i)
    {
        if (FbxString(argv[i]) == "-i")
        {
            mode = ARG_MODE_INPUT;
        }
        //vertex element
        else if (FbxString(argv[i]) == "-nor")
        {
            meshExtractElms |= MESH_EXTRACT_ELM_NOR;
        }
        else if (FbxString(argv[i]) == "-tan")
        {
            meshExtractElms |= MESH_EXTRACT_ELM_TAN;
        }
        else if (FbxString(argv[i]) == "-col")
        {
            meshExtractElms |= MESH_EXTRACT_ELM_COL;
        }
        else if (FbxString(argv[i]) == "-uv")
        {
            meshExtractElms |= MESH_EXTRACT_ELM_UV;
        }
        else if (FbxString(argv[i]) == "-uv2")
        {
            meshExtractElms |= MESH_EXTRACT_ELM_UV2;
        }
        else if (FbxString(argv[i]) == "-skin")
        {
            meshExtractElms |= MESH_EXTRACT_ELM_SKIN;
        }
        //export format(mesh/skeleton/anim)
        else if (FbxString(argv[i]) == "-mesh")
        {
            exportFormat |= EXPORT_TYPE_MESH;
        }
        else if (FbxString(argv[i]) == "-skel")
        {
            exportFormat |= EXPORT_TYPE_SKEL;
        }
        else if (FbxString(argv[i]) == "-anim")
        {
            exportFormat |= EXPORT_TYPE_ANIM;
        }
        else
        {
            switch (mode)
            {
            case ARG_MODE_INPUT:
                inputFilePath = argv[i];
                break;
            default:
                break;
            }
            mode = ARG_MODE_NONE;
        }
    }

    //test
    //inputFilePath = "Eri_sweather";
    //meshExtractElms = MESH_EXTRACT_ELM_NOR | MESH_EXTRACT_ELM_UV | MESH_EXTRACT_ELM_SKIN;
    //exportFormat = EXPORT_TYPE_MESH | EXPORT_TYPE_SKEL;

    //inputFilePath = "Anim@Run";
    //meshExtractElms = MESH_EXTRACT_ELM_NOR | MESH_EXTRACT_ELM_UV | MESH_EXTRACT_ELM_SKIN;
    //exportFormat = EXPORT_TYPE_ANIM;

    if (inputFilePath.IsEmpty())
    {
        FBXSDK_printf("\nFbxConv -mesh -skel -anim -i <filepath> -nor -tan -col -uv -uv2 -skin\
                       \n <filepath> except for extension (ex: testCube)\
                       \n -nor -tan -col -uv -uv2 -skin is optional");
        return false;
    }

    return true;
}

bool FetchPaths(
    const FbxString& inputFilePath,
    FbxString& dir,
    FbxString& fbxFilePath,
    FbxString& jmeFilePath,
    FbxString& jskFilePath,
    FbxString& janFilePath)
{
    dir = GetDirectory(inputFilePath);

    fbxFilePath = inputFilePath;
    fbxFilePath.Append(".fbx", 4);

    jmeFilePath = inputFilePath;
    jmeFilePath.Append(".jme", 4);

    jskFilePath = inputFilePath;
    jskFilePath.Append(".jsk", 4);

    janFilePath = inputFilePath;
    janFilePath.Append(".jan", 4);

    return true;
}

int main(int argc, char** argv)
{
    FbxString inputFilePath;
    uint32_t meshExtractElms = 0;
    uint32_t extractFormat = 0;

    if (false == FetchFromArgs(argc, argv, inputFilePath, meshExtractElms, extractFormat))
    {
        return 0;
    }

    FbxString fbxFilePath;
    FbxString jmeFilePath;
    FbxString jskFilePath;
    FbxString janFilePath;

    FbxString dir;

    if (false == FetchPaths(inputFilePath, dir, fbxFilePath, jmeFilePath, jskFilePath, janFilePath))
    {
        return 0;
    }

    FbxManager* sdkManager = NULL;
    FbxScene* scene = NULL;
    InitializeSdkObjects(sdkManager, scene);

    bool result = LoadScene(sdkManager, scene, fbxFilePath.Buffer());
    if (false == result)
    {
        FBXSDK_printf("\n <%s> fbx file not exist or invalid file", fbxFilePath.Buffer());
    }
    else
    {
        std::unique_ptr<ConvSkeletonImporter> skeletonImporter;
        std::unique_ptr<ConvMeshImporter> meshImporter;
        std::unique_ptr<ConvAnimImporter> animImporter;

        if ((extractFormat & EXPORT_TYPE_SKEL)>0 ||
            (extractFormat & EXPORT_TYPE_MESH)>0 && (meshExtractElms & MESH_EXTRACT_ELM_SKIN)>0)
        {
            skeletonImporter = std::make_unique<ConvSkeletonImporter>(scene);

            if ((extractFormat & EXPORT_TYPE_SKEL) > 0)
            {
                ConvSkeletonJsonWriter skeletonJsonWriter(jskFilePath, skeletonImporter->GetSkeleton());
            }
        }

        if ((extractFormat & EXPORT_TYPE_ANIM) > 0)
        {
            animImporter = std::make_unique<ConvAnimImporter>(scene, DEFAULT_FRAMERATE);
            ConvAnimJsonWriter animJsonWriter(janFilePath, animImporter->GetAnim());
        }

        if ((extractFormat & EXPORT_TYPE_MESH) > 0)
        {
            meshImporter = std::make_unique<ConvMeshImporter>(
                scene,
                meshExtractElms,
                skeletonImporter.get());

            //jme
            ConvMeshJsonWriter meshJsonWriter(jmeFilePath, meshImporter->GetMesh());

            //jma
            ConvMaterialMap materialsMap = meshImporter->GetMaterials();
            ConvMaterialMapIter iter = materialsMap.begin();
            for (; iter != materialsMap.end(); ++iter)
            {
                FbxString jmaFilePath = dir;
                jmaFilePath += iter->first;

                ConvMaterialSPtr importedMat = iter->second;

                ConvMaterialJsonWriter materialJsonWriter(jmaFilePath, importedMat.get());
            }
        }
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(sdkManager, result);

    return 0;
}
