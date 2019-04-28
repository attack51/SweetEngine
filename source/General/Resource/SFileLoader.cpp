#define _CRT_SECURE_NO_WARNINGS

//C++ Include
#include <iostream>
#include <memory>
#include <assert.h>
#include <stdio.h>
#include <locale>
#include <codecvt>
#include <fstream>

//Header Include
#include "SFileLoader.h"

namespace SFileLoader
{

SResult LoadToUPtr(const CString& fileName, unique_ptr<uint8_t>& buffer, size_t& fileSize)
{
    FILE *fp = fopen(fileName.c_str(), "rb");
    if (!fp)
    {
        return SResult::Error_IO;
    }

    size_t retval;
    long int size;

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    buffer = unique_ptr<uint8_t>((uint8_t*)malloc(size + 1));
    buffer.get()[size] = 0; //Null character for text file

    retval = fread(buffer.get(), size, 1, fp);
    assert(retval == 1);

    fileSize = size;
    fclose(fp);

    return SResult::Success;
}

SResult LoadToVector(const CString& fileName, bool isText, vector<uint8_t>& buffer)
{
    FILE *fp = fopen(fileName.c_str(), "rb");
    if (!fp) 
    {
        return SResult::Error_IO;
    }

    size_t retval;
    long int size;

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    if (isText)
    {
        buffer.resize(size + 1);
        buffer[size] = 0;
    }
    else
    {
        buffer.resize(size);
    }

    retval = fread(buffer.data(), size, 1, fp);
    assert(retval == 1);

    fclose(fp);

    return SResult::Success;
}

SResult LoadToSString(const CString& fileName, SString& str)
{
    SIfStream stream(fileName.c_str());
    if (stream.is_open())
    {
        typedef std::codecvt_utf8<SChar> converter_type;

        //stream 소멸자에서 삭제하기 때문에 따로 삭제하면 안됨
        auto* converter = new converter_type;
        const std::locale utf8_locale = std::locale(std::locale::empty(), converter);

        stream.imbue(utf8_locale);
      
        str = SString((std::istreambuf_iterator<SChar>(stream)), std::istreambuf_iterator<SChar>());

        stream.close();
        return SResult::Success;
    }
    return SResult::Error_IO;
}

SResult LoadToCString(const CString& fileName, CString& str)
{
    CIfStream stream(fileName.c_str());
    if (stream.is_open())
    {
        str = CString((std::istreambuf_iterator<CChar>(stream)), std::istreambuf_iterator<CChar>());

        stream.close();
        return SResult::Success;
    }
    return SResult::Error_IO;
}

SResult LoadU8ToSString(const CString& fileName, SString& str)
{
    SIfStream stream(fileName.c_str());
    if (stream.is_open())
    {
        typedef std::codecvt_utf8<wchar_t> converter_type;

        //stream 소멸자에서 삭제하기 때문에 따로 삭제하면 안됨
        auto* converter = new converter_type;
        const std::locale utf8_locale = std::locale(std::locale::empty(), converter);

        stream.imbue(utf8_locale);
        str = SString((std::istreambuf_iterator<wchar_t>(stream)), std::istreambuf_iterator<wchar_t>());

        stream.close();
        return SResult::Success;
    }
    return SResult::Error_IO;
}

}//End namepace SFileLoader
