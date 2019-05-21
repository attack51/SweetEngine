#pragma once

//General Include
#include "General/SInclude.h"

//SVk Include 
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMaterialConnector.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMMsContainer.h"

//C++ Include

//Header Include
#include "SVkRHC.h"

SVkStaticMeshRHC::SVkStaticMeshRHC()
{

}
SVkStaticMeshRHC::~SVkStaticMeshRHC()
{

}

const uint32_t SVkAnimMeshRHC::GetBoneCount() const
{
    return static_cast<uint32_t>(AnimMMsContainer->MMs.size());
}

const uint32_t SVkAnimMeshRHC::GetVertexCount() const
{
    return MeshHandle.GetAsset()->GetMeshVertexCount();
}