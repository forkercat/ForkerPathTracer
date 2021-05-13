#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

#include <fstream>
#include <iostream>

#include "common.h"
#include "core.h"

Scene RandomScene()
{
    Scene scene;

    auto ground_material = std::make_shared<Lambertian>(Color3(0.5f));
    scene.Add(std::make_shared<Sphere>(Point3f(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            Float   chooseMat = Random01();
            Point3f center(a + 0.9f * Random01(), 0.2f, b + 0.9f * Random01());

            if ((center - Point3f(4, 0.2f, 0)).Length() > 0.9f)
            {
                std::shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8f)
                {
                    // diffuse
                    auto albedo = Color3(Random01(), Random01(), Random01());
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    scene.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
                }
                else if (chooseMat < 0.95)
                {
                    // metal
                    auto albedo = Color3(Random01() * 0.5f + 0.5f);
                    auto fuzz = Random01() * 0.5f;
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                    scene.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
                }
                else
                {
                    // glass
                    sphereMaterial = std::make_shared<Dielectric>(1.5f);
                    scene.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5f);
    scene.Add(std::make_shared<Sphere>(Point3f(0, 1, 0), 1.f, material1));

    auto material2 = std::make_shared<Lambertian>(Color3(0.4f, 0.2f, 0.1f));
    scene.Add(std::make_shared<Sphere>(Point3f(-4, 1, 0), 1.f, material2));

    auto material3 = std::make_shared<Metal>(Color3(0.7f, 0.6f, 0.5f), 0.f);
    scene.Add(std::make_shared<Sphere>(Point3f(4, 1, 0), 1.f, material3));

    return scene;
}

inline void UpdateProgress(float progress)
{
    int barWidth = 70;

    std::cout << "Progress: [";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.001) << " %\r";
    std::cout.flush();
}

Color3 CastRay(const Ray& ray, const Hittable& world, int depth)
{
    HitRecord hitRecord;

    if (depth <= 0)
    {
        return Color3(0, 0, 0);
    }

    if (world.Hit(ray, 0.001, Infinity, hitRecord))
    {
        Ray    rayScattered;
        Color3 attenuation;
        if (hitRecord.material->Scatter(ray, hitRecord, attenuation, rayScattered))
        {
            return attenuation * CastRay(rayScattered, world, depth - 1);
        }

        return Color3(0, 0, 0);
    }

    // Background
    Float t = 0.5f * (ray.dir.y + 1.f);
    return Lerp(t, Color3(1.f, 1.f, 1.f), Color3(0.5f, 0.7f, 1.f));
}

int main()
{
    // Spdlog
    spdlog::set_pattern("[%^%l%$] %v");
    spdlog::set_level(spdlog::level::debug);

    // Image
    const Float aspectRatio = 16.f / 9.f;
    const int   imageWidth = 400;
    const int   imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int   samplesPerPixel = 50;
    const int   maxDepth = 100;

    // World
    Scene scene;

    auto materialGround = std::make_shared<Lambertian>(Color3(0.8f, 0.8f, 0.f));
    auto materialCenter = std::make_shared<Lambertian>(Color3(0.1f, 0.2f, 0.5f));
    auto materialLeft = std::make_shared<Dielectric>(1.5f);
    auto materialRight = std::make_shared<Metal>(Color3(0.8f, 0.6f, 0.2f), 0.f);

    // scene.Add(std::make_shared<Sphere>(Point3f(0.f, -100.5f, -1.f), 100, materialGround));
    // scene.Add(std::make_shared<Sphere>(Point3f(0.f, 0.f, -1.f), 0.5f, materialCenter));
    // scene.Add(std::make_shared<Sphere>(Point3f(-1.f, 0.f, -1.f), 0.5f, materialLeft));
    // scene.Add(std::make_shared<Sphere>(Point3f(-1.f, 0.f, -1.f), -0.45f, materialLeft));
    // scene.Add(std::make_shared<Sphere>(Point3f(1.f, 0.f, -1.f), 0.5f, materialRight));

    scene = RandomScene();

    scene.BuildBVH();

    if (!scene.SupportBVH())
    {
        spdlog::warn("Cast rays without BVH build!");
    }

    // Camera
    // Point3f lookFrom(3, 3, 2);
    // Point3f lookAt(0, 0, -1);
    // Float   focusDistance = Distance(lookFrom, lookAt);
    Float   aperture = 0.f;
    Float   vfov = 20;

    Point3f lookFrom(13, 2, 3);
    Point3f lookAt(0, 0, 0);
    Float   focusDistance = 10.0;

    Camera camera(lookFrom, lookAt, Vector3f::Up(), vfov, aspectRatio, aperture,
                  focusDistance);

    // Render
    std::ofstream outfile("output/image.ppm");
    outfile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    spdlog::stopwatch timer;

    for (int j = imageHeight - 1; j >= 0; --j)
    {
        for (int i = 0; i < imageWidth; ++i)
        {
            Color3 color(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s)
            {
                Float xOffset = i + Random01();
                Float yOffset = j + Random01();

                // Map To [0, 1]
                Float u = xOffset / (imageWidth - 1);
                Float v = yOffset / (imageHeight - 1);

                Ray ray = camera.GetRay(u, v);
                color += CastRay(ray, scene, maxDepth);
            }
            // Output
            WriteColor(outfile, color, samplesPerPixel);
        }

        Float progress = (Float)(imageHeight - 1 - j) / (imageHeight - 1);
        UpdateProgress(progress);
    }

    spdlog::info("<Time Used: {:.6} Seconds>", timer);

    outfile.close();
}
