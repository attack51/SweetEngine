//General Include
#include "General/SInclude.h"

//C++ Include
#include <fstream>

//Header Include
#include "SJsonFileStream.h"


const SJsonFileStream::Ch SJsonFileStream::EofCh = U8Text('\0');
const istreambuf_iterator<SJsonFileStream::Ch> SJsonFileStream::EofIter = istreambuf_iterator<SJsonFileStream::Ch>();

SJsonFileStream::SJsonFileStream(const CString& fileName)
{
    m_stream = make_unique<U8IfStream>(fileName.c_str());
    if (m_stream->is_open())
    {
        m_tell = 0;
        m_iter = istreambuf_iterator<Ch>(*m_stream);
    }
    else
    {
        Close();
    }
}

SJsonFileStream::SJsonFileStream(const SString& fileName)
{
    m_stream = make_unique<U8IfStream>(fileName.c_str());
    if (m_stream->is_open())
    {
        m_tell = 0;
        m_iter = istreambuf_iterator<Ch>(*m_stream);
    }
    else
    {
        Close();
    }
}

bool SJsonFileStream::IsOpened() const
{
    return m_stream.get();
}

void SJsonFileStream::Close()
{
    m_stream.reset();
    m_iter = EofIter;
    m_tell = 0;
}

SJsonFileStream::Ch SJsonFileStream::Peek() const
{
    assert(IsOpened());

    if (m_stream->eof() || m_iter == EofIter)
    {
        return EofCh;
    }
    return *m_iter;
}

SJsonFileStream::Ch SJsonFileStream::Take()
{
    assert(IsOpened());

    if (m_stream->eof() || m_iter == EofIter)
    {
        return EofCh;
    }
    m_tell++;
    return *m_iter++;
}

size_t SJsonFileStream::Tell()
{
    assert(IsOpened());
    return m_tell;
}
