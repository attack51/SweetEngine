//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkSemaphores.h"


SVkSemaphores::SVkSemaphores(const SVkDevice* device, int numSemaphore)
{
    m_deviceRef = device;
    InitSemaphores(numSemaphore);
}

SVkSemaphores::~SVkSemaphores()
{
    DeInitSemaphores();
}

const size_t SVkSemaphores::NumSemaphore() const
{
    return m_semaphores.size();
}

const VkSemaphore* SVkSemaphores::GetSemaphore(int index) const
{
    if (index < 0 || index >= NumSemaphore()) return nullptr;
    return &m_semaphores[index];
}

const VkSemaphore* SVkSemaphores::GetSemaphorePtr() const
{
    if (NumSemaphore() == 0) return nullptr;

    return m_semaphores.data();
}

VkSemaphoreIter SVkSemaphores::SemaphoreIter() const
{
    return m_semaphores.begin();
}

void SVkSemaphores::InitSemaphores(int numSemaphore)
{
    assert(m_deviceRef);

    m_semaphores.resize(numSemaphore);
    for (int i = 0; i < numSemaphore; ++i)
    {
        VkSemaphoreCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore;
        vkCreateSemaphore(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &semaphore);
        m_semaphores[i] = semaphore;
    }
}

void SVkSemaphores::DeInitSemaphores()
{
    assert(m_deviceRef);

    for (size_t i = 0; i < m_semaphores.size(); ++i)
    {
        vkDestroySemaphore(m_deviceRef->GetVkDevice(), m_semaphores[i], nullptr);
    }
    m_semaphores.clear();
}