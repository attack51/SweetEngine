#pragma once

//General Include
#include "General/SHeader.h"

//SVk Include 
#include "SVk/SVkHeader.h"


struct SGeneralUniformDataG
{
    SMatrix VP;
    SVector4 ScreenSize;//zw:invScreenSize
    float DeltaTime;
};

struct SAnimUniformDataG
{
    uint32_t VertexCount;
    uint32_t PrevAnimVertOffset;
    uint32_t CurAnimVertOffset;
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
    uint32_t OutVertexOffset;
};

struct SPostProcessUniformDataG
{
    float Dummy;
};