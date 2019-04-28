#pragma once

//General Include
#include "General/SHeader.h"

#include "General/Json/SJsonFileStream.h"
#include "General/Json/SJsonConvertedFileStream.h"
#include "General/Json/SAX/SJsonHandler.h"

//C++ Include
#include <vector>
#include <memory>


class SJsonLoadSaxHelper
{
public:
    template<class NodeT, class DataT>
    static unique_ptr<DataT> LoadFromFile(const CString& path)
    {
        auto data = make_unique<DataT>();
        SJsonHandler<NodeT, DataT> jsonHandler(data.get());

        SJsonFileStream stream(path);
        if (stream.IsOpened())
        {
            rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<>> reader;
            reader.Parse(stream, jsonHandler);
        }
        return std::move(data);
    }

    template<class NodeT, class DataT>
    static unique_ptr<DataT> LoadConvertedFile(const CString& path)
    {
        auto data = make_unique<DataT>();
        SJsonHandler<NodeT, DataT> jsonHandler(data.get());

        SJsonConvertedFileStream stream(path);
        if (stream.IsOpened())
        {
            rapidjson::GenericReader<rapidjson::UTF8<SChar>, rapidjson::UTF8<SChar>> reader;
            reader.Parse(stream, jsonHandler);
        }
        return std::move(data);
    }
};