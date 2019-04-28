#define _CRT_SECURE_NO_WARNINGS

//General Include
#include "General/Resource/SFileLoader.h"

//C++ Include
#include <iostream>
#include <memory>
#include <assert.h>
#include <stdio.h>

//Extension Include
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "gli/gli.hpp"

//Header Include
#include "STextureLoader.h"


namespace STextureLoader
{

SResult LoadFromASTC(
    const CString& fileName,
    vector<uint8_t>& data,
    uint32_t& width,
    uint32_t& height,
    uint8_t& blockDimX,
    uint8_t& blockDimY)
{
    vector<uint8_t> compressed;
    if (SResult::Success != SFileLoader::LoadToVector(fileName, false, compressed))
    {
        //LOGE("Failed to read ASTC texture: %s.\n", pPath);
        return SResult::Error_IO;
    }

    if (compressed.size() < sizeof(SAstcHeader))
        return SResult::Error_Generic;

    SAstcHeader header;
    memcpy(&header, compressed.data(), sizeof(SAstcHeader));
    uint32_t magic = 
        header.magic[0] | 
        (uint32_t(header.magic[1]) << 8) | 
        (uint32_t(header.magic[2]) << 16) |
        (uint32_t(header.magic[3]) << 24);

    if (magic != ASTC_MAGIC)
    {
        //LOGE("Texture %s is not ASTC.\n", pPath);
        return SResult::Error_Generic;
    }

    if (header.blockdimZ != 1)
    {
        //LOGE("ASTC 3D textures not supported yet in Vulkan.\n");
        return SResult::Error_Generic;
    }

    blockDimX = header.blockdimX;
    blockDimY = header.blockdimY;
    
    data.clear();
    data.insert(end(data), begin(compressed) + sizeof(SAstcHeader), end(compressed));
    width = header.xsize[0] | (header.xsize[1] << 8) | (header.xsize[2] << 16);
    height = header.ysize[0] | (header.ysize[1] << 8) | (header.ysize[2] << 16);
    return SResult::Success;
}

SResult LoadFromPNG(
    const CString& fileName,
    vector<uint8_t>& data,
    uint32_t& width,
    uint32_t& height)
{
    vector<uint8_t> compressed;
    if (SResult::Success != SFileLoader::LoadToVector(fileName, false, compressed))
    {
        //LOGE("Failed to read ASTC texture: %s.\n", pPath);
        return SResult::Error_IO;
    }

    int x, y, comp;
    uint8_t* pUnsafeBuffer = stbi_load_from_memory(compressed.data(), (int)compressed.size(), &x, &y, &comp, STBI_rgb_alpha);
    if (!pUnsafeBuffer || comp != STBI_rgb_alpha)
    {
        //LOGE("Failed to decompress texture: %s.\n", pPath);
        free(pUnsafeBuffer);
        return SResult::Error_Generic;
    }

    data.resize(x * y * 4);

    int lineSize = x * 4;
    for (int i = 0; i < y; ++i)
    {
        int srcOffset = i * x * 4;
        int destOffset = (y-1-i) * x * 4;

        memcpy(data.data() + srcOffset, pUnsafeBuffer + destOffset, lineSize);
    }

    width = x;
    height = y;
    free(pUnsafeBuffer);

    return SResult::Success;
}

SResult LoadPacked(
    const CString& fileName,
    uint32_t startMipLevel,
    uint32_t maxMipLevelCount,
    vector<uint8_t>& data,
    vector<uint32_t>& mipmapDataOffset,
    vector<uint32_t>& mipmapDataSize,
    uint32_t& width,
    uint32_t& height,
    SFormat& format)
{
    gli::texture2d gliImage(gli::load(fileName.c_str()));
    if (gliImage.empty())
    {
        return SResult::Error_IO;
    }
    
    if (!gli::is_compressed(gliImage.format()) || gli::is_s3tc_compressed(gliImage.format()))
    {
        gliImage = gli::flip(gliImage);
    }

    uint32_t mipLevelCount = std::min((uint32_t)gliImage.levels(), maxMipLevelCount);
    startMipLevel = std::min(startMipLevel, mipLevelCount - 1);

    data.clear();
    width = gliImage[startMipLevel].extent().x;
    height = gliImage[startMipLevel].extent().y;
    format = static_cast<SFormat>(gliImage.format());

    if (startMipLevel == 0 && mipLevelCount == (uint32_t)gliImage.levels())
    {
        data.insert(end(data), (uint8_t*)gliImage.data(), (uint8_t*)gliImage.data() + gliImage.size());
    }
    else
    {
        for (uint32_t i = startMipLevel; i < mipLevelCount; ++i)
        {
            auto& mipImage = gliImage[i];
            uint8_t* pStart = (uint8_t*)mipImage.data();

            data.insert(end(data), pStart, pStart + mipImage.size());
        }
    }

    uint32_t curOffset = 0;
    for (uint32_t i = startMipLevel; i < mipLevelCount; ++i)
    {
        auto& mipImage = gliImage[i];
        mipmapDataOffset.push_back(curOffset);

        uint32_t curSize = (uint32_t)mipImage.size();
        curOffset += curSize;
        mipmapDataSize.push_back(curSize);
    }

    gliImage.clear();
    return SResult::Success;
}

}//End namespace STextureLoader
