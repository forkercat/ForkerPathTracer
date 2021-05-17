#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

#include <fstream>
#include <future>
#include <iostream>
#include <thread>

#include "common.h"
#include "core.h"

#define NUM_THREADS 8

Scene RandomScene(int num)
{
    Scene scene;

    auto ground_material = std::make_shared<Lambertian>(Color3(0.5f));
    scene.Add(std::make_shared<Sphere>(Point3f(0, -1000, 0), 1000, ground_material));

    for (int a = -num; a < num; ++a)
    {
        for (int b = -num; b < num; ++b)
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

    // auto material1 = std::make_shared<Dielectric>(1.5f);
    // scene.Add(std::make_shared<Sphere>(Point3f(0, 1, 0), 1.f, material1));
    //
    // auto material2 = std::make_shared<Lambertian>(Color3(0.4f, 0.2f, 0.1f));
    // scene.Add(std::make_shared<Sphere>(Point3f(-4, 1, 0), 1.f, material2));
    //
    // auto material3 = std::make_shared<Metal>(Color3(0.7f, 0.6f, 0.5f), 0.f);
    // scene.Add(std::make_shared<Sphere>(Point3f(4, 1, 0), 1.f, material3));

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

    if (world.Hit(ray, 0.001f, Infinity, hitRecord))
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

struct SampleInfo
{
    int x, y;
    int numSamples, maxDepth;
    int imageWidth, imageHeight;
};

Color3 Sample(SampleInfo info, Camera& camera, Scene& scene)
{
    Color3 color(0.f);
    for (int s = 0; s < info.numSamples; ++s)
    {
        Float xOffset = info.x + Random01();
        Float yOffset = info.y + Random01();

        // Map To [0, 1]
        Float u = xOffset / (info.imageWidth - 1);
        Float v = yOffset / (info.imageHeight - 1);

        Ray ray = camera.GetRay(u, v);
        color += CastRay(ray, scene, info.maxDepth);
    }

    return color;
}

int main()
{
    // Spdlog
    spdlog::set_pattern("[%^%l%$] %v");
    spdlog::set_level(spdlog::level::debug);

    // Image
    const Float aspectRatio = 16.f / 10.f;
    const int   imageWidth = 300;
    const int   imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int   samplesPerPixel = 100;
    const int   maxDepth = 50;

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

    scene = RandomScene(3);
    scene.Add(std::make_shared<Rectangle>(-1.f, 1.f, -1.f, 1.f, 0.f, materialCenter));

    Loader loader("obj/chalkboard/chalkboard.obj");
    auto meshTriangles = loader.MeshTriangles();  // vector

    for (const std::shared_ptr<MeshTriangle>& mesh : meshTriangles)
    {
        // Transformation
        mesh->ApplyTransform(Vector3f(0.f, 0.f, 0.f), 180.f, 1.5f);
        // mesh->ApplyTransform(Vector3f(0.f, 0.f, 0.f), 0.f);

        // BVH
        mesh->BuildBVH();

        // mesh->SetMaterial(materialCenter);
        scene.Add(mesh);
    }

    scene.BuildBVH();

    if (!scene.SupportBVH())
    {
        spdlog::warn("Cast rays without BVH build!");
    }

    // Camera
    // Point3f lookFrom(3, 3, 2);
    // Point3f lookAt(0, 0, -1);
    Float   aperture = 0.f;
    Float   vfov = 20;

    Point3f lookFrom(2.f, 2.5, 13);
    Point3f lookAt(0, 1.6f, 0);
    Float   focusDistance = Distance(lookFrom, lookAt);
    // Float   focusDistance = 10.0;

    Camera camera(lookFrom, lookAt, Vector3f::Up(), vfov, aspectRatio, aperture,
                  focusDistance);

    // Render
    std::ofstream outfile("output/image.ppm");
    outfile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    spdlog::stopwatch timer;

    // Configure Sample Info
    SampleInfo sampleInfo;
    sampleInfo.maxDepth = maxDepth;
    sampleInfo.imageWidth = imageWidth;
    sampleInfo.imageHeight = imageHeight;

    assert(NUM_THREADS > 0);
    spdlog::info("#Threads: {}", NUM_THREADS);

    std::future<Color3> sampleColors[NUM_THREADS];

    for (int j = imageHeight - 1; j >= 0; --j)
    {
        for (int i = 0; i < imageWidth; ++i)
        {
            // Sending out multiple samples within a pixel

            int numSamplesPerThread = (int) std::round(samplesPerPixel / (Float)NUM_THREADS);

            // Update Sample Info
            sampleInfo.x = i;
            sampleInfo.y = j;

            // Allocation
            for (int tid = 0; tid < NUM_THREADS; ++tid)
            {
                sampleInfo.numSamples = numSamplesPerThread;
                sampleColors[tid] =
                    std::async(Sample, sampleInfo, std::ref(camera), std::ref(scene));
            }

            // Join
            Color3 color(0.f);
            for (int tid = 0; tid < NUM_THREADS; ++tid)
            {
                color += sampleColors[tid].get();
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
