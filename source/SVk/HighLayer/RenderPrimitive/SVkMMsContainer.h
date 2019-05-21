#pragma once

//General Include

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include

struct SVkMMsContainer
{
    vector<SMatrix4x3>  MMs = {};

    SVkMMsContainer(uint32_t mmCount, bool init=false)
    {
        MMs.resize(mmCount);

        if (init)
        {
            for (uint32_t i = 0; i < mmCount; ++i)
            {
                MMs[i] = SMatrix4x3::Identity;
            }
        }
    }
};
