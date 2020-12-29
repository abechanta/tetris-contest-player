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

#ifndef TCPOS_H
#define TCPOS_H
//! @file

#include "tctype.h"

//----------------------------------------------------------------------------
/*!
	2 次元の位置、つまり横（X値）および縦（Y値）を保持するためのクラスです。
	コピー・加算・減算に対応しています。
*/
class TCPOS
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
	DATA_T				m_Pos[TC::XY];	//!< 保持している位置（X値およびY値）

	//
	// method
	//
public:
#if 0
	/*!
		デフォルトコンストラクタです。
		各メンバはゼロで初期化されます。
	*/
	TCPOS()
	{
		m_Pos[TC::X] = 0;
		m_Pos[TC::Y] = 0;

	} // end of TCPOS::ctor
#endif

	/*!
		初期値付きのコンストラクタです。
		各メンバは指定された幅・高さで初期化されます。
		@param[in] PosX 幅（X 値）
		@param[in] PosY 高さ（Y 値）
	*/
	TCPOS(DATA_T PosX, DATA_T PosY)
	{
		m_Pos[TC::X] = PosX;
		m_Pos[TC::Y] = PosY;

	} // end of TCPOS::ctor

	/*!
		コピーコンストラクタです。
		各メンバは指定されたインスタンスの各メンバに初期化されます。
		@param[in] Rhs コピー元のインスタンス
	*/
	TCPOS(const TCPOS &Rhs)
	{
		m_Pos[TC::X] = Rhs.m_Pos[TC::X];
		m_Pos[TC::Y] = Rhs.m_Pos[TC::Y];

	} // end of TCPOS::ctor

	/*!
		代入演算子です。
		各メンバは指定されたインスタンスの各メンバに代入されます。
		@param[in] Rhs 代入元のインスタンス
		@return このインスタンス
	*/
	TCPOS &operator=(const TCPOS &Rhs)
	{
		m_Pos[TC::X] = Rhs.m_Pos[TC::X];
		m_Pos[TC::Y] = Rhs.m_Pos[TC::Y];
		return *this;

	} // end of TCPOS::operator=

	/*!
		加算演算子です。
		各メンバに指定されたインスタンスの各メンバを加算したインスタンスを返します。
		@param[in] Rhs 加算相手となるインスタンス
		@return 加算結果のインスタンス
	*/
	TCPOS operator+(const TCPOS &Rhs) const
	{
		TCPOS	Value(*this);
		Value.m_Pos[TC::X] = Value.m_Pos[TC::X] + Rhs.m_Pos[TC::X];
		Value.m_Pos[TC::Y] = Value.m_Pos[TC::Y] + Rhs.m_Pos[TC::Y];
		return Value;

	} // end of TCPOS::operator+

	/*!
		加算代入演算子です。
		各メンバに指定されたインスタンスの各メンバを加算します。
		@param[in] Rhs 加算相手となるインスタンス
		@return このインスタンス
	*/
	TCPOS &operator+=(const TCPOS &Rhs)
	{
		m_Pos[TC::X] = m_Pos[TC::X] + Rhs.m_Pos[TC::X];
		m_Pos[TC::Y] = m_Pos[TC::Y] + Rhs.m_Pos[TC::Y];
		return *this;

	} // end of TCPOS::operator+=

	/*!
		減算演算子です。
		各メンバから指定されたインスタンスの各メンバを減算したインスタンスを返します。
		@param[in] Rhs 減算相手となるインスタンス
		@return 減算結果のインスタンス
	*/
	TCPOS operator-(const TCPOS &Rhs) const
	{
		TCPOS	Value(*this);
		Value.m_Pos[TC::X] = Value.m_Pos[TC::X] - Rhs.m_Pos[TC::X];
		Value.m_Pos[TC::Y] = Value.m_Pos[TC::Y] - Rhs.m_Pos[TC::Y];
		return Value;

	} // end of TCPOS::operator-

	/*!
		減算代入演算子です。
		各メンバから指定されたインスタンスの各メンバを減算します。
		@param[in] Rhs 減算相手となるインスタンス
		@return このインスタンス
	*/
	TCPOS &operator-=(const TCPOS &Rhs)
	{
		m_Pos[TC::X] = m_Pos[TC::X] - Rhs.m_Pos[TC::X];
		m_Pos[TC::Y] = m_Pos[TC::Y] - Rhs.m_Pos[TC::Y];
		return *this;

	} // end of TCPOS::operator-=

	/*!
		横（X値）を返します。
		@return このインスタンスの X 値
	*/
	DATA_T	GetX() const
	{
		return m_Pos[TC::X];

	} // end of TCPOS::GetX

	/*!
		縦（Y値）を返します。
		@return このインスタンスの Y 値
	*/
	DATA_T	GetY() const
	{
		return m_Pos[TC::Y];

	} // end of TCPOS::GetY
};



//----------------------------------------------------------------------------
#endif	// TCPOS_H
