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

#ifndef TCBIT_H
#define TCBIT_H
//! @file

#include "tctype.h"

//----------------------------------------------------------------------------
/*!
	ビット演算をまとめたクラスです。
*/
namespace TCBIT
{
	//
	// assign
	//

	//
	// variable
	//

	//
	// method
	//
	/*!
		与えられた特定ビットのみが On である 32bit 整数を返します。
		@param[in] idx ビット番号
		@retval 当該ビットだけが立っている整数（idx が 0～31 のとき）
		@retval 0（idx が範囲外のとき）
	*/
	uint32_t	Get(size_t idx)
	{
		static const uint32_t	BitTbl[] = {
			1U << 0,	1U << 1,	1U << 2,	1U << 3,	1U << 4,	1U << 5,	1U << 6,	1U << 7,
			1U << 8,	1U << 9,	1U << 10,	1U << 11,	1U << 12,	1U << 13,	1U << 14,	1U << 15,
			1U << 16,	1U << 17,	1U << 18,	1U << 19,	1U << 20,	1U << 21,	1U << 22,	1U << 23,
			1U << 24,	1U << 25,	1U << 26,	1U << 27,	1U << 28,	1U << 29,	1U << 30,	1U << 31,
		};
		return (idx >= 32) ? 0 : BitTbl[idx];

	} // end of TCBIT::Get

	/*!
		与えられたビット幅のすべてが On である 32bit 整数を返します。
		@param[in] width ビット番号（1～32）
		@retval 当該ビット幅が立っている整数（width が 1～32 のとき）
		@retval 00000000h（width が 0 のとき）
		@retval FFFFFFFFh（width が 32 より大きいとき）
	*/
	uint32_t	GetMask(size_t width)
	{
		uint32_t	rv = 0;

		if (width > 32) {
			width = 32;
		}
		for (size_t ii = 0; ii < width; ii++) {
			rv |= Get(ii);
		}
		return rv;

	} // end of TCBIT::GetMask

	/*!
		与えられた数値を左方向へ論理シフトした数値を返します。
		シフト回数に負数を指定すると、右方向へ論理シフトします。
		@param[in] bitmap 入力とする数値
		@param[in] count 論理シフトする回数（正数なら左方向、負数なら右方向）
		@return 入力数値を論理シフトした数値
	*/
	uint32_t	Shift(uint32_t bitmap, ssize_t count)
	{
		if (count > 0) {
			return bitmap << count;
		}
		if (count < 0) {
			return bitmap >> -count;
		}
		return bitmap;

	} // end of TCBIT::Shift

	/*!
		与えられた数値に含まれる On ビットの数を返します。
		@param[in] bitmap 入力とする数値
		@param[in] width 入力とする数値のうち、On でないことが自明であるビット数を除いたビット幅（1～32、省略時は 32）
		@return On ビットの数
	*/
	size_t	Count(uint32_t bitmap, size_t width = 32)
	{
		size_t	rv = 0;
		if (width > 32) {
			width = 32;
		}
		for (size_t ii = 0; ii < width; ii++) {
			rv += (bitmap & 0x00000001U) ? 1 : 0;
			bitmap >>= 1;
		}
		return rv;

	} // end of TCBIT::Count
};



//----------------------------------------------------------------------------
#endif	// TCBIT_H
