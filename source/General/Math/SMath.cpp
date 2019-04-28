//General Include
#include "General/SInclude.h"

//Header Include
#include "SMath.h"

void SMath::SinCos(float value, float& outSin, float& outCos)
{
    // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
    float quotient = (S_INV_PI*0.5f)*value;
    if (value >= 0.0f)
    {
        quotient = (float)((int)(quotient + 0.5f));
    }
    else
    {
        quotient = (float)((int)(quotient - 0.5f));
    }
    float y = value - (2.0f*S_PI)*quotient;

    // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
    float sign;
    if (y > S_HALF_PI)
    {
        y = S_PI - y;
        sign = -1.0f;
    }
    else if (y < -S_HALF_PI)
    {
        y = -S_PI - y;
        sign = -1.0f;
    }
    else
    {
        sign = +1.0f;
    }

    float y2 = y * y;

    // 11-degree minimax approximation
    outSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

    // 10-degree minimax approximation
    float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
    outCos = sign * p;
}

inline float SMath::DeltaAngleDegrees(float a1, float a2)
{
    float delta = a2 - a1;
    if (delta > 180.0f)
    {
        delta = delta - 360.0f;
    }
    else if (delta < -180.0f)
    {
        delta = delta + 360.0f;
    }

    return delta;
}

inline float SMath::DeltaAngleRadians(float a1, float a2)
{
    float delta = a2 - a1;

    if (delta > S_PI)
    {
        delta = delta - (S_PI * 2.0f);
    }
    else if (delta < -S_PI)
    {
        delta = delta + (S_PI * 2.0f);
    }

    return delta;
}