#pragma once

//General Include
#include "General/SHeader.h"
#include "General/SString.h"

//C++ Include
#include <vector>

namespace SFileLoader
{

SResult LoadToUPtr(const CString& fileName, unique_ptr<uint8_t>& buffer, size_t& fileSize);
SResult LoadToVector(const CString& fileName, bool isText, vector<uint8_t>& buffer);

SResult LoadToSString(const CString& fileName, SString& str);
SResult LoadToCString(const CString& fileName, CString& str);
SResult LoadU8ToSString(const CString& fileName, SString& str);

}//End namepace SFileLoader
