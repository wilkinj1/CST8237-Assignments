#pragma once

#include <string>

class StringUtils
{
public:
  static std::string GetCStringFromWideString(const wchar_t* wString);
  static std::wstring GetWideStringFromCString(const char *cString);
};