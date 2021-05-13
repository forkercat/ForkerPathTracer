//
// Created by Junhao Wang (@Forkercat) on 2021/3/28.
//

#ifndef SRC_COMMON_COLOR_H_
#define SRC_COMMON_COLOR_H_

#include <iostream>
#include <cmath>

#include "geometry.h"

using Color3 = Vector3f;
// Color4 to be added

void WriteColor(std::ostream& out, Color3 pixelColor, int samplesPerPixel);

#endif  // SRC_COMMON_COLOR_H_
