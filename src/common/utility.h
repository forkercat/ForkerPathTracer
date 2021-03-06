//
// Created by Junhao Wang (@Forkercat) on 2021/3/27.
//

#ifndef COMMON_UTILITY_H_
#define COMMON_UTILITY_H_

#include <cstdlib>
#include <random>

#include "constant.h"

// Utility Inline Functions

inline Float Lerp(Float t, Float v1, Float v2)
{
    return (1 - t) * v1 + t * v2;
}

template <typename T>
inline T Clamp(T val, T min, T max)
{
    return std::min(max, std::max(val, min));
}

template <typename T>
inline T Clamp01(T val)
{
    return Clamp(val, 0.f, 1.f);
}

inline Float Radians(Float deg)
{
    return deg * Pi / 180.f;
}

inline Float Degrees(Float rad)
{
    return rad * 180.f / Pi;
}

template <typename T>
inline T Min(const T& v1, const T& v2)
{
    return std::min(v1, v2);
}

template <typename T>
inline T Max(const T& v1, const T& v2)
{
    return std::max(v1, v2);
}

template <typename T>
inline T Min(const T& v1, const T& v2, const T& v3)
{
    return std::min(v1, std::min(v2, v3));
}

template <typename T>
inline T Max(const T& v1, const T& v2, const T& v3)
{
    return std::max(v1, std::max(v2, v3));
}

template <typename T>
inline T Min3(const T& v1, const T& v2, const T& v3)
{
    return std::min(v1, std::min(v2, v3));
}

template <typename T>
inline T Max3(const T& v1, const T& v2, const T& v3)
{
    return std::max(v1, std::max(v2, v3));
}

inline Float Random01()
{
    // Old
    // return rand() / (RAND_MAX + 1.f);  // random real in [0, 1)
    // New
    static std::uniform_real_distribution<Float> distribution(0.f, 1.f);
    static std::mt19937                          generator;
    return distribution(generator);
}

inline Float Random(Float min, Float max)
{
    return min + (max - min) * Random01();  // random real in [min, max)
}

#endif  // COMMON_UTILITY_H_
