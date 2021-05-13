//
// Created by Junhao Wang (@Forkercat) on 2021/3/28.
//

#ifndef CORE_CAMERA_H_
#define CORE_CAMERA_H_

#include <cmath>

#include "ray.h"

class Camera
{
public:
    Camera(const Point3f& lookFrom, const Point3f& lookAt, const Vector3f& vup,
           Float vfov, Float aspectRatio, Float aperture, Float focusDistance)
    {
        // Fov + Focus Length --> Size of Image Plane
        // Size of Image + Focus Distance --> Size of Focus Plane
        // Remember: two planes are virtual in the world

        // Defines Image Plane (focalLength = 1)
        Float h = std::tan(Radians(vfov) / 2.f);
        Float viewportHeight = 2.0f * h;  // top and bottom parts
        Float viewportWidth = aspectRatio * viewportHeight;

        // Defines Orientation
        // https://bloggg-1254259681.cos.na-siliconvalley.myqcloud.com/u3y64.jpg
        m_W = Normalize(lookFrom - lookAt);  // backward
        m_U = Normalize(Cross(vup, m_W));      // right
        m_V = Cross(m_W, m_U);                   // up

        // Defines Focus Plane
        m_EyePos = lookFrom;
        m_Horizontal = viewportWidth * m_U * focusDistance;  // focalLength = 1
        m_Vertical = viewportHeight * m_V * focusDistance;
        Vector3f vecToPlane = m_EyePos + focusDistance * (-m_W);  // z negative
        m_LowerLeftCorner = vecToPlane - (m_Horizontal / 2.0f + m_Vertical / 2.0f);

        m_LensRadius = aperture / 2;
    }

    Ray GetRay(Float s, Float t) const
    {
        Vector3f rd = m_LensRadius * RandomVectorInUnitDisk();
        Vector3f offset = m_U * rd.x + m_V * rd.y;

        Vector3f dir = Normalize(m_LowerLeftCorner + s * m_Horizontal + t * m_Vertical -
                                 (m_EyePos + offset));
        return Ray(m_EyePos + offset, dir);
    }

private:
    Point3f  m_EyePos;
    Point3f  m_LowerLeftCorner;
    Vector3f m_Horizontal;
    Vector3f m_Vertical;
    Vector3f m_U, m_V, m_W;
    Float    m_LensRadius;
};

#endif  // CORE_CAMERA_H_
