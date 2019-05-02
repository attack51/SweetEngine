#pragma once

//용도: svk와 연관된 define, typedef, enum 등을 모아둠
//access SVkHeader only

#define SVK_INVALID_INDEX -1
#define SVK_PTR_TO_HANDLE(pHandle) pHandle ? (*pHandle) : VK_NULL_HANDLE

#define UNIFORM_BINDING_START 0
#define STORAGE_BINDING_START 1000
#define IMAGE_SAMPLER_BINDING_START 2000


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

enum SVkCommandBufferType : uint8_t
{
    SVk_CommandBuffer_Graphics,
    SVk_CommandBuffer_Compute,
    SVk_CommandBuffer_Transfer,
    SVk_CommandBuffer_Count,
};

enum SVkGraphicsCommandBufferType : uint8_t
{
    SVk_GraphicsCommandBuffer_Render,
    SVk_GraphicsCommandBuffer_Push,
    SVk_GraphicsCommandBuffer_Texture,
    SVk_GraphicsCommandBuffer_Count,
};

enum SVkComputeCommandBufferType : uint8_t
{
    SVk_ComputeCommandBuffer_Physics,
    SVk_ComputeCommandBuffer_Count,
};

enum SVkTransferCommandBufferType : uint8_t
{
    SVk_TransferCommandBuffer_Texture,
    SVk_TransferCommandBuffer_Count,
};

enum SVkBufferMemoryType : uint8_t
{
    SVk_BufferMemory_Device,
    SVk_BufferMemory_HostVisible,
};
