#pragma once

//General Include
#include "General/SHeader.h"


class SPlane3D
{
public:
    SPlane3D();
    SPlane3D(const SPlane3D& original);
    SPlane3D(float inA, float inB, float inC, float inD);

    static float DotProduct(const SPlane3D& lhs, const SVector& rhs);
    void Normalize();

public:

    float a = 1;
    float b = 0;
    float c = 0;
    float d = 0;
};

class SPlane2D
{
public:
    SPlane2D();
    SPlane2D(const SPlane2D& original);
    SPlane2D(float inA, float inB, float inD);

    static float DotProduct(const SPlane2D& lhs, const SVector2& rhs);
    void Normalize();

public:

    float a = 1;
    float b = 0;
    float d = 0;
};
