//General Include
#include "General/SInclude.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Material/SVkMaterial.h"
#include "SVk/LowLayer/RenderState/SVkDescriptor.h"
#include "SVk/LowLayer/RenderState/SVkPipelineCache.h"
#include "SVk/LowLayer/RenderState/SVkPipeline.h"

//Header Include
#include "SVkMaterialConnector.h"


SVkMaterialConnector::SVkMaterialConnector()
{
}

SVkMaterialConnector::~SVkMaterialConnector()
{
    SPTR_SAFE_DELETE(Pipeline);
    SPTR_SAFE_DELETE(Descriptor);
    MaterialHandle.Clear();
}
