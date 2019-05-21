//General Include
#include "General/SInclude.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Material/SVkMaterial.h"

#include "SVk/LowLayer/Descriptor/SVkGraphicsDescriptor.h"

#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Pipeline/SVkGraphicsPipeline.h"

//Header Include
#include "SVkMaterialConnector.h"


SVkMaterialConnector::SVkMaterialConnector()
{
}

SVkMaterialConnector::~SVkMaterialConnector()
{
    SPTR_SAFE_DELETE(StaticDescriptor);
    SPTR_SAFE_DELETE(StaticPipeline);

    SPTR_SAFE_DELETE(AnimDescriptor);
    SPTR_SAFE_DELETE(AnimPipeline);

    MaterialHandle.Clear();
}
