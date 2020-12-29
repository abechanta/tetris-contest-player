//
// Copyright (c) 2010, Abe Chanta
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef TCDEBUG_H
#define TCDEBUG_H
//! @file

#include <iostream>

#if defined(_DEBUG)
#define DEBUG		1
#else
#define DEBUG		0
#endif

#define TCBREAK\
	_asm	int		3

#if 1
//
// Release 時は、これらの warning は intentional として扱う。
//
#pragma warning(disable:4100)	// warning C4100: '<varname>' : 引数は関数の本体部で 1 度も参照されません。
#pragma warning(disable:4127)	// warning C4127: 条件式が定数です。
#endif

#define TCASSERT(cond, message)\
{\
	if (!(cond)) {\
		std::cerr << __FUNCTION__ << '@' << __LINE__ << "(" << #cond << "): " << (message) << std::endl;\
	}\
}

#define TCVERBOSE(message)\
{\
	{\
		std::cerr << __FUNCTION__ << '@' << __LINE__ << "(VERBOSE): " << (message) << std::endl;\
	}\
}

#endif	// TCDEBUG_H
