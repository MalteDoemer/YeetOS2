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

#pragma once

using FlatPtr = __UINTPTR_TYPE__;

using Uint8 = __UINT8_TYPE__;
using Uint16 = __UINT16_TYPE__;
using Uint32 = __UINT32_TYPE__;
using Uint64 = __UINT64_TYPE__;

using Int8 = __INT8_TYPE__;
using Int16 = __INT16_TYPE__;
using Int32 = __INT32_TYPE__;
using Int64 = __INT64_TYPE__;

using size_t = __SIZE_TYPE__;

constexpr Uint64 KiB = 1024;
constexpr Uint64 MiB = KiB * KiB;
constexpr Uint64 GiB = KiB * KiB * KiB;
constexpr Uint64 TiB = KiB * KiB * KiB * KiB;
constexpr Uint64 PiB = KiB * KiB * KiB * KiB * KiB;
constexpr Uint64 EiB = KiB * KiB * KiB * KiB * KiB * KiB;
