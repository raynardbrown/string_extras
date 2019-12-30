////////////////////////////////////////////////////////////////////////////////
//
// File: CharacterEncoder.cpp
//
// Author: Raynard Brown
//
// Copyright (c) 2019 Raynard Brown
//
// All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "CharacterEncoder.h"

#include <cstring>
#include <exception>
#include <vector>

#include <windows.h>

template <typename T>
static int stringLengthValidator(typename std::vector<T>::size_type stringLength)
{
  int finalLength = 0;

  // Check to see if the string can fit into an int if not truncate the length
  // ensure to truncate enough to hold the null terminator
  // (hence the negative one).
  if(stringLength <= (INT_MAX - 1))
  {
    // The size_type to int cast is safe because we verified that size_type is
    // less than int max so there will be no loss of data.
    finalLength = static_cast<int>(stringLength);
  }
  else
  {
    // string would be larger than INT_MAX, truncate the string length
    finalLength = INT_MAX - 1;
  }

  return finalLength;
}

/* static */ std::wstring CharacterEncoder::utf8ToUtf16(const std::string& utf8String)
{
  // Check for the empty string first.
  if(utf8String == "")
  {
    return L"";
  }

  int utf8Length = stringLengthValidator<char>(utf8String.length());

  // Get the length of the destination UTF-16 buffer with the first call.
  int utf16Length = ::MultiByteToWideChar(CP_UTF8,                                // The code page that is used to perform the conversion.

                                          MB_ERR_INVALID_CHARS,                   // Fail if an invalid character is encountered.

                                          utf8String.c_str(),                     // The UTF-8 string to convert.

                                          utf8Length,                             // Size of the UTF-8 buffer in bytes excluding the NULL terminator,
                                                                                  // so that this function does not process a NULL terminator for the UTF-16
                                                                                  // string. Truncate the string if it is greater than the range of int bytes,
                                                                                  // since ::MultiByteToWideChar can only take an int for the length argument.

                                          NULL,                                   // Buffer is ignored since this function will perform a length check.

                                          0);                                     // Size of the UTF-16 string buffer.
                                                                                  // Since it is 0, this function returns the number of bytes required for the
                                                                                  // UTF-16 buffer.

  if(utf16Length == 0)
  {
    throw std::exception("Error processing the UTF-8 string");
  }

  std::vector<wchar_t> utf16StringBuffer(static_cast<std::vector<wchar_t>::size_type>(utf16Length), 0);

  // Convert the UTF-8 string to UTF-16.
  int result = ::MultiByteToWideChar(CP_UTF8,                                     // The code page that is used to perform the conversion.

                                     MB_ERR_INVALID_CHARS,                        // Fail if an invalid character is encountered.

                                     utf8String.c_str(),                          // The UTF-8 string to convert.

                                     utf8Length,                                  // Size of the UTF-8 buffer in bytes excluding the NULL terminator,
                                                                                  // so that this function does not process a NULL terminator for the UTF-16
                                                                                  // string. Truncate the string if it is greater than the range of int bytes,
                                                                                  // since ::MultiByteToWideChar can only take an int for the length argument.

                                     &utf16StringBuffer[0],                       // Buffer that receives the UTF-16 string.

                                     utf16Length);                                // Size of the UTF-16 string buffer which does not include space for the null
                                                                                  // terminator since we explictly excluded the null terminator in the length check
                                                                                  // in the first call to this function.

  if(result == 0)
  {
    throw std::exception("Error processing the UTF-8 string");
  }

  return std::wstring(utf16StringBuffer.begin(), utf16StringBuffer.end());;
}

/* static */ std::string CharacterEncoder::utf16ToUtf8(const std::wstring& utf16String)
{
  // Check for the empty string first.
  if(utf16String == L"")
  {
    return "";
  }

  DWORD flags = 0;

  OSVERSIONINFOEX osvi;
  bool isWindowsVistaOrGreater;

  std::memset(&osvi, 0, sizeof(OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

  ::GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&osvi));

  // OSVERSIONINFO structure
  // https://msdn.microsoft.com/en-us/library/windows/desktop/ms724834(v=vs.85).aspx
  isWindowsVistaOrGreater = ((osvi.dwMajorVersion > 6) ||
                             ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 0)));

  if(isWindowsVistaOrGreater)
  {
    // WideCharToMultiByte function
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130(v=vs.85).aspx
    // This flag or 0 is required on Windows Vista or later or with UTF-8. It
    // must be 0 otherwise.
    flags = WC_ERR_INVALID_CHARS;
  }
  else
  {
    flags = 0;
  }

  int utf16Length = stringLengthValidator<wchar_t>(utf16String.length());

  int utf8Length = ::WideCharToMultiByte(CP_UTF8,                                 // The code page that is used to perform the conversion.

                                         flags,                                   // Fail if an invalid character is encountered.

                                         utf16String.c_str(),                     // The UTF-16 string to convert.

                                         utf16Length,                             // Size of the UTF-8 buffer in bytes excluding the NULL terminator,
                                                                                  // so that this function does not process a NULL terminator for the UTF-8 string.
                                                                                  // Truncate the string if it is greater than the range of int bytes,
                                                                                  // since ::MultiByteToWideChar can only take an int for the length argument.

                                         NULL,                                    // Buffer is ignored for length check.

                                         0,                                       // Size of the UTF-8 string buffer.
                                                                                  // Since it is 0, this function returns the number of bytes required for
                                                                                  // the UTF-8 buffer.

                                         NULL,                                    // The default char to use if a character cannot be represented.
                                                                                  // Must be NULL for UTF-8.

                                         NULL);                                   // Has this function used a default character in the conversion.
                                                                                  // Must be NULL for UTF-8.

  if(utf8Length == 0)
  {
    throw std::exception("Error processing the UTF-16 string");
  }

  std::vector<char> utf8StringBuffer(static_cast<std::vector<char>::size_type>(utf8Length), 0);

  int result = ::WideCharToMultiByte(CP_UTF8,                                     // The code page that is used to perform the conversion.

                                     flags,                                       // Fail if an invalid character is encountered.

                                     utf16String.c_str(),                         // The UTF-8 string to convert.

                                     utf16Length,                                 // Size of the UTF-8 buffer in bytes excluding the NULL terminator,
                                                                                  // so that this function does not process a NULL terminator for the UTF-8 string.
                                                                                  // Truncate the string if it is greater than the range int bytes,
                                                                                  // since ::MultiByteToWideChar can only take an int for the length argument.

                                     &utf8StringBuffer[0],                        // Buffer that receives the UTF-8 string.

                                     utf8Length,                                  // Size of the UTF-16 string buffer.

                                     NULL,                                        // The default char to use if a character cannot be represented.
                                                                                  // Must be NULL for UTF-8.

                                     NULL);                                       // Has this function used a default character in the conversion
                                                                                  // Must be NULL for UTF-8.

  if(result == 0)
  {
    throw std::exception("Error processing the UTF-16 string");
  }

  return std::string(utf8StringBuffer.begin(), utf8StringBuffer.end());
}
