#pragma once

//�뵵: svk�� ������ define, typedef, enum ���� ��Ƶ�
//����: only SVkHeader

#define SVK_INVALID_INDEX -1
#define SVK_PTR_TO_HANDLE(pHandle) pHandle ? (*pHandle) : VK_NULL_HANDLE

//index�� casting ���پ��ϸ� enum class
enum class SVkDepthStencilState : uint8_t
{
    SVk_DepthStencilState_None              = 0,
    SVk_DepthStencilState_DepthOnly         = 1,
    SVk_DepthStencilState_DepthStencil      = 2
};

//index���·� �����ϸ� �׳� enum
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
