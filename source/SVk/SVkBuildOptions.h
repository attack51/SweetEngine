#pragma once

//용도: svk와 연관된 build option (on/off) 만을 모아둠
//접근: only SVkHeader

#define BUILD_ENABLE_VULKAN_DEBUG           1
#define BUILD_ENABLE_RUNTIME_VULKAN_DEBUG   1
#define AUTO_COMPILE_GLSL_TO_SPV            1

//to use GLFW
#define BUILD_USE_GLFW                      0