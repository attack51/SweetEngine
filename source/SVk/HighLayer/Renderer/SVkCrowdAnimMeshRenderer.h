#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"

//SVk Include 
#include "SVk/SVkHeader.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"

//C++ Include
#include <vector>
#include <map>
#include <list>
#include <memory>

FORWARD_DECL_UPTR(class, SVkFence);
FORWARD_DECL_UPTR(class, SVkBufferBarrier);

FORWARD_DECL_SPTR(class, SRHC);
FORWARD_DECL_SPTR(class, SVkAnimMeshRHC);

FORWARD_DECL_PTR(class, SCamera);
FORWARD_DECL_PTR(class, SInputState);

FORWARD_DECL_SPTR(class, SVkShader);
FORWARD_DECL_SPTR(class, SVkBuffer);
FORWARD_DECL_SPTR(class, SVkStorageBuffer);
FORWARD_DECL_SPTR(class, SVkUniformBuffer);
FORWARD_DECL_SPTR(class, SVkVertexBuffer);
FORWARD_DECL_SPTR(class, SVkComputeDescriptor);
FORWARD_DECL_SPTR(class, SVkComputePipeline);

FORWARD_DECL_SPTR(struct, SVkMMsContainer);


class SVkCrowdAnimMeshRenderer
{
public:
    SVkCrowdAnimMeshRenderer(
        const SVkDevice* device,
        SAssetManager* assetManager,
        const SVkPipelineCache* pipelineCache,
        const SVkDescriptorPool* descriptorPool,
        const SVkUniformBuffer* generalUB,
        const SAssetHandle<SVkShader>& csHandle);

    ~SVkCrowdAnimMeshRenderer();

    void ClearMeshAsset();
    void ClearRHC();
    bool PushRHC(SVkAnimMeshRHCSPtr rhc);

    void ComputeVertex();
    void Paint();

    bool IsValid() const { return m_meshHandle.IsValid(); }
    uint32_t GetNumRHC() const { return static_cast<uint32_t>(m_rhcs.size()); }
    const CString& GetKey() const { return  IsValid() ? m_meshHandle.GetKey() : EmptyCString; }

protected:
    void InitStorageBuffers();
    void InitUniformBuffer();
    void InitDescriptor(const SVkDescriptorPool* descriptorPool);
    void InitPipeline(const SVkPipelineCache* pipelineCache, const SAssetHandle<SVkShader>& csHandle);
    void InitFence();

    void DeInitStorageBuffers();
    void DeInitUniformBuffer();
    void DeInitDescriptor();
    void DeInitPipeline();
    void DeInitFence();

    void UpdateAnimInfoUB(SVkAnimMeshRHC* rhc);
    void UpdateCsDescriptor();

    void UpdateGraphicsDescriptor();
    void InsertCrowdAnimMMs(SVkAnimMeshRHC* rhc);

protected:
    //don't change member
    const SVkDevice*                    m_deviceRef = nullptr;
    SAssetManager*                      m_assetManager = nullptr;
    const SVkUniformBuffer*             m_generalUB = nullptr;

    //todo:차후에 compute animation을 ManyCrowd에서 한번한 하도록 수정하기
    SAssetHandle<SVkShader>             m_csHandle = {};
    
    //MMs cpu memory. copy from MeshAnimInstance
    SVkMMsContainerSPtr                 m_crowdAnimMMs = nullptr;//don't change but write every frame

    //MMs storage buffer. copy from crowndAnimMMs
    SVkStorageBufferSPtr                m_csAnimMatrixSB;//don't change but write every frame
    
    //AnimResult storage buffer. process from compute shader
    SVkStorageBufferSPtr                m_csAnimatedVertexSB = nullptr;
    
    //uniform buffer for compute shader 
    SVkUniformBufferSPtr                m_csAnimInfoUB = nullptr;

    SVkComputeDescriptorSPtr            m_csDescriptor = nullptr; //don't change but update
    SVkComputePipelineSPtr              m_csPipeline = nullptr;
    SVkFenceUPtr                        m_csFence = nullptr;

    SVkFenceUPtr                        m_copyFence = nullptr;

    //change member
    SAssetHandle<SVkMesh>               m_meshHandle;
    vector<SVkAnimMeshRHCSPtr>          m_rhcs;
};

typedef unique_ptr<SVkCrowdAnimMeshRenderer> SVkCrowdAnimMeshRendererUPtr;

typedef vector<SVkCrowdAnimMeshRendererUPtr> PoolRenderers;
typedef list<SVkCrowdAnimMeshRenderer*> UnUseRenderers;

typedef map<CString, SVkCrowdAnimMeshRenderer*> ValidRendererMap;
typedef ValidRendererMap::iterator ValidRendererMapIter;
typedef pair<const CString, SVkCrowdAnimMeshRenderer*> ValidRendererPair;

class SVkManyCrowdAnimMeshRenderer
{
public:
    SVkManyCrowdAnimMeshRenderer(
        const SVkDevice* device,
        SAssetManager* assetManager,
        const SVkPipelineCache* pipelineCache,
        const SVkDescriptorPool* descriptorPool,
        const SVkUniformBuffer* generalUB);

    ~SVkManyCrowdAnimMeshRenderer();

    void ClearRHC();
    void PushRHC(SVkAnimMeshRHCSPtr rhc);

    void ComputeVertex();
    void Paint();

protected:
    void InitComputeShader();

    void InitPoolRenderers(
        const SVkDevice* device,
        SAssetManager* assetManager,
        const SVkPipelineCache* pipelineCache,
        const SVkDescriptorPool* descriptorPool,
        const SVkUniformBuffer* generalUB);

    void DeInitComputeShader();
    void DeInitPoolRenderers();

    void ClearNoUseRenderers();

protected:
    const SVkDevice*                            m_deviceRef = nullptr;
    SAssetManager*                              m_assetManager = nullptr;

    SAssetHandle<SVkShader>                     m_csHandle = {};
    PoolRenderers                               m_poolRenderers;

    //UpdateDescriptor를 줄이기 위해 다음에도 가능하면 재사용
    UnUseRenderers                              m_unUseRenderers;
    ValidRendererMap                            m_validRenderers;
};