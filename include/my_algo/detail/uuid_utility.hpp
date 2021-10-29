/*
The MIT License (MIT)

Copyright (c) 2014 Graeme Hill (http://graemehill.ca)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef LEE_UTILITY_DETAIL_UUID_UTILITY_HPP__
#define LEE_UTILITY_DETAIL_UUID_UTILITY_HPP__

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <array>

#ifdef _WIN32
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

namespace lee
{
// Class to represent a GUID/UUID. Each instance acts as a wrapper around a
// 16 byte value that can be passed around by value. It also supports
// conversion to string (via the stream operator <<) and conversion from a
// string via constructor.
class uuid
{
public:
// obviously this is the windows version
#ifdef _WIN32
    static uuid GetUUID()
    {
        GUID newId;
        CoCreateGuid(&newId);

        std::array<unsigned char, 16> bytes;

        bytes[0] = (unsigned char)((newId.Data1 >> 24) & 0xFF);
        bytes[1] = (unsigned char)((newId.Data1 >> 16) & 0xFF);
        bytes[2] = (unsigned char)((newId.Data1 >> 8) & 0xFF);
        bytes[3] = (unsigned char)((newId.Data1) & 0xff);
        bytes[4] = (unsigned char)((newId.Data2 >> 8) & 0xFF);
        bytes[5] = (unsigned char)((newId.Data2) & 0xff);
        bytes[6] = (unsigned char)((newId.Data3 >> 8) & 0xFF);
        bytes[7] = (unsigned char)((newId.Data3) & 0xFF);
        bytes[8] = (unsigned char)newId.Data4[0];
        bytes[9] = (unsigned char)newId.Data4[1];
        bytes[10] = (unsigned char)newId.Data4[2];
        bytes[11] = (unsigned char)newId.Data4[3];
        bytes[12] = (unsigned char)newId.Data4[4];
        bytes[13] = (unsigned char)newId.Data4[5];
        bytes[14] = (unsigned char)newId.Data4[6];
        bytes[15] = (unsigned char)newId.Data4[7];

        return uuid(bytes);
    }
#else
    // This is the linux friendly implementation, but it could work on other
    // systems that have libuuid available
    static uuid GetUUID()
    {
        std::array<unsigned char, 16> data;
        /// assert(std::is_same<unsigned char[16], uuid_t>::value, "Wrong type!");
        uuid_generate(data.data());
        return uuid(data);
    }
#endif

public:
    uuid(const std::array<unsigned char, 16> &bytes) : _bytes(bytes) {}

    uuid(const std::string &fromString)
    {
        char charOne = '\0';
        char charTwo = '\0';
        bool lookingForFirstChar = true;
        unsigned nextByte = 0;

        for (unsigned i = 0; i < fromString.size(); ++i)
        {
            if (fromString.at(i) == '-')
                continue;

            if (nextByte >= 16 || !isValidHexChar(fromString.at(i)))
            {
                // Invalid string so bail
                zeroify();
                return;
            }

            if (lookingForFirstChar)
            {
                charOne = fromString.at(i);
                lookingForFirstChar = false;
            }
            else
            {
                charTwo = fromString.at(i);
                unsigned char byte = hexPairToChar(charOne, charTwo);
                _bytes[nextByte++] = byte;
                lookingForFirstChar = true;
            }
        }

        // if there were fewer than 16 bytes in the string then guid is bad
        if (nextByte < 16)
        {
            zeroify();
            return;
        }
    }

    uuid()
    {
        zeroify();
    }

    uuid(const uuid &other)
    {
        _bytes = other._bytes;
    }
    uuid &operator=(const uuid &other)
    {
        if (&other != this)
        {
            _bytes = other._bytes;
        }
        return *this;
    }

    bool operator==(const uuid &other) const
    {
        return other.bytes() == bytes();
    }
    bool operator!=(const uuid &other) const
    {
        return !((*this) == other);
    }

    std::string str() const
    {
        char one[10], two[6], three[6], four[6], five[14];

        snprintf(one, 10, "%02x%02x%02x%02x", _bytes[0], _bytes[1], _bytes[2], _bytes[3]);
        snprintf(two, 6, "%02x%02x", _bytes[4], _bytes[5]);
        snprintf(three, 6, "%02x%02x", _bytes[6], _bytes[7]);
        snprintf(four, 6, "%02x%02x", _bytes[8], _bytes[9]);
        snprintf(five, 14, "%02x%02x%02x%02x%02x%02x", _bytes[10], _bytes[11], _bytes[12], _bytes[13], _bytes[14],
                 _bytes[15]);
        const std::string sep("-");
        std::string out(one);

        out += sep + two;
        out += sep + three;
        out += sep + four;
        out += sep + five;

        return out;
    }
    operator std::string() const
    {
        return str();
    }
    const std::array<unsigned char, 16> &bytes() const
    {
        return _bytes;
    }
    void swap(uuid &other)
    {
        _bytes.swap(other._bytes);
    }
    bool isValid() const
    {
        uuid empty;
        return *this != empty;
    }

private:
    void zeroify()
    {
        std::fill(_bytes.begin(), _bytes.end(), static_cast<unsigned char>(0));
    }

    // actual data
    std::array<unsigned char, 16> _bytes;

    // converts a single hex char to a number (0 - 15)
    unsigned char hexDigitToChar(char ch)
    {
        // 0-9
        if (ch > 47 && ch < 58)
            return ch - 48;

        // a-f
        if (ch > 96 && ch < 103)
            return ch - 87;

        // A-F
        if (ch > 64 && ch < 71)
            return ch - 55;

        return 0;
    }

    bool isValidHexChar(char ch)
    {
        // 0-9
        if (ch > 47 && ch < 58)
            return true;

        // a-f
        if (ch > 96 && ch < 103)
            return true;

        // A-F
        if (ch > 64 && ch < 71)
            return true;

        return false;
    }

    // converts the two hexadecimal characters to an unsigned char (a byte)
    unsigned char hexPairToChar(char a, char b)
    {
        return hexDigitToChar(a) * 16 + hexDigitToChar(b);
    }

    // make the << operator a friend so it can access _bytes
    friend lee::string_stream &operator<<(lee::string_stream &s, const uuid &guid)
    {
        /// std::ios_base::fmtflags f(s.flags());  // politely don't leave the ostream in hex mode
        s << std::hex << std::setfill('0') << std::setw(2) << (int)guid._bytes[0] << std::setw(2) << (int)guid._bytes[1]
          << std::setw(2) << (int)guid._bytes[2] << std::setw(2) << (int)guid._bytes[3] << "-" << std::setw(2)
          << (int)guid._bytes[4] << std::setw(2) << (int)guid._bytes[5] << "-" << std::setw(2) << (int)guid._bytes[6]
          << std::setw(2) << (int)guid._bytes[7] << "-" << std::setw(2) << (int)guid._bytes[8] << std::setw(2)
          << (int)guid._bytes[9] << "-" << std::setw(2) << (int)guid._bytes[10] << std::setw(2) << (int)guid._bytes[11]
          << std::setw(2) << (int)guid._bytes[12] << std::setw(2) << (int)guid._bytes[13] << std::setw(2)
          << (int)guid._bytes[14] << std::setw(2) << (int)guid._bytes[15];
        /// s.flags(f);
        return s;
    }
    /// friend bool operator<(const uuid &lhs, const uuid &rhs) { return lhs.bytes() < rhs.bytes(); }
};  // namespace uuid

}  // namespace lee
#endif  ///< LEE_UTILITY_DETAIL_UUID_UTILITY_HPP__
