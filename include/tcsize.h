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

#ifndef TCSIZE_H
#define TCSIZE_H
//! @file

#include "tctype.h"

//----------------------------------------------------------------------------
/*!
	2 次元のサイズ、つまり幅（X 値）および高さ（Y 値）を保持するためのクラスです。
	コピー・加算・減算に対応しています。
*/
class TCSIZE
{
	//
	// assign
	//
private:
	typedef int8_t		DATA_T;

	//
	// variable
	//
private:
	DATA_T				m_Size[TC::XY];	//!< 保持しているサイズ（X 値および Y 値）

	//
	// method
	//
public:
#if 0
	/*!
		デフォルトコンストラクタです。
		各メンバはゼロで初期化されます。
	*/
	TCSIZE()
	{
		m_Size[TC::X] = 0;
		m_Size[TC::Y] = 0;

	} // end of TCSIZE::ctor
#endif

	/*!
		初期値付きのコンストラクタです。
		各メンバは指定された幅・高さで初期化されます。
		@param[in] SizeX 幅（X 値）
		@param[in] SizeY 高さ（Y 値）
	*/
	TCSIZE(DATA_T SizeX, DATA_T SizeY)
	{
		m_Size[TC::X] = SizeX;
		m_Size[TC::Y] = SizeY;

	} // end of TCSIZE::ctor

	/*!
		コピーコンストラクタです。
		各メンバは指定されたインスタンスの各メンバに初期化されます。
		@param[in] Rhs コピー元のインスタンス
	*/
	TCSIZE(const TCSIZE &Rhs)
	{
		m_Size[TC::X] = Rhs.m_Size[TC::X];
		m_Size[TC::Y] = Rhs.m_Size[TC::Y];

	} // end of TCSIZE::ctor

	/*!
		代入演算子です。
		各メンバは指定されたインスタンスの各メンバに代入されます。
		@param[in] Rhs 代入元のインスタンス
		@return このインスタンス
	*/
	TCSIZE &operator=(const TCSIZE &Rhs)
	{
		m_Size[TC::X] = Rhs.m_Size[TC::X];
		m_Size[TC::Y] = Rhs.m_Size[TC::Y];
		return *this;

	} // end of TCSIZE::operator=

	/*!
		加算演算子です。
		各メンバに指定されたインスタンスの各メンバを加算したインスタンスを返します。
		@param[in] Rhs 加算相手となるインスタンス
		@return 加算結果のインスタンス
	*/
	TCSIZE operator+(const TCSIZE &Rhs) const
	{
		TCSIZE	Value(*this);
		Value.m_Size[TC::X] = Value.m_Size[TC::X] + Rhs.m_Size[TC::X];
		Value.m_Size[TC::Y] = Value.m_Size[TC::Y] + Rhs.m_Size[TC::Y];
		return Value;

	} // end of TCSIZE::operator+

	/*!
		加算代入演算子です。
		各メンバに指定されたインスタンスの各メンバを加算します。
		@param[in] Rhs 加算相手となるインスタンス
		@return このインスタンス
	*/
	TCSIZE &operator+=(const TCSIZE &Rhs)
	{
		m_Size[TC::X] = m_Size[TC::X] + Rhs.m_Size[TC::X];
		m_Size[TC::Y] = m_Size[TC::Y] + Rhs.m_Size[TC::Y];
		return *this;

	} // end of TCSIZE::operator+=

	/*!
		減算演算子です。
		各メンバから指定されたインスタンスの各メンバを減算したインスタンスを返します。
		@param[in] Rhs 減算相手となるインスタンス
		@return 減算結果のインスタンス
	*/
	TCSIZE operator-(const TCSIZE &Rhs) const
	{
		TCSIZE	Value(*this);
		Value.m_Size[TC::X] = Value.m_Size[TC::X] - Rhs.m_Size[TC::X];
		Value.m_Size[TC::Y] = Value.m_Size[TC::Y] - Rhs.m_Size[TC::Y];
		return Value;

	} // end of TCSIZE::operator-

	/*!
		減算代入演算子です。
		各メンバから指定されたインスタンスの各メンバを減算します。
		@param[in] Rhs 減算相手となるインスタンス
		@return このインスタンス
	*/
	TCSIZE &operator-=(const TCSIZE &Rhs)
	{
		m_Size[TC::X] = m_Size[TC::X] - Rhs.m_Size[TC::X];
		m_Size[TC::Y] = m_Size[TC::Y] - Rhs.m_Size[TC::Y];
		return *this;

	} // end of TCSIZE::operator-=

	/*!
		幅（X値）を返します。
		@return このインスタンスの X 値
	*/
	DATA_T	GetX() const
	{
		return m_Size[TC::X];

	} // end of TCSIZE::GetX

	/*!
		高さ（Y値）を返します。
		@return このインスタンスの Y 値
	*/
	DATA_T	GetY() const
	{
		return m_Size[TC::Y];

	} // end of TCSIZE::GetY
};



//----------------------------------------------------------------------------
#endif	// TCSIZE_H
