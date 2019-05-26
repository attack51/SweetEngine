#pragma once

//용도: svk와 연관된 define, typedef, enum 등을 모아둠
//access SVkHeader only

#define SVK_INVALID_INDEX -1
#define SVK_PTR_TO_HANDLE(pHandle) pHandle ? (*pHandle) : VK_NULL_HANDLE

#define UNIFORM_BINDING_START 0
#define STORAGE_BINDING_START 1000
#define IMAGE_SAMPLER_BINDING_START 2000

typedef vector<VkSemaphore> VkSemaphores;

//index로 casting 접근안하면 enum class
enum class SVkDepthStencilState : uint8_t
{
    SVk_DepthStencilState_None              = 0,
    SVk_DepthStencilState_DepthOnly         = 1,
    SVk_DepthStencilState_DepthStencil      = 2
};

//index형태로 접근하면 그냥 enum
enum SVkSurfaceSemaphoreType : uint8_t
{
    SVk_SurfaceSemaphoreType_RenderComplete,
    SVk_SurfaceSemaphoreType_PresentComplete,
    SVk_SurfaceSemaphoreType_Count
};

enum SVkRTSemaphoreType : uint8_t
{
    SVk_RTSemaphoreType_Geometry,
    SVk_RTSemaphoreType_PostProcess,
    SVk_RTSemaphoreType_Count
};

enum SVkCommandBufferType : uint8_t
{
    SVk_CommandBuffer_Graphics,
    SVk_CommandBuffer_Compute,
    SVk_CommandBuffer_Transfer,
    SVk_CommandBuffer_Count,
};

enum SVkGCommandBufferType : uint8_t
{
    SVk_GCommandBuffer_Geo,
    SVk_GCommandBuffer_PP,
    SVk_GCommandBuffer_Screen,
    SVk_GCommandBuffer_Push,
    SVk_GCommandBuffer_Texture,
    SVk_GCommandBuffer_Count,
};

enum SVkCCommandBufferType : uint8_t
{
    SVk_CCommandBuffer_Physics,
    SVk_CCommandBuffer_Count,
};

enum SVkTCommandBufferType : uint8_t
{
    SVk_TCommandBuffer_Texture,
    SVk_TCommandBuffer_Count,
};

enum SVkBufferMemoryType : uint8_t
{
    SVk_BufferMemory_Device,
    SVk_BufferMemory_HostVisible,
};
