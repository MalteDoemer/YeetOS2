/*
 * Copyright 2021 Malte Dömer
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Types.hpp"
#include "String.hpp"
#include "StdLib.hpp"
#include "StdLibExtras.hpp"

#include "Kernel/KernelTests.hpp"

namespace Kernel::Tests {

TEST_CASE(string_default_ctor)
{
    String str;

    EXPECT(str.is_empty());
    EXPECT_NOT_EQU(str.data(), nullptr);
    EXPECT_EQU(str, "");
    EXPECT_EQU(str, String());

    return true;
}

TEST_CASE(string_length)
{
    const char* data1 = "";
    const char* data2 = "123";
    const char* data3 = "123456789101112131415161718192021222324252627282930213134353637383940";

    String str1 = data1;
    String str2 = data2;
    String str3 = data3;

    EXPECT_EQU(str1.count(), strlen(data1));
    EXPECT_EQU(str2.count(), strlen(data2));
    EXPECT_EQU(str3.count(), strlen(data3));

    return true;
}

TEST_CASE(string_data)
{
    const char* data1 = "";
    const char* data2 = "123";
    const char* data3 = "123456789101112131415161718192021222324252627282930213134353637383940";

    String str1 = data1;
    String str2 = data2;
    String str3 = data3;

    EXPECT_EQU(strcmp(str1.data(), data1), 0);
    EXPECT_EQU(strcmp(str2.data(), data2), 0);
    EXPECT_EQU(strcmp(str3.data(), data3), 0);

    return true;
}

TEST_CASE(string_capacity)
{
    
}

TEST_CASE(string_construction)
{
    const char* data = "test test";
    auto list = { 't', 'e', 's', 't', ' ', 't', 'e', 's', 't' };

    String str1 = data;
    String str2(data);
    String str3 = list;
    String str4(list);
    String str5 = str1;
    String str6(str3);
    String tmp = String(data);
    String str7(move(tmp));
    String str8(data, strlen(data));

    EXPECT_ALL_EQU(str1, str2, str3, str4, str5, str6, str7, str8);
    return true;
}

TEST_CASE(wstring_construction)
{
    const wchar_t* data = L"test test";
    auto list = { L't', L'e', L's', L't', L' ', L't', L'e', L's', L't' };

    WString str1 = data;
    WString str2(data);
    WString str3 = list;
    WString str4(list);
    WString str5 = str1;
    WString str6(str3);
    WString tmp = WString(data);
    WString str7(move(tmp));
    WString str8(data, 9); // FIXME: wstrlen() ??

    EXPECT_ALL_EQU(str1, str2, str3, str4, str5, str6, str7, str8);
    return true;
}

TEST_CASE(string_equality_small)
{
    String str1 = "fett";
    String str2 = "felf";
    String str3 = "lol";
    String str4 = "fett";
    const char* str5 = "fett";
    const char* str6 = "nid so fett";

    EXPECT_EQU(str1, str4);
    EXPECT_EQU(str1, str5);

    EXPECT_NOT_EQU(str1, str6);
    EXPECT_NOT_EQU(str1, str2);
    EXPECT_NOT_EQU(str1, str3);

    return true;
}

TEST_CASE(string_equality_large)
{
    String str1 = "fettfettfettfettfettfettfettfettfettfettfettfettfettfettfett";
    String str2 = "felffelffelffelffelffelffelffelffelffelffelffelffelffelffelf";
    String str3 = "lol lol lol lol lol lol lol lol lol lol lol lol";
    String str4 = "fettfettfettfettfettfettfettfettfettfettfettfettfettfettfett";
    const char* str5 = "fettfettfettfettfettfettfettfettfettfettfettfettfettfettfett";
    const char* str6 = "nid so fett nid so fett nid so fett nid so fett nid so fett";

    EXPECT_EQU(str1, str4);
    EXPECT_EQU(str1, str5);

    EXPECT_NOT_EQU(str1, str6);
    EXPECT_NOT_EQU(str1, str2);
    EXPECT_NOT_EQU(str1, str3);

    return true;
}

TEST_CASE(wstring_equality_small)
{
    WString str1 = L"fett";
    WString str2 = L"felf";
    WString str3 = L"lol";
    WString str4 = L"fett";
    const wchar_t* str5 = L"fett";
    const wchar_t* str6 = L"nid so fett";

    EXPECT_EQU(str1, str4);
    EXPECT_EQU(str1, str5);

    EXPECT_NOT_EQU(str1, str6);
    EXPECT_NOT_EQU(str1, str2);
    EXPECT_NOT_EQU(str1, str3);

    return true;
}

TEST_CASE(wstring_equality_large)
{
    WString str1 = L"fettfettfettfettfettfettfettfettfettfettfettfettfettfettfett";
    WString str2 = L"felffelffelffelffelffelffelffelffelffelffelffelffelffelffelf";
    WString str3 = L"lol lol lol lol lol lol lol lol lol lol lol lol";
    WString str4 = L"fettfettfettfettfettfettfettfettfettfettfettfettfettfettfett";
    const wchar_t* str5 = L"fettfettfettfettfettfettfettfettfettfettfettfettfettfettfett";
    const wchar_t* str6 = L"nid so fett nid so fett nid so fett nid so fett nid so fett";

    EXPECT_EQU(str1, str4);
    EXPECT_EQU(str1, str5);

    EXPECT_NOT_EQU(str1, str6);
    EXPECT_NOT_EQU(str1, str2);
    EXPECT_NOT_EQU(str1, str3);

    return true;
}

}