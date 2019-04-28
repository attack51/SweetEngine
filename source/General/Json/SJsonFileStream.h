#pragma once

//General Include
#include "General/SHeader.h"

//C++ Include
#include <iostream>
#include <fstream>


class SJsonFileStream
{
public:
    typedef typename U8Char Ch;

    SJsonFileStream(const CString& fileName);
    SJsonFileStream(const SString& fileName);
    
    bool IsOpened() const;
    void Close();

    Ch Peek() const;
    Ch Take();
    size_t Tell();

    Ch* PutBegin() { return 0; }
    void Put(Ch) { }
    void Flush() { }
    size_t PutEnd(Ch*) { return 0; }

private:
    unique_ptr<U8IfStream> m_stream;
    istreambuf_iterator<Ch> m_iter;
    size_t m_tell;

    static const Ch EofCh;
    static const istreambuf_iterator<Ch> EofIter;
};
