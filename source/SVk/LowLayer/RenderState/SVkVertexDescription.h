#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>


class SVkVertexDescription
{
public:
// Begin public funtions

    SVkVertexDescription(vector<SVertexFormat> vertexFormats);
    ~SVkVertexDescription();

    const VkVertexInputBindingDescription& GetVkBindingDescription() const { return m_bindingDescription; }
    const vector<VkVertexInputAttributeDescription>& GetVkAttributeDescriptions() const { return m_attributeDescriptions; }
// ~End public funtions

private:
// Begin private funtions

// ~End private funtions

private:
// Begin private fields
    
    VkVertexInputBindingDescription             m_bindingDescription    = {};
    vector<VkVertexInputAttributeDescription>   m_attributeDescriptions  = {};
// ~End private fields

private:
};