#define _CRT_SECURE_NO_WARNINGS

//C++ Include
#include <iostream>
#include <memory>
#include <assert.h>
#include <stdio.h>
#include <locale>
#include <codecvt>

//Header Include
#include "SUtil.h"


std::vector<SString> SString_split(const SString& s, SChar delim)
{
    std::vector<SString> result;
    SStringStream ss(s);
    SString item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

std::vector<CString> CString_split(const CString& s, CChar delim)
{
    std::vector<CString> result;
    CStringStream ss(s);
    CString item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

std::vector<U8String> U8String_split(const U8String& s, U8Char delim)
{
    std::vector<U8String> result;
    U8StringStream ss(s);
    U8String item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

std::string wstring_string(const std::wstring& var)
{
    static std::locale loc("");
    auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
    return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(var);
}

std::wstring string_wstring(const std::string& var)
{
    static std::locale loc("");
    auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
    return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(var);
}

std::string wstring_utf8(const std::wstring& var)
{
    typedef std::codecvt_utf8<wchar_t> converter_type;
    auto converter = new converter_type;
    const std::locale utf8_locale = std::locale(std::locale::empty(), converter);

    auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(utf8_locale);
    return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(var);
}

std::wstring utf8_wstring(const std::string& var)
{
    typedef std::codecvt_utf8<wchar_t> converter_type;
    auto converter = new converter_type;
    const std::locale utf8_locale = std::locale(std::locale::empty(), converter);

    auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(utf8_locale);
    return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(var);
}

std::string string_utf8(const std::string& var)
{
    std::wstring wstr = string_wstring(var);
    return wstring_utf8(wstr);
}

std::string utf8_string(const std::string& var)
{
    std::wstring wstr = utf8_wstring(var);
    return wstring_string(wstr);
}
