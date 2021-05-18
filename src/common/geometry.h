//
// Created by Junhao Wang (@Forkercat) on 2021/3/26.
//

#ifndef COMMON_GEOMETRY_H_
#define COMMON_GEOMETRY_H_

#include <spdlog/fmt/ostr.h>


#include <cmath>
#include <iostream>

#include "check.h"
#include "stringprint.h"
#include "utility.h"

template <typename T>
inline bool isNaN(const T x)
{
    return std::isnan(x);
}

template <>
inline bool isNaN(const int x)
{
    return false;
}

// Vector2

template <typename T>
class Vector2
{
public:
    bool HasNaNs() const { return isNaN(x) || isNaN(y); }
    bool NearZero() const
    {
        const Float s = 1e-8;
        return std::fabs(x) < s && std::fabs(y) < s;
    }

    // Constructors
    Vector2() : x(T()), y(T()) { }
    Vector2(T xx, T yy) : x(xx), y(yy) { DCHECK(!HasNaNs()); }
    explicit Vector2(T val) : x(val), y(val) { DCHECK(!HasNaNs()); }

#ifndef NDEBUG
    // If it is in DEBUG mode, we use these copy constructor and operator=
    // otherwise, we go for the default options
    Vector2(const Vector2<T>& v)
    {
        DCHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
    }

    Vector2<T>& operator=(const Vector2<T>& v)
    {
        DCHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        return *this;
    }
#endif

    // Subscript Operators
    T operator[](size_t i) const
    {
        DCHECK(i >= 0 && i < 2);
        if (i == 0) return x;
        return y;
    }

    T& operator[](size_t i)
    {
        DCHECK(i >= 0 && i < 2);
        if (i == 0) return x;
        return y;
    }

    // Positive / Negative Operators
    Vector2<T> operator+() const { return Vector2<T>(x, y); }
    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

    // Equality Operators
    bool operator==(const Vector2<T>& v) const { return x == v.x && y == v.y; }
    bool operator!=(const Vector2<T>& v) const { return x != v.x || y != v.y; }

    // Add / Minus Operators
    Vector2<T> operator+(const Vector2<T>& v) const
    {
        DCHECK(!v.HasNaNs());
        return Vector2<T>(x + v.x, y + v.y);
    }

    Vector2<T> operator-(const Vector2<T>& v) const
    {
        DCHECK(!v.HasNaNs());
        return Vector2<T>(x - v.x, y - v.y);
    }

    Vector2<T>& operator+=(const Vector2<T>& v)
    {
        DCHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2<T>& operator-=(const Vector2<T>& v)
    {
        DCHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        return *this;
    }

    // Product / Division
    Vector2<T> operator*(const Vector2<T>& v) const
    {
        return Vector2<T>(x * v.x, y * v.y);
    }

    template <typename U>
    Vector2<T> operator*(U f) const
    {
        return Vector2<T>(x * f, y * f);
    }

    template <typename U>
    Vector2<T> operator/(U f) const
    {
        CHECK_NE(f, 0);
        Float inv = (Float)1 / f;
        return Vector2<T>(x * inv, y * inv);
    }

    template <typename U>
    Vector2<T>& operator*=(U f)
    {
        DCHECK(!isNaN(f));
        x *= f;
        y *= f;
        return *this;
    }

    template <typename U>
    Vector2<T>& operator/=(U f)
    {
        CHECK_NE(f, 0);
        Float inv = (Float)1 / f;
        x *= inv;
        y *= inv;
        return *this;
    }

    // Public Methods
    Float LengthSquared() const { return x * x + y * y; }
    Float Length() const { return std::sqrt(LengthSquared()); }

    // Public Data
    union
    {  // clang-format off
        struct { T x, y; };
        struct { T s, t; };
        struct { T u, v; };
    };  // clang-format on
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& v)
{
    os << "[ " << v.x << ", " << v.y << " ]";
    return os;
}

template <>
inline std::ostream& operator<<(std::ostream& os, const Vector2<Float>& v)
{
    os << StringPrintf("[ %f, %f ]", v.x, v.y);
    return os;
}

// Vector3

template <typename T>
class Vector3
{
public:
    bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }
    bool NearZero() const
    {
        const Float s = 1e-8;
        return std::fabs(x) < s && std::fabs(y) < s && std::fabs(z) < s;
    }

