//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"

//sax load
#include "General/Json/SAX/SJsonMeshNode.h"
#include "General/Json/SAX/SJsonLoadSaxHelper.h"

//dom load
#include "General/Json/Dom/SJsonMeshReader.h"
#include "General/Json/Dom/SJsonLoadDomHeper.h"

#include "General/Serialized/SSerializedMesh.h"

//C++ Include
#include <memory>

//Header Include
#include "SVkMeshLoadParam.h"


SVkMeshLoadParam::SVkMeshLoadParam(
    const CString& filePath,
    const SVkDevice* device,
    const VkRenderPass& renderPass,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool,
    const SVkUniformBuffer* generalUB,
    SAssetManager* assetManager)
        : SAssetLoadParameter(filePath)
{
    m_device = device;
    m_renderPass = renderPass;
    m_pipelineCache = pipelineCache;
    m_descriptorPool = descriptorPool;
    m_assetManager = assetManager;
    m_generalUB = generalUB;
}

unique_ptr<SAsset> SVkMeshLoadParam::Create()
{
    //sax load
    //SSerializedMeshUPtr serializedMesh =
    //    std::move(SJsonLoadSaxHelper::LoadFromFile<SJsonMeshNode, SSerializedMesh>(m_filePath));

    //dom load (dom load가 메모리는 더 먹겠지만 더 빠른듯 하여 dom사용)
    SSerializedMeshUPtr serializedMesh =
        std::move(SJsonLoadDomHelper::LoadFromFile<SJsonMeshReader, SSerializedMesh>(m_filePath));

    auto mesh = make_unique<SVkMesh>(
        m_device, 
        m_renderPass,
        m_pipelineCache,
        m_descriptorPool,
        m_generalUB,
        m_assetManager, 
        serializedMesh.get());

    assert(mesh);

    return std::move(unique_ptr<SAsset>(std::move(mesh)));
}
