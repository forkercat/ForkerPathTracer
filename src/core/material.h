//
// Created by Junhao Wang (@Forkercat) on 2021/3/29.
//

#ifndef SRC_CORE_MATERIAL_H_
#define SRC_CORE_MATERIAL_H_

#include "common.h"
#include "hittable.h"

class Material
{
public:
    virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         Color3& attenuation,
                         Ray& rayScattered) const = 0;
};

// Lambertian (Diffuse)
class Lambertian : public Material
{
public:
    explicit Lambertian(const Color3& a) : albedo(a) { }

    virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         Color3& attenuation,
                         Ray& rayScattered) const override
    {
        Vector3f scatterDirection = hitRecord.normal + RandomUnitVector();

        // Catch degenerate scatter direction
        if (scatterDirection.NearZero())
            scatterDirection = hitRecord.normal;
        else
            scatterDirection = Normalize(scatterDirection);

        rayScattered = Ray(hitRecord.p, scatterDirection);
        attenuation = albedo;

        return true;
    }

    Color3 albedo;
};

// Metal
class Metal : public Material
{
public:
    explicit Metal(const Color3& a, Float f) : albedo(a), fuzz(f < 1 ? f : 1) { }

    virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         Color3& attenuation,
                         Ray& rayScattered) const override
    {
        Vector3f reflectDir =
            Reflect(rayIn.dir, hitRecord.normal) + fuzz * RandomVectorInUnitSphere();
        reflectDir = Normalize(reflectDir);
        rayScattered = Ray(hitRecord.p, reflectDir);
        attenuation = albedo;

        return Dot(reflectDir, hitRecord.normal) > 0;
    }

    Color3 albedo;
    Float fuzz;
};

// Dielectric
class Dielectric : public Material
{
public:
    explicit Dielectric(Float indexOfRefraction) : ir(indexOfRefraction) { }

    virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         Color3& attenuation,
                         Ray& rayScattered) const override
    {
        attenuation = Color3(1, 1, 1);

        // Front face: air --> inside
        Float etaRatio = hitRecord.frontFace ? (1.f / ir) : ir;

        Float cosTheta = Min(1.f, Dot(-rayIn.dir, hitRecord.normal));
        Float sinTheta = std::sqrt(1.f - cosTheta * cosTheta);

        bool cannotRefract = etaRatio * sinTheta > 1.f;

        Vector3f outDir;

        if (cannotRefract || reflectance(cosTheta, etaRatio) > Random01())
            outDir = Reflect(rayIn.dir, hitRecord.normal);
        else
            outDir = Normalize(Refract(rayIn.dir, hitRecord.normal, etaRatio));

        rayScattered = Ray(hitRecord.p, outDir);

        return true;
    }

    Float ir;  // index of refraction

private:
    static Float reflectance(Float cosTheta, Float etaRatio)
    {
        // Use Schlick's approximation for reflectance
        Float r0 = (1.f - etaRatio) / (1.f + etaRatio);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow(1.f - cosTheta, 5.f);
    }
};

#endif  // SRC_CORE_MATERIAL_H_