    // Constructors
    Vector3() : x(T()), y(T()), z(T()) { }
    Vector3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) { DCHECK(!HasNaNs()); }
    explicit Vector3(T val) : x(val), y(val), z(val) { DCHECK(!HasNaNs()); }

#ifndef NDEBUG
    // If it is in DEBUG mode, we use these copy constructor and operator=
    // otherwise, we go for the default options
    Vector3(const Vector3<T>& v)
    {
        DCHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Vector3<T>& operator=(const Vector3<T>& v)
    {
        DCHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
#endif

    // Swizzle Constructors
    Vector3(T xx, const Vector2<T>& v) : x(xx), y(v.x), z(v.y) { DCHECK(!HasNaNs()); }
    Vector3(const Vector2<T>& v, T zz) : x(v.x), y(v.y), z(zz) { DCHECK(!HasNaNs()); }

    // Subscript Operators
    T operator[](size_t i) const
    {
        DCHECK(i >= 0 && i < 3);
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    T& operator[](size_t i)
    {
        DCHECK(i >= 0 && i < 3);
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    // Positive / Negative Operators
    Vector3<T> operator+() const { return Vector3<T>(x, y, z); }
    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

    // Equality Operators
    bool operator==(const Vector3<T>& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector3<T>& v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }

    // Add / Minus Operators
    Vector3<T> operator+(const Vector3<T>& v) const
    {
        DCHECK(!v.HasNaNs());
        return Vector3<T>(x + v.x, y + v.y, z + v.z);
    }

    Vector3<T> operator-(const Vector3<T>& v) const
    {
        DCHECK(!v.HasNaNs());
        return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }

    Vector3<T>& operator+=(const Vector3<T>& v)
    {
        DCHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3<T>& operator-=(const Vector3<T>& v)
    {
        DCHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    // Product / Division
    Vector3<T> operator*(const Vector3<T>& v) const
    {
        return Vector3<T>(x * v.x, y * v.y, z * v.z);
    }

    template <typename U>
    Vector3<T> operator*(U f) const
    {
        return Vector3<T>(x * f, y * f, z * f);
    }

    template <typename U>
    Vector3<T> operator/(U f) const
    {
        CHECK_NE(f, 0);
        Float inv = (Float)1 / f;
        return Vector3<T>(x * inv, y * inv, z * inv);
    }

    template <typename U>
    Vector3<T>& operator*=(U f)
    {
        DCHECK(!isNaN(f));
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    template <typename U>
    Vector3<T>& operator/=(U f)
    {
        CHECK_NE(f, 0);
        Float inv = (Float)1 / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    // Public Methods
    Float LengthSquared() const { return x * x + y * y + z * z; }
    Float Length() const { return std::sqrt(LengthSquared()); }

    // Public Static Method
    inline static Vector3<T> Up() { return Vector3<T>(0, 1, 0); }

    inline static Vector3<T> Right() { return Vector3<T>(1, 0, 0); }

    inline static Vector3<T> Forward() { return Vector3<T>(0, 0, 1); }

    // Public Data (glm-style swizzle)
    union
    {  // clang-format off
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { Vector2<T> xy; };  // __z_ignore_it__
        struct { T __x_ignore_it__; Vector2<T> yz; };
    };  // clang-format on
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& v)
{
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
}
template <>
inline std::ostream& operator<<(std::ostream& os, const Vector3<Float>& v)
{
    os << StringPrintf("[ %f, %f, %f ]", v.x, v.y, v.z);
    return os;
}

template <typename T, typename U>
inline Vector2<T> operator*(U s, const Vector2<T>& v)
{
    return v * s;
}

template <typename T, typename U>
inline Vector3<T> operator*(U s, const Vector3<T>& v)
{
    return v * s;
}

// Typedef

typedef Vector2<Float> Vector2f;
typedef Vector2<int>   Vector2i;
typedef Vector3<Float> Vector3f;
typedef Vector3<int>   Vector3i;

// Aliases

using Point3f = Vector3f;

// Vector Inline Functions

template <typename T>
inline Vector2<T> Abs(const Vector2<T>& v)
{
    return Vector2<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
inline Vector3<T> Abs(const Vector3<T>& v)
{
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
inline Float Dot(const Vector2<T>& v1, const Vector2<T>& v2)
{
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
inline Float Dot(const Vector3<T>& v1, const Vector3<T>& v2)
{
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline Float AbsDot(const Vector2<T>& v1, const Vector2<T>& v2)
{
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return std::abs(Dot(v1, v2));
}

template <typename T>
inline Float AbsDot(const Vector3<T>& v1, const Vector3<T>& v2)
{
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return std::abs(Dot(v1, v2));
}

template <typename T>
inline Vector3<T> Cross(const Vector3<T>& v1, const Vector3<T>& v2)
{
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;
    return Vector3<T>(v1y * v2z - v1z * v2y, v1z * v2x - v1x * v2z,
                      v1x * v2y - v1y * v2x);
}

template <typename T>
inline Vector3<T> Normalize(const Vector3<T>& v)
{
    return v / v.Length();
}

template <typename T>
inline Float Distance(const Vector3<T>& p1, const Vector3<T>& p2)
{
    return (p1 - p2).Length();
}

template <typename T>
inline T MinComponent(const Vector3<T>& v)
{
    return std::min(v.x, std::min(v.y, v.z));
}

template <typename T>
inline T MaxComponent(const Vector3<T>& v)
{
    return std::max(v.x, std::max(v.y, v.z));
}

template <typename T>
int MaxDimension(const Vector3<T>& v)
{
    return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
}

template <typename T>
inline Vector3<T> Min(const Vector3<T>& p1, const Vector3<T>& p2)
{
    return Vector3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
}

template <typename T>
inline Vector3<T> Max(const Vector3<T>& p1, const Vector3<T>& p2)
{
    return Vector3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
}

template <typename T>
inline Vector3<T> Lerp(Float t, const Vector3<T>& v1, const Vector3<T>& v2)
{
    return (1 - t) * v1 + t * v2;
}

template <typename T>
inline Vector2<T> Clamp(const Vector2<T>& val, T min, T max)
{
    Vector2<T> ret;
    ret.x = Clamp(val.x, min, max);
    ret.y = Clamp(val.y, min, max);
    return ret;
}

template <typename T>
inline Vector3<T> Clamp(const Vector3<T>& val, T min, T max)
{
    Vector3<T> ret;
    ret.x = Clamp(val.x, min, max);
    ret.y = Clamp(val.y, min, max);
    ret.z = Clamp(val.z, min, max);
    return ret;
}

template <typename T>
inline Vector2<T> Clamp01(const Vector2<T>& val)
{
    return Clamp(val, 0.f, 1.f);
}

template <typename T>
inline Vector3<T> Clamp01(const Vector3<T>& val)
{
    return Clamp(val, 0.f, 1.f);
}

inline Vector3f Transform(const Vector3f& v, const Vector3f& translate, const Vector3f& rotate = Vector3f(0.f), Float scale = 1.f)
{
    Float radX = Radians(rotate.x);
    Float cosThetaX = std::cos(radX);
    Float sinThetaX = std::sin(radX);

    Float radY = Radians(rotate.y);
    Float cosThetaY = std::cos(radY);
    Float sinThetaY = std::sin(radY);

    Float radZ = Radians(rotate.z);
    Float cosThetaZ = std::cos(radZ);
    Float sinThetaZ = std::sin(radZ);

    Vector3f ret;

    // Scale
    ret = v * scale;

    // Rotate X
    ret.x = ret.x;
    ret.y = cosThetaX * ret.y - sinThetaX * ret.z;
    ret.z = sinThetaX * ret.y + cosThetaX * ret.z;

    // Rotate Y
    ret.x = cosThetaY * ret.x + sinThetaY * ret.z;
    ret.y = ret.y;
    ret.z = -sinThetaY * ret.x + cosThetaY * ret.z;

    // Rotate Z
    ret.x = cosThetaZ * ret.x - sinThetaZ * ret.y;
    ret.y = sinThetaZ * ret.x + cosThetaZ * ret.y;
    ret.z = ret.z;

    // Translate
    ret += translate;

    return ret;
}

inline Vector3f TransformNormal(const Vector3f& normal, const Vector3f& rotate = Vector3f(0.f))
{
    Float radX = Radians(rotate.x);
    Float cosThetaX = std::cos(radX);
    Float sinThetaX = std::sin(radX);

    Float radY = Radians(rotate.y);
    Float cosThetaY = std::cos(radY);
    Float sinThetaY = std::sin(radY);

    Float radZ = Radians(rotate.z);
    Float cosThetaZ = std::cos(radZ);
    Float sinThetaZ = std::sin(radZ);

    Vector3f ret;

    // Rotate X
    ret.x = normal.x;
    ret.y = cosThetaX * normal.y - sinThetaX * normal.z;
    ret.z = sinThetaX * normal.y + cosThetaX * normal.z;

    // Rotate Y
    ret.x = cosThetaY * normal.x + sinThetaY * normal.z;
    ret.y = normal.y;
    ret.z = -sinThetaY * normal.x + cosThetaY * normal.z;

    // Rotate Z
    ret.x = cosThetaZ * normal.x - sinThetaZ * normal.y;
    ret.y = sinThetaZ * normal.x + cosThetaZ * normal.y;
    ret.z = normal.z;

    return ret;
}

// Random

inline Vector3f RandomVector3f()
{
    return Vector3f(Random01(), Random01(), Random01());
}

inline Vector3f RandomVector3f(Float min, Float max)
{
    return Vector3f(Random(min, max), Random(min, max), Random(min, max));
}

// Point in unit sphere
inline Vector3f RandomVectorInUnitSphere()
{
    // Rejection Method
    while (true)
    {
        Vector3f p = RandomVector3f(-1.f, 1.f);
        if (p.LengthSquared() >= 1.f) continue;
        return p;
    }
}

// Point on the unit sphere's surface
inline Vector3f RandomUnitVector()
{
    return Normalize(RandomVectorInUnitSphere());
}

// Another way for doing the same as RandomUnitVector
inline Vector3f RandomVectorInHemisphere(const Vector3f& normal)
{
    Vector3f vectorInUnitSphere = RandomVectorInUnitSphere();

    if (Dot(vectorInUnitSphere, normal) > 0.f)  // in the same hemisphere as the normal
    {
        return vectorInUnitSphere;
    }
    else
    {
        return -vectorInUnitSphere;
    }
}

inline Vector3f RandomVectorInUnitDisk()
{
    while (true)
    {
        Vector3f p = Vector3f(Random(-1, 1), Random(-1, 1), 0);
        if (p.LengthSquared() >= 1.f) continue;
        return p;
    }
}

// Reflect & Refract

inline Vector3f Reflect(const Vector3f& inDir, const Vector3f& normal)
{
    return inDir - 2 * Dot(inDir, normal) * normal;
}

inline Vector3f Refract(const Vector3f& inDir, const Vector3f& normal, Float etaRatio)
{
    Float    cosTheta = Min(1.f, Dot(-inDir, normal));
    Vector3f refractPerp = etaRatio * (inDir + cosTheta * normal);
    Vector3f refractParallel =
        -std::sqrt(std::fabs(1.f - refractPerp.LengthSquared())) * normal;
    return refractPerp + refractParallel;
}

#endif  // COMMON_COMMON_H_
