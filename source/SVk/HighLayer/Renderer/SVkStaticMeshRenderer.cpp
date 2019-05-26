//General Include
#include "General/SInclude.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Device/SVkDevice.h"
#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkIndexBuffer.h"
#include "SVk/LowLayer/Pipeline/SVkGraphicsPipeline.h"
#include "SVk/LowLayer/Descriptor/SVkGraphicsDescriptor.h"
#include "SVk/LowLayer/Texture/SVkTexture.h"

#include "SVk/HighLayer/Renderer/SVkRHC.h"
#include "SVk/HighLayer/Renderer/SVkUniformData.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMaterialConnector.h"

//Platform Include

//C++ Include
#include <algorithm>
#include <functional>

//Header Include
#include "SVkStaticMeshRenderer.h"


SVkStaticMeshRenderer::SVkStaticMeshRenderer(const SVkDevice* device)
{
    m_deviceRef = device;
}

SVkStaticMeshRenderer::~SVkStaticMeshRenderer()
{

}

void SVkStaticMeshRenderer::ClearRHC()
{
    m_rhcs.clear();
}

bool SVkStaticMeshRenderer::PushRHC(SVkStaticMeshRHCSPtr rhc)
{
    m_rhcs.push_back(rhc);
    return true;
}

void SVkStaticMeshRenderer::Paint(SVkCommandBuffer* commandBuffer)
{
    for_each(m_rhcs.begin(), m_rhcs.end(),
        [this, &commandBuffer](SVkStaticMeshRHCSPtr& RHC)
    {
        PaintForEach(commandBuffer, RHC.get(), false);
    });

    for_each(m_rhcs.begin(), m_rhcs.end(),
        [this, &commandBuffer](SVkStaticMeshRHCSPtr& RHC)
    {
        PaintForEach(commandBuffer, RHC.get(), true);
    });
}

void SVkStaticMeshRenderer::PaintForEach(SVkCommandBuffer* commandBuffer, SVkStaticMeshRHC* RHC, bool drawAlphaBlend)
{
    SAssetHandle<SVkMesh> meshHandle = RHC->MeshHandle;
    SVkMeshRHA* RHA = meshHandle.GetAsset()->GetRHA();
    vector<SVkMaterialConnectorSPtr>& matConnectors = RHA->MaterialConnectors;

    //wrong method! fix it.
    SStaticUniformDataG data;
    data.WVP = RHC->WVP;

    meshHandle.GetAsset()->SetStaticBufferData(&data);

    RHA->StaticVB->CmdBind(commandBuffer);
    RHA->IB->CmdBind(commandBuffer);

    vector<SVkMeshElement>& meshElements = meshHandle.GetAsset()->GetMeshElements();

    for_each(meshElements.begin(), meshElements.end(),
        [this, &drawAlphaBlend, &RHC, &commandBuffer, &matConnectors](SVkMeshElement& drawElement)
    {
        auto& matConnector = matConnectors[drawElement.MaterialIndex];
        if (matConnector->MaterialHandle.GetAsset()->AlphaBlend() != drawAlphaBlend) return;

        matConnector->StaticPipeline->CmdBind(commandBuffer, matConnector->StaticDescriptor.get());

        vkCmdDrawIndexed(
            commandBuffer->GetVkCommandBuffer(),
            drawElement.IndexCount,
            drawElement.InstanceCount,
            drawElement.IndexOffset,
            drawElement.VertexOffset,
            drawElement.InstanceOffset);
    });
}
