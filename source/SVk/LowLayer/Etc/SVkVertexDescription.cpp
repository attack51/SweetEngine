//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkVertexDescription.h"


SVkVertexDescription::SVkVertexDescription(vector<SVertexFormat> vertexFormats)
{
    m_attributeDescriptions.reserve(vertexFormats.size());

    uint32_t stride = 0;
    for(uint32_t i=0; i<(uint32_t)vertexFormats.size(); ++i)
    {
        VkFormat format = VertexFormatToVkFormat(vertexFormats[i]);

        VkVertexInputAttributeDescription attribute{};
        attribute.binding = 0;
        attribute.location = i;
        attribute.format = format;
        attribute.offset = stride;
        m_attributeDescriptions.push_back(attribute);

        stride += GetFormatBytes(format);
    }

    m_bindingDescription.binding = 0;
    m_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    m_bindingDescription.stride = stride;
}

SVkVertexDescription::~SVkVertexDescription()
{
}
