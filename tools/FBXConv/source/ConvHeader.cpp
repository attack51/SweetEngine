#include "ConvData.h"
#include "ConvHeader.h"


bool ConvEditVertKeyComp::operator() (const ConvEditVertKeySPtr& lhs, const ConvEditVertKeySPtr& rhs) const
{
    ConvEditVertKey* lValue = lhs.get();
    ConvEditVertKey* rValue = rhs.get();

    assert(lValue && rValue);
    return (*lhs) < (*rhs);
};
