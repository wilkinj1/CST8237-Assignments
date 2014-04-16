#include "stdafx.h"
#include "StringUtils.h"

std::string StringUtils::GetCStringFromWideString(const wchar_t* wString)
{
  int stringLength = lstrlen(wString);
  char *returnString = new char[stringLength + 1];
  wcstombs(returnString, wString, stringLength);
  returnString[stringLength] = '\0';

  std::string composedString = returnString;
  delete[] returnString;
  return composedString;
}

std::wstring StringUtils::GetWideStringFromCString(const char *cString)
{
  int stringLength = strlen(cString);
  wchar_t *returnString = new wchar_t[stringLength + 1];
  mbstowcs(returnString, cString, stringLength);
  returnString[stringLength] = L'\0';

  std::wstring composedString = returnString;
  delete[] returnString;
  return composedString;
}