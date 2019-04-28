//General Include
#include "General/SInclude.h"

//Header Include
#include "SPlane.h"


////////////////////////////////////////////////////////////////////////
// SPlane3D

SPlane3D::SPlane3D()
{

}

SPlane3D::SPlane3D(const SPlane3D& original)
{
    a = original.a;
    b = original.b;
    c = original.c;
    d = original.d;
}

SPlane3D::SPlane3D(float inA, float inB, float inC, float inD)
{
    a = inA;
    b = inB;
    c = inC;
    d = inD;
}

float SPlane3D::DotProduct(const SPlane3D& lhs, const SVector& rhs)
{
    return lhs.a * rhs.x + lhs.b * rhs.y + lhs.c * rhs.z + lhs.d;
}

void SPlane3D::Normalize()
{
    float norLen = sqrtf(a * a + b * b + c * c);
    a /= norLen;
    b /= norLen;
    c /= norLen;
}

////////////////////////////////////////////////////////////////////////
// SPlane2D

SPlane2D::SPlane2D()
{

}

SPlane2D::SPlane2D(const SPlane2D& original)
{
    a = original.a;
    b = original.b;
    d = original.d;
}

SPlane2D::SPlane2D(float inA, float inB, float inD)
{
    a = inA;
    b = inB;
    d = inD;
}

float SPlane2D::DotProduct(const SPlane2D& lhs, const SVector2& rhs)
{
    return lhs.a * rhs.x + lhs.b * rhs.y + lhs.d;
}

void SPlane2D::Normalize()
{
    float norLen = sqrtf(a * a + b * b);
    a /= norLen;
    b /= norLen;
}