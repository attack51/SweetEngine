//SVk Include
#include "SVk/SVkInclude.h"

//C++ Include
#include <chrono>
#include <iostream>

constexpr double PI = 3.14159265359;
constexpr double CIRCLE_RAD = PI * 2;
constexpr double CIRCLE_THIRD = CIRCLE_RAD / 3.0;
constexpr double CIRCLE_THIRD_1 = 0;
constexpr double CIRCLE_THIRD_2 = CIRCLE_THIRD;
constexpr double CIRCLE_THIRD_3 = CIRCLE_THIRD * 2;


int main()
{
    SVkRenderer renderer;
    renderer.OpenMainWindow(512, 512, "test_win");

    float color_rotate = 0;
    auto timer = std::chrono::steady_clock();
    auto last_time = timer.now();
    uint32_t frame_counter = 0;
    uint32_t fps = 0;

    while (true)
    {
        color_rotate += 0.001f;
        SVector4 clearColor;
        clearColor.x = (float)std::sin(color_rotate + CIRCLE_THIRD_1)*0.5f + 0.5f;
        clearColor.y = (float)std::sin(color_rotate + CIRCLE_THIRD_2)*0.5f + 0.5f;
        clearColor.z = (float)std::sin(color_rotate + CIRCLE_THIRD_3)*0.5f + 0.5f;
        clearColor.w = 1;

        if (renderer.UpdateWindows(clearColor) == false) break;

        ++frame_counter;
        if (last_time + std::chrono::seconds(1) < timer.now())
        {
            last_time = timer.now();
            fps = frame_counter;
            frame_counter = 0;
            std::cout << "FPS : " << fps << std::endl;
        }
    }

    return 0;
}