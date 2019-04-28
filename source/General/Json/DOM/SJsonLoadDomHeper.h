#pragma once

#include "General/SHeader.h"

#include "General/Json/SJsonFileStream.h"
#include "General/Json/SJsonConvertedFileStream.h"

//C++ Include
#include <vector>
#include <memory>

#include "ostreamwrapper.h"
#include "filereadstream.h" 
#include "istreamwrapper.h"
#include "reader.h"
#include "document.h"


class SJsonLoadDomHelper
{
public:
    template<class ReaderT, class DataT>
    static unique_ptr<DataT> LoadFromFile(const CString& path)
    {
        auto data = make_unique<DataT>();

        SJsonFileStream stream(path);
        if (stream.IsOpened())
        {
            SJsonDocument doc;
            doc.ParseStream<0, SJsonEncoding>(stream);

            ReaderT(doc, data.get());
        }
        return std::move(data);
    }

    template<class ReaderT, class DataT>
    static unique_ptr<DataT> LoadConvertedFile(const CString& path)
    {
        auto data = make_unique<DataT>();

        SJsonConvertedFileStream stream(path);
        if (stream.IsOpened())
        {
            SJsonDocument doc;
            doc.ParseStream<0, SJsonEncoding>(stream);

            ReaderT(doc, data.get());
        }
        return std::move(data);
    }
};

