//
// Created by Junhao Wang (@Forkercat) on 2021/5/12.
//

#include "color.h"

void WriteColor(std::ostream& out, Color3 pixelColor, int samplesPerPixel)
{
    Float r = pixelColor.x;
    Float g = pixelColor.y;
    Float b = pixelColor.z;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0
    Float scale = 1.f / samplesPerPixel;
    r = std::sqrt(scale * r);
    g = std::sqrt(scale * g);
    b = std::sqrt(scale * b);

    int ir = static_cast<int>(255.999 * Clamp01(r));
    int ig = static_cast<int>(255.999 * Clamp01(g));
    int ib = static_cast<int>(255.999 * Clamp01(b));

    out << ir << ' ' << ig << ' ' << ib << '\n';
}