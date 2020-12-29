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

#ifndef TCVIEW_H
#define TCVIEW_H
//! @file

#include <vector>
#include "tctype.h"
#include "tcsize.h"

//----------------------------------------------------------------------------
/*!
	画面出力のインタフェースです。
	プログラマが新たな画面出力を実装するときは、このインタフェースに準拠している必要があります。
	このインタフェースは以下の画面出力機能を提供します。
		・セルマップ形式の描画
		・セルフォントの描画
	このインタフェースは、０～２５５の範囲のセル＃だけを扱います。
*/
class TCVIEW
{
	//
	// assign
	//
public:
	typedef uint8_t				CELL;
	typedef CELL				(_cdecl *TRANSFUNC)(char);

	/*!
		画面出力におけるマップ座標を格納するためのクラスです。
		コピー・代入・加算に対応しています。
	*/
	class LOCATOR
	{
		//
		// variable
		//
	public:
		ssize_t	m_Pos[TC::XYZ];	//!< 座標値（X=横方向、Y=縦方向、Z=奥行き方向。座標系は実装依存）

		//
		// method
		//
	public:
		/*!
			初期値付きのコンストラクタです。
			各メンバは指定された XYZ 値で初期化されます。
			@param[in] PosX 横位置（X 値）
			@param[in] PosY 縦位置（Y 値）
			@param[in] PosZ 奥位置（Z 値）
		*/
		LOCATOR(ssize_t PosX, ssize_t PosY, ssize_t PosZ)
		{
			m_Pos[TC::X] = PosX;
			m_Pos[TC::Y] = PosY;
			m_Pos[TC::Z] = PosZ;

		} // end of TCVIEW::LOCATOR::ctor

		/*!
			コピーコンストラクタです。
			各メンバは指定されたインスタンスの各メンバに初期化されます。
			@param[in] Rhs コピー元のインスタンス
		*/
		LOCATOR(const LOCATOR &Rhs)
		{
			m_Pos[TC::X] = Rhs.m_Pos[TC::X];
			m_Pos[TC::Y] = Rhs.m_Pos[TC::Y];
			m_Pos[TC::Z] = Rhs.m_Pos[TC::Z];

		} // end of TCVIEW::LOCATOR::ctor

		/*!
			代入演算子です。
			各メンバは指定されたインスタンスの各メンバに初期化されます。
			@param[in] Rhs コピー元のインスタンス
		*/
		LOCATOR &operator=(const LOCATOR &Rhs)
		{
			m_Pos[TC::X] = Rhs.m_Pos[TC::X];
			m_Pos[TC::Y] = Rhs.m_Pos[TC::Y];
			m_Pos[TC::Z] = Rhs.m_Pos[TC::Z];
			return *this;

		} // end of TCVIEW::LOCATOR::operator=

		/*!
			加算演算子です。
			各メンバに指定されたインスタンスの各メンバを加算したインスタンスを返します。
			@param[in] Rhs 加算相手となるインスタンス
			@return 加算結果のインスタンス
		*/
		LOCATOR operator+(const LOCATOR &Rhs) const
		{
			LOCATOR	Value(*this);
			Value.m_Pos[TC::X] += Rhs.m_Pos[TC::X];
			Value.m_Pos[TC::Y] += Rhs.m_Pos[TC::Y];
			Value.m_Pos[TC::Z] += Rhs.m_Pos[TC::Z];
			return Value;

		} // end of TCVIEW::LOCATOR::operator+

		/*!
			加算代入演算子です。
			各メンバに指定されたインスタンスの各メンバを加算します。
			@param[in] Rhs 加算相手となるインスタンス
			@return このインスタンス
		*/
		LOCATOR &operator+=(const LOCATOR &Rhs)
		{
			m_Pos[TC::X] += Rhs.m_Pos[TC::X];
			m_Pos[TC::Y] += Rhs.m_Pos[TC::Y];
			m_Pos[TC::Z] += Rhs.m_Pos[TC::Z];
			return *this;

		} // end of TCVIEW::LOCATOR::operator+=

		/*!
			減算演算子です。
			各メンバに指定されたインスタンスの各メンバを減算したインスタンスを返します。
			@param[in] Rhs 減算相手となるインスタンス
			@return 減算結果のインスタンス
		*/
		LOCATOR operator-(const LOCATOR &Rhs) const
		{
			LOCATOR	Value(*this);
			Value.m_Pos[TC::X] -= Rhs.m_Pos[TC::X];
			Value.m_Pos[TC::Y] -= Rhs.m_Pos[TC::Y];
			Value.m_Pos[TC::Z] -= Rhs.m_Pos[TC::Z];
			return Value;

		} // end of TCVIEW::LOCATOR::operator-

