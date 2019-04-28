//General Include
#include "General/SInclude.h"

//C++ Include
#include <locale>
#include <codecvt>
#include <fstream>

//Header Include
#include "SJsonConvertedFileStream.h"


const SJsonConvertedFileStream::Ch SJsonConvertedFileStream::EofCh = SText('\0');
const istreambuf_iterator<SJsonConvertedFileStream::Ch> SJsonConvertedFileStream::EofIter = 
    istreambuf_iterator<SJsonConvertedFileStream::Ch>();

SJsonConvertedFileStream::SJsonConvertedFileStream(const CString& fileName)
{
    m_stream = make_unique<SIfStream>(fileName.c_str());
    if (m_stream->is_open())
    {
        typedef std::codecvt_utf8<Ch> converter_type;

        //stream 소멸자에서 삭제하기 때문에 따로 삭제하면 안됨
        auto* converter = new converter_type;
        const std::locale utf8_locale = std::locale(std::locale::empty(), converter);

        m_stream->imbue(utf8_locale);

        m_tell = 0;
        m_iter = istreambuf_iterator<Ch>(*m_stream);
    }
    else
    {
        Close();
    }
}

SJsonConvertedFileStream::SJsonConvertedFileStream(const SString& fileName)
{
    m_stream = make_unique<SIfStream>(fileName.c_str());
    if (m_stream->is_open())
    {
        typedef std::codecvt_utf8<Ch> converter_type;

        //stream 소멸자에서 삭제하기 때문에 따로 삭제하면 안됨
        auto* converter = new converter_type;
        const std::locale utf8_locale = std::locale(std::locale::empty(), converter);

        m_stream->imbue(utf8_locale);

        m_tell = 0;
        m_iter = istreambuf_iterator<Ch>(*m_stream);
    }
    else
    {
        Close();
    }
}

bool SJsonConvertedFileStream::IsOpened() const
{
    return m_stream.get();
}

void SJsonConvertedFileStream::Close()
{
    m_stream.reset();
    m_iter = EofIter;
    m_tell = 0;
}

SJsonConvertedFileStream::Ch SJsonConvertedFileStream::Peek() const
{
    assert(IsOpened());

    if (m_stream->eof() || m_iter == EofIter)
    {
        return EofCh;
    }
    return *m_iter;
}

SJsonConvertedFileStream::Ch SJsonConvertedFileStream::Take()
{
    assert(IsOpened());

    if (m_stream->eof() || m_iter == EofIter)
    {
        return EofCh;
    }
    m_tell++;
    return *m_iter++;
}

size_t SJsonConvertedFileStream::Tell()
{
    assert(IsOpened());
    return m_tell;
}
