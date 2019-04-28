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

FORWARD_DECL_SPTR(class, SVkShader);
FORWARD_DECL_SPTR(class, SVkVertexBuffer);
FORWARD_DECL_SPTR(class, SVkStorageBufferC2G);

FORWARD_DECL_PTR(class, SAssetManager);

FORWARD_DECL_PTR(struct, SSerializedSkeleton);
FORWARD_DECL_PTR(struct, SSerializedAnim);


class SVkMeshAnimInstance
{
public:
// Begin public funtions

    SVkMeshAnimInstance(
        const SVkDevice* device,
        SAssetManager* assetManager,
        const SAssetHandle<SVkMesh>& meshHandle,
        const SAssetHandle<SVkSkeleton>& skeletonHandle);
    ~SVkMeshAnimInstance();

    void SetAnim(const SAssetHandle<SVkAnim>& animHandle);
    void Update(float deltaTime);

    //cpu skinning test
    SVkVertexBuffer* GetAnimatedVertexBuffer() const
    {
        return m_vertexBuffer.get();
    }

// ~End public funtions

private:
// Begin private funtions
    void InitAnimDatas();
    void InitStorageBuffers();
    void InitComputeShader();

    void DeInitStorageBuffers();
    void DeInitComputeShader();

    SSerializedSkeleton* GetSkeleton() const;
    SSerializedAnim* GetAnim() const;
// ~End private funtions

private:
// Begin private fields

    const SVkDevice*                    m_deviceRef = nullptr;
    SAssetManager*                      m_assetManager          = nullptr;

    SAssetHandle<SVkMesh>               m_meshHandle            = {};
    SAssetHandle<SVkSkeleton>           m_skeletonHandle        = {};
    SAssetHandle<SVkAnim>               m_animHandle            = {};
    SAssetHandle<SVkShader>             m_csHandle              = {};

    vector<STransform>                  m_lcTransforms = {};//local transform
    vector<STransform>                  m_hcTransforms = {};//hierarchical transform

    vector<SMatrix4x3>                  m_meshAnimMMs = {};

    float                               m_animTime              = .0f;

    //cpu skinning test
    SVkVertexBufferSPtr                 m_vertexBuffer = nullptr;
    const SVkMeshVertex*                m_before_ani_vertices = {};
    vector<SVkAnimatedMeshVertex>       m_after_ani_vertices = {};

    SVkStorageBufferC2GSPtr             m_animMatricesStorageBuffer = nullptr;

// ~End private fields

private:
};
