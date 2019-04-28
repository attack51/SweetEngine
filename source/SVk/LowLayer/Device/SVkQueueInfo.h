#pragma once

//SVk Include
#include "SVk/SVkHeader.h"


struct SVkQueueInfo
{
    VkQueue     Queue;
    uint32_t    FamilyIndex;
    uint32_t    QueueIndex;//sub index of Family
    float       Priority;
};
