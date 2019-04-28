#pragma once

//General Include
#include "General/SHeader.h"

//C++ Include
#include <iostream>
#include <fstream>

//CString a = "한" => ascii c7/d1 한글
//U8String b = u8"한" => utf8 ea/95/9c 한글 (제대로 안나옴)
//SString c = L"한" => multi byte d55c 한글 
//한글ascii 로 저장된 파일은 c7/d1 으로 제대로 나옴
//utf8로 저장한 파일은 ea/95/9c로 나옴
//utf8로 저장하고 wchar로 convert하면 d55c가 됨
//d55c를 wCString2CString 하면 c7d1이 됨
//watch 에서 볼때 var,s8 하면 utf8 제대로 나옴

//todo : utf8에서 한글 제대로 읽어지게 하기

class SJsonConvertedFileStream
{
public:
    typedef typename SChar Ch;

    SJsonConvertedFileStream(const CString& fileName);
    SJsonConvertedFileStream(const SString& fileName);

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
    unique_ptr<SIfStream> m_stream;
    istreambuf_iterator<Ch> m_iter;
    size_t m_tell;

    static const Ch EofCh;
    static const istreambuf_iterator<Ch> EofIter;
};
