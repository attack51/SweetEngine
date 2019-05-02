#pragma once

//General Include
#include "General/SHeader.h"
#include "General/SString.h"

//C++ Include
#include <vector>
#include <iostream>
#include <sstream>


template <typename T>
T process_arg(T value) noexcept
{
    return value;
}

template <typename T>
T const * process_arg(std::basic_string<T> const & value) noexcept
{
    return value.c_str();
}

template<typename ... Args>
std::string string_format(const std::string& format, Args const & ... args)
{
    const auto fmt = format.c_str();
    const size_t size = std::snprintf(nullptr, 0, fmt, process_arg(args) ...) + 1;
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, fmt, process_arg(args) ...);
    auto res = std::string(buf.get(), buf.get() + size - 1);
    return res;
}

std::vector<SString> SString_split(const SString& s, SChar delim);
std::vector<CString> CString_split(const CString& s, CChar delim);
std::vector<U8String> U8String_split(const U8String& s, U8Char delim);

//로케일 언어 변경
std::string wstring_string(const std::wstring& var);
std::wstring string_wstring(const std::string& var);
std::string wstring_utf8(const std::wstring& var);
std::wstring utf8_wstring(const std::string& var);
std::string string_utf8(const std::string& var);
std::string utf8_string(const std::string& var);
