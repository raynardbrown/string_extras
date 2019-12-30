////////////////////////////////////////////////////////////////////////////////
//
// File: CharacterEncoder.h
//
// Author: Raynard Brown
//
// Copyright (c) 2019 Raynard Brown
//
// All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CHARACTERENCODER_H_
#define CHARACTERENCODER_H_

#include <string>

class CharacterEncoder
{
  protected:
    CharacterEncoder();

  public:
    ~CharacterEncoder();

    /**
     * Convert the specified string from UTF-8 to UTF-16. If the specified
     * string is greater than the range of "int" in bytes, then the string is
     * truncated.
     *
     * @param[in] utf8String a string encoded in UTF-8.
     *
     * @return the specified string encoded in UTF-16.
     *
     * @throws std::exception if specified string cannot be processed.
     */
    static std::wstring utf8ToUtf16(const std::string& utf8String);

    /**
     * Convert the specified string from UTF-16 to UTF-8. If the specified
     * string is greater than the range of "int" in bytes, then the string is
     * truncated.
     *
     * @param[in] utf16String a string encoded in UTF-16.
     *
     * @return the specified string encoded in UTF-8.
     *
     * @throws std::exception if specified string cannot be processed.
     */
    static std::string utf16ToUtf8(const std::wstring& utf16String);

};

#endif /* INC_CHARACTERENCODER_H_ */
