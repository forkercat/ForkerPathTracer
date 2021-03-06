//
// Created by Junhao Wang (@Forkercat) on 2021/3/27.
//

#ifndef COMMON_CONSTANT_H_
#define COMMON_CONSTANT_H_

// Macros
// #define FLOAT_AS_DOUBLE

#ifdef FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif

// Global Constants
#define MinInt std::numeric_limits<int>::min()
#define MaxInt std::numeric_limits<int>::max()
#define MaxFloat std::numeric_limits<Float>::max()
#define MinFloat std::numeric_limits<Float>::min()
#define LowestFloat std::numeric_limits<Float>::lowest()
#define Infinity std::numeric_limits<Float>::infinity()

static const Float Pi = 3.14159265358979323846;
static const Float InvPi = 0.31830988618379067154;
static const Float Inv2Pi = 0.15915494309189533577;
static const Float Inv4Pi = 0.07957747154594766788;
static const Float PiOver2 = 1.57079632679489661923;
static const Float PiOver4 = 0.78539816339744830961;
static const Float Sqrt2 = 1.41421356237309504880;

#endif  // COMMON_CONSTANT_H_
