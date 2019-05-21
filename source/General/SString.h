#pragma once

#include <string>

typedef typename char CChar;
typedef typename std::string  CString;
typedef typename std::stringstream CStringStream;
typedef typename std::ifstream CIfStream;

typedef typename char U8Char;
typedef typename std::string  U8String;
typedef typename std::stringstream U8StringStream;
typedef typename std::ifstream U8IfStream;

typedef typename wchar_t SChar;
typedef typename std::wstring  SString;
typedef typename std::wstringstream SStringStream;
typedef typename std::wifstream SIfStream;

#define CStrCmp strcmp 
#define CAtoD(str) atof(str)
#define CAtoF(str) (float)atof(str)
#define CAtoI(str) atoi(str)
#define CText(str) str


#define U8StrCmp strcmp
#define U8AtoD(str) atof(str)
#define U8AtoF(str) (float)atof(str)
#define U8AtoI(str) atoi(str)
#define U8Text(str) u8##str

#define SStrCmp wcscmp
#define SAtoD(str) wcstod(str,nullptr)
#define SAtoF(str) wcstof(str,nullptr)
#define SAtoI(str) (int)wcstol(str,nullptr)
#define SText(str) L##str

//cstr -> 
#define CStrToSStr(in) string_wstring(in)
#define CStrToU8Str(in) string_utf8(in)

//sstr ->
#define SStrToCStr(in) wstring_string(in)
#define SStrToU8Str(in) wstring_utf8(in)

//u8 ->
#define U8StrToCStr(in) utf8_string(in)
#define U8StrToSStr(in) utf8_wstring(in)

static CString EmptyCString = CText("");
static U8String EmptyU8String = U8Text("");
static SString EmptySString = SText("");
