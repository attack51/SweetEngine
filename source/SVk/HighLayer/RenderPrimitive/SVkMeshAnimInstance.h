#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkSkeleton.h"
#include "SVk/HighLayer/RenderPrimitive/SVkAnim.h"

//C++ Include
#include <vector>

FORWARD_DECL_UPTR(class, SVkFence);

FORWARD_DECL_SPTR(class, SVkShader);
FORWARD_DECL_SPTR(class, SVkVertexBuffer);
FORWARD_DECL_SPTR(class, SVkStorageBuffer);
FORWARD_DECL_SPTR(class, SVkComputeDescriptor);
FORWARD_DECL_SPTR(class, SVkComputePipeline);

FORWARD_DECL_PTR(class, SVkDescriptorPool);
FORWARD_DECL_PTR(class, SVkPipelineCache);
FORWARD_DECL_PTR(class, SAssetManager);

FORWARD_DECL_PTR(struct, SSerializedSkeleton);
FORWARD_DECL_PTR(struct, SSerializedAnim);


struct SVkSkinComputeInfo
{
    SAssetHandle<SVkShader>             CsHandle            = {};

    SVkStorageBufferSPtr                AnimMatrixSB        = nullptr;
    SVkStorageBufferSPtr                AnimatedVertexSB    = nullptr;

    SVkComputeDescriptorSPtr            Descriptor          = nullptr;
    SVkComputePipelineSPtr              Pipeline            = nullptr;

    SVkFenceUPtr                        Fence               = nullptr;
};

class SVkMeshAnimInstance
{
public:
// Begin public funtions

    SVkMeshAnimInstance(
        const SVkDevice* device,
        const SVkPipelineCache* pipelineCache,
        const SVkDescriptorPool* descriptorPool,
        SAssetManager* assetManager,
        const SAssetHandle<SVkMesh>& meshHandle,
        const SAssetHandle<SVkSkeleton>& skeletonHandle);
    ~SVkMeshAnimInstance();

    void SetAnim(const SAssetHandle<SVkAnim>& animHandle);
    void Update(float deltaTime);

    SVkStorageBuffer* GetAnimatedStorageBuffer() const
    {
        return m_skinComputeInfo.AnimatedVertexSB.get();
    }

// ~End public funtions

private:
// Begin private funtions
    void InitAnimDatas();
    void InitStorageBuffers();
    void InitComputeShader();
    void InitDescriptor(const SVkDescriptorPool* descriptorPool);
    void InitPipeline(const SVkPipelineCache* pipelineCache);
    void InitFence();

    void DeInitStorageBuffers();
    void DeInitComputeShader();
    void DeInitDescriptor();
    void DeInitPipeline();
    void DeInitFence();

    SSerializedSkeleton* GetSkeleton() const;
    SSerializedAnim* GetAnim() const;

    void UpdateAnims(float deltaTime);
    void ComputeAnimatedVertices(float deltaTime);

    uint32_t GetVertexCount() const;
    uint32_t GetBoneCount() const;

// ~End private funtions

private:
// Begin private fields

    const SVkDevice*                    m_deviceRef             = nullptr;
    SAssetManager*                      m_assetManager          = nullptr;

    SAssetHandle<SVkMesh>               m_meshHandle            = {};
    SAssetHandle<SVkSkeleton>           m_skeletonHandle        = {};
    SAssetHandle<SVkAnim>               m_animHandle            = {};

    vector<STransform>                  m_lcTransforms          = {};//local transform
    vector<STransform>                  m_hcTransforms          = {};//hierarchical transform

    vector<SMatrix4x3>                  m_meshAnimMMs           = {};

    float                               m_animTime              = .0f;

    SVkSkinComputeInfo                  m_skinComputeInfo;

// ~End private fields

private:
};
