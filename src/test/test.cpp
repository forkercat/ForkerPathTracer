//
// Created by Junhao Wang (@Forkercat) on 2021/3/27.
//

#include <spdlog/spdlog.h>

#include <iostream>

#include "common.h"

int main()
{
    // Spdlog

    spdlog::set_level(spdlog::level::debug);

    Vector2f v2(1.53f, 1.5111f);

    std::cout << v2 << std::endl;

    return 0;
}