		/*!
			減算代入演算子です。
			各メンバに指定されたインスタンスの各メンバを減算します。
			@param[in] Rhs 減算相手となるインスタンス
			@return このインスタンス
		*/
		LOCATOR &operator-=(const LOCATOR &Rhs)
		{
			m_Pos[TC::X] -= Rhs.m_Pos[TC::X];
			m_Pos[TC::Y] -= Rhs.m_Pos[TC::Y];
			m_Pos[TC::Z] -= Rhs.m_Pos[TC::Z];
			return *this;

		} // end of TCVIEW::LOCATOR::operator-=
	};

	//
	// variable
	//

	//
	// method
	//
public:
	/*!
		画面描画を担うクラスのインスタンスです。
		@param[in] Width マップの幅（X 値）
		@param[in] Height マップの高さ（Y 値）
		@param[in] Layer マップの奥行き（Z 値）
	*/
	TCVIEW(ssize_t Width, ssize_t Height, ssize_t Layer) {}

	/*!
		デストラクタです。
	*/
	virtual ~TCVIEW() {}

	/*!
		マップのサイズを返します。
		奥行きは考慮されません。
		@return マップのサイズ
	*/
	virtual	TCSIZE	GetSize() const = 0;

	/*!
		画面全体を消去します（例えばマップ全体がセル＃がゼロのセルでフィルされます）。
	*/
	virtual	void	Clear() = 0;

	/*!
		画面を描画対象（例えばコンソール）に描画します。
	*/
	virtual	void	Paint() = 0;

	/*!
		マップの一部を書き換えます。
		指定された座標の 1x1 サイズのマップを指定のセル＃に書き換えます。
		指定された座標が画面範囲外の場合は何もしません。
		@param[in] Loc セルを配置する座標
		@param[in] Cid セル＃
	*/
	virtual	void	Draw(const LOCATOR &Loc, CELL Cid) = 0;

	/*!
		マップの一部を書き換えます。
		指定された座標の 1x1 サイズのマップを指定のセル＃に書き換えます。
		指定された座標が画面範囲外の場合は何もしません。
		@param[in] Loc セルを配置する座標
		@param[in] Chr 文字コード
		@param[in] pfTrans セル＃変換関数（NULL 指定時は変換しない）
	*/
	virtual	void	DrawFont(const LOCATOR &Loc, char Chr, TRANSFUNC pfTrans = NULL) = 0;

	/*!
		マップの一部を書き換えます。
		指定された座標の 1xN サイズのマップを指定の値に書き換えます。
		指定された座標が画面範囲外の場合は何もしません。
		@param[in] Loc セルを配置する座標
		@param[in] CidLine セル＃（X 方向の配列）
	*/
	virtual	void	Draw(const LOCATOR &Loc, const std::vector<CELL> &CidLine) = 0;

	/*!
		マップの一部を書き換えます。
		指定された座標の 1xN サイズのマップを指定の値に書き換えます。
		指定された座標が画面範囲外の場合は何もしません。
		@param[in] Loc セルを配置する座標
		@param[in] ChrLine 文字列（X 方向の配列）
		@param[in] pfTrans セル＃変換関数（NULL 指定時は変換しない）
	*/
	virtual	void	DrawFont(const LOCATOR &Loc, const std::string &ChrLine, TRANSFUNC pfTrans = NULL) = 0;

	/*!
		マップの一部を書き換えます。
		指定された座標の MxN サイズのマップを指定の値に書き換えます。
		指定された座標が画面範囲外の場合は何もしません。
		@param[in] Loc セルを配置する座標
		@param[in] CidRect セル＃（X 方向×Y 方向の２次元配列）
	*/
	virtual	void	Draw(const LOCATOR &Loc, const std::vector<std::vector<CELL> > &CidRect) = 0;

	/*!
		マップの一部を書き換えます。
		指定された座標の MxN サイズのマップを指定の値に書き換えます。
		指定された座標が画面範囲外の場合は何もしません。
		@param[in] Loc セルを配置する座標
		@param[in] ChrRect 文字列の配列
		@param[in] pfTrans セル＃変換関数（NULL 指定時は変換しない）
	*/
	virtual	void	DrawFont(const LOCATOR &Loc, const std::vector<std::string> &ChrRect, TRANSFUNC pfTrans = NULL) = 0;

	/*!
		マップの一部を書き換えるための、引数変換ヘルパです。
		文字コードをセル＃に置換しながらマップを書き換えたいときに利用することができます。
		@param[in] Chr 文字
		@return セル＃
	*/
	static	CELL	Trans_Default(char Chr)
	{
		return static_cast<CELL>(Chr);

	} // end of TCVIEW::Trans_Default
};



//----------------------------------------------------------------------------
#endif // TCVIEW_H
