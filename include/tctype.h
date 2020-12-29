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

#ifndef TCTYPE_H
#define TCTYPE_H
//! @file

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "tcdebug.h"

typedef unsigned char	uint8_t;	//!< 8bit 符号なし整数
typedef signed char		int8_t;		//!< 8bit 符号あり整数
typedef unsigned short	uint16_t;	//!< 16bit 符号なし整数
typedef signed short	int16_t;	//!< 16bit 符号あり整数
typedef unsigned long	uint32_t;	//!< 32bit 符号なし整数
typedef signed long		int32_t;	//!< 32bit 符号あり整数
typedef unsigned int	size_t;		//!< サイズ指定なしの符号なし整数
typedef signed int		ssize_t;	//!< サイズ指定なしの符号あり整数

namespace TC
{
	enum COORD {
		X		= 0,
		Y		= 1,
		Z		= 2,
		XYZ		= 3,

		XY		= Y + 1,
	};
};

#endif	// TCTYPE_H
