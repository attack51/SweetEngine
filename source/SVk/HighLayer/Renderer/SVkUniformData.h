#pragma once

//General Include
#include "General/SHeader.h"

//SVk Include 
#include "SVk/SVkHeader.h"


struct SGeneralUniformDataG
{
    SMatrix VP;
};

struct SAnimUniformDataG
{
    uint32_t VertexCount;
};

struct SStaticUniformDataG
{
    SMatrix WVP;
};

struct SMaterialUniformDataG
{
    SVector Col;
};

struct SAnimUniformDataC
{
    uint32_t BoneCount;
    uint32_t VertexCount;
};