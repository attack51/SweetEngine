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

void SVkStaticMeshRenderer::Paint()
{
    auto* renderingCommandBuffer = m_deviceRef->GetRenderingCommandBuffer();

    for_each(m_rhcs.begin(), m_rhcs.end(),
        [this, &renderingCommandBuffer](SVkStaticMeshRHCSPtr& RHC)
    {
        SAssetHandle<SVkMesh> meshHandle = RHC->MeshHandle;
        SVkMeshRHA* RHA = meshHandle.GetAsset()->GetRHA();
        vector<SVkMaterialConnectorSPtr>& matConnectors = RHA->MaterialConnectors;

        SStaticGraphicsUniformData data;
        data.WVP = RHC->WVP;
        data.Col = RHC->Col;
        meshHandle.GetAsset()->SetBufferData(&data);

        RHA->StaticVB->CmdBind(renderingCommandBuffer);
        RHA->IB->CmdBind(renderingCommandBuffer);

        vector<SVkMeshElement>& meshElements = meshHandle.GetAsset()->GetMeshElements();

        for_each(meshElements.begin(), meshElements.end(),
            [this, &RHC, &renderingCommandBuffer, &matConnectors](SVkMeshElement& drawElement)
        {
            auto& matConnector = matConnectors[drawElement.MaterialIndex];

            matConnector->StaticPipeline->CmdBind(renderingCommandBuffer, matConnector->StaticDescriptor.get());

            vkCmdDrawIndexed(
                renderingCommandBuffer->GetVkCommandBuffer(),
                drawElement.IndexCount,
                drawElement.InstanceCount,
                drawElement.IndexOffset,
                drawElement.VertexOffset,
                drawElement.InstanceOffset);
        });
    });
}