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
    auto frame_timer = std::chrono::steady_clock();
    auto last_frame_time = frame_timer.now();
    uint32_t frame_counter = 0;
    uint32_t fps = 0;

    auto timer = std::chrono::high_resolution_clock();
    auto last_time = timer.now();

    while (true)
    {
        auto deltaTime = timer.now() - last_time;
        auto deltaTimeSec = std::chrono::duration_cast<std::chrono::duration<float>>(deltaTime);
        auto deltaTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime);

        last_time = timer.now();

        color_rotate += 0.3f * deltaTimeSec.count();
        SVector4 clearColor;
        clearColor.x = (float)std::sin(color_rotate + CIRCLE_THIRD_1)*0.5f + 0.5f;
        clearColor.y = (float)std::sin(color_rotate + CIRCLE_THIRD_2)*0.5f + 0.5f;
        clearColor.z = (float)std::sin(color_rotate + CIRCLE_THIRD_3)*0.5f + 0.5f;
        clearColor.w = 1;

        if (renderer.UpdateWindows(clearColor, deltaTimeSec.count()) == false) break;

        ++frame_counter;
        if (last_frame_time + std::chrono::seconds(1) < frame_timer.now())
        {
            last_frame_time = frame_timer.now();
            fps = frame_counter;
            frame_counter = 0;
            std::cout << "FPS : " << fps << std::endl;
        }
    }

    return 0;
}