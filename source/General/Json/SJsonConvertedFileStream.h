#pragma once

//General Include
#include "General/SHeader.h"

//C++ Include
#include <iostream>
#include <fstream>

//CString a = "��" => ascii c7/d1 �ѱ�
//U8String b = u8"��" => utf8 ea/95/9c �ѱ� (����� �ȳ���)
//SString c = L"��" => multi byte d55c �ѱ� 
//�ѱ�ascii �� ����� ������ c7/d1 ���� ����� ����
//utf8�� ������ ������ ea/95/9c�� ����
//utf8�� �����ϰ� wchar�� convert�ϸ� d55c�� ��
//d55c�� wCString2CString �ϸ� c7d1�� ��
//watch ���� ���� var,s8 �ϸ� utf8 ����� ����

//todo : utf8���� �ѱ� ����� �о����� �ϱ�

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
