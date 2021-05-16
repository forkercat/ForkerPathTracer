//
// Created by Junhao Wang (@Forkercat) on 2021/3/27.
//

#include <spdlog/spdlog.h>

#include <iostream>

#include "common.h"
#include "core.h"
#include "triangle.h"
#include "loader.h"

int main()
{
    // Spdlog

    spdlog::set_level(spdlog::level::debug);

    Loader loader("obj/mary/mary.obj");
    loader.BuildBVH();

    auto meshTriangles = loader.MeshTriangles();


    spdlog::debug("cool!");

    // MeshTriangle

    return 0;
}
