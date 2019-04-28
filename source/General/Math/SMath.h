#pragma once

//General Include
#include "General/SHeader.h"

//C++ Include
#include <math.h>


#define S_PI_HIGH_PRECISION         3.14159265358979323846
#define S_PI                        3.1415926536f
#define S_HALF_PI                   S_PI / 2.f
#define S_INV_PI                    0.31830988618f
#define SMALL_FLOAT                 0.000001f
#define HUGE_FLOAT                  1000000000.0f

struct SMath
{
    template<typename T>
    inline static T DegreesToRadians(T value)
    {
        return (T)(value * S_PI / 180.0f);
    }

    template<typename T>
    inline static T RadiansToDegrees(T value)
    {
        return (T)(value * 180.0f / S_PI);
    }

    template<typename T>
    inline static T Lerp(const T& lhs, const T& rhs, float alpha)
    {
        return lhs * (1.f - alpha) + rhs * alpha;
    }

    inline static float Abs(float value)
    {
        return fabs(value);
    }

    inline static float Sin(float value)
    {
        return sinf(value);
    }

    inline static float Cos(float value)
    {
        return cosf(value);
    }

    inline static float Tan(float value)
    {
        return tanf(value);
    }

    inline static float ASin(float value)
    {
        return asinf(value);
    }

    inline static float ACos(float value)
    {
        return acosf(value);
    }

    inline static float ATan(float value)
    {
        return atanf(value);
    }

    inline static float ATan2(float valueY, float valueX)
    {
        return atan2(valueY, valueX);
    }

    static void SinCos(float value, float& outSin, float& outCos);

    inline static float Sqrt(float value)
    {
        return sqrt(value);
    }

    inline static float InvSqrt(float value)
    {
        return 1 / sqrt(value);
    }

    template<typename T>
    inline static T Square(T value)
    {
        return value * value;
    }

    template<typename T>
    inline static T Clamp(T value, T min, T max)
    {
        if (value < min) return min;
        else if (value > max) return max;
        return value;
    }

    inline static float Saturate(float value)
    {
        if (value < 0.0f) return 0.0f;
        else if (value > 1.0f) return 1.0f;
        return value;
    }

    inline static float Frac(float value)
    {
        return value - floor(value);
    }

    inline static float Frac(float value, float range)
    {
        float rangeValue = value / range;
        return rangeValue - floor(rangeValue);
    }

    inline static float Freq(float value, float min, float max)
    {
        if (value > min && value < max) return value;
        float freq = max - min;

        float count = floor((value - min) / freq);
        return value - (count * freq);
    }

    inline static float DeltaAngleDegrees(float a1, float a2);
    inline static float DeltaAngleRadians(float a1, float a2);

    template<typename T>
    inline static float VectorBetweenAngle(const T& v1, const T& v2)
    {
        float cos = T::DotProduct(v1, v2);
        return RadiansToDegrees(ACos(cos));
    }
};
