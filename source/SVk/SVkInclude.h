#pragma once

//용도: SVk~.cpp 에서 공통적으로 사용하는 헤더들 모음. 이 파일내에서는 include만 할것 
//접근: all SVk~.cpp

//C++ Include
#include <assert.h>

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/SVkUtil.h"

#include "SVk/HighLayer/Renderer/SVkRenderer.h"

#include "SVk/LowLayer/Command/SVkCommandBuffers.h"
#include "SVk/LowLayer/Command/SVkCommandBuffer.h"
#include "SVk/LowLayer/Command/SVkCommandPool.h"
#include "SVk/LowLayer/Device/SVkDevice.h"
#include "SVk/LowLayer/Device/SVkGPUInfo.h"
#include "SVk/LowLayer/Device/SVkGPUs.h"
#include "SVk/LowLayer/Device/SVkInstance.h"
#include "SVk/LowLayer/Device/SVkQueueInfo.h"
#include "SVk/LowLayer/Sync/SVkFence.h"
#include "SVk/LowLayer/Sync/SVkSemaphores.h"
#include "SVk/LowLayer/Memory/SVkDeviceMemory.h"
#include "SVk/LowLayer/RenderTarget/SVkSurfaceRT.h"
