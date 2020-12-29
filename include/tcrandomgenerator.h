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

#ifndef TCRANDOMGENERATOR_H
#define TCRANDOMGENERATOR_H
//! @file

#include <ctime>
#include "tctype.h"

//----------------------------------------------------------------------------
/*!
	擬似乱数を生成するためのクラスです。
	コピーに対応しています。
	乱数アルゴリズムには XorShift 法を使っています。
	http://en.wikipedia.org/wiki/Xorshift
*/
class TCRANDOMGENERATOR
{
	//
	// variable
	//
private:
	uint32_t			m_ValX;	//!< パラメータ X
	uint32_t			m_ValY;	//!< パラメータ Y
	uint32_t			m_ValZ;	//!< パラメータ Z
	uint32_t			m_ValW;	//!< パラメータ W

	//
	// method
	//
public:
	/*!
		デフォルトのコンストラクタです。
		各メンバは程良く初期化されます。
		@param[in] Seed 乱数生成のためのシード（0 指定時は秒カウンタを代用。省略時は 0）
	*/
	TCRANDOMGENERATOR(uint32_t Seed = 0)
		: m_ValX(Seed),
		m_ValY(362436069),
		m_ValZ(521288629),
		m_ValW(88675123)
	{
		if (Seed == 0) {
			m_ValX = static_cast<uint32_t>(time(NULL));
		}

	} // end of TCRANDOMGENERATOR::ctor

	/*!
		コピーコンストラクタです。
		各メンバは指定されたインスタンスの各メンバに初期化されます。
		コピー先のこの（this）インスタンスは、以降、コピー元のインスタンスと同じ乱数列を生成します。
		@param[in] Rhs コピー元となるインスタンス
	*/
	TCRANDOMGENERATOR(const TCRANDOMGENERATOR &Rhs)
		: m_ValX(Rhs.m_ValX),
		m_ValY(Rhs.m_ValY),
		m_ValZ(Rhs.m_ValZ),
		m_ValW(Rhs.m_ValW)
	{
	} // end of TCRANDOMGENERATOR::ctor

	/*!
		代入演算子です。
		各メンバは指定されたインスタンスの各メンバに初期化されます。
		コピー先のこの（this）インスタンスは、以降、コピー元のインスタンスと同じ乱数列を生成します。
		@param[in] Rhs コピー元となるインスタンス
		@return このインスタンス
	*/
	TCRANDOMGENERATOR &operator=(const TCRANDOMGENERATOR &Rhs)
	{
		m_ValX = Rhs.m_ValX;
		m_ValY = Rhs.m_ValY;
		m_ValZ = Rhs.m_ValZ;
		m_ValW = Rhs.m_ValW;
		return *this;

	} // end of TCRANDOMGENERATOR::operator=

	/*!
		乱数を１つ生成します。
		@return 32bit の乱数
	*/
	uint32_t	Get()
	{
		uint32_t	ValT = (m_ValX ^ (m_ValX << 11));
		m_ValX = m_ValY;
		m_ValY = m_ValZ;
		m_ValZ = m_ValW;
		m_ValW = (m_ValW ^ (m_ValW >> 19)) ^ (ValT ^ (ValT >> 8));
#if 0 && DEBUG
		//
		// 生成するたびに 0、1、、、と昇順の整数が返る。
		//
		static uint32_t	Val;
		m_ValW = Val++;
#endif
		return m_ValW;

	} // end of TCRANDOMGENERATOR::Get
};



//----------------------------------------------------------------------------
#endif	// TCRANDOMGENERATOR_H
