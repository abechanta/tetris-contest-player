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

#ifndef TCFIELD_H
#define TCFIELD_H
//! @file

#include <vector>
#include "tctype.h"
#include "tcbit.h"
#include "tcpiece.h"
#include "tcpos.h"
#include "tcsize.h"

//----------------------------------------------------------------------------
/*!
	フィールドの状態を保持しているクラスです。
	どの箇所が埋まっているかを確認したり、ピースを配置してみたりすることができます。
	コピー・代入に対応しています。
*/
class TCFIELD
{
	//
	// assign
	//
private:
	static const int8_t	FIELD_WIDTH_MIN = TCPIECE::PIECE_WIDTH;
	static const int8_t	FIELD_WIDTH_MAX = 30;	// because of impl

	//
	// variable
	//
private:
	size_t					m_Gravity;		//!< 重力係数（1=0.1G、10=1G、200=20G）
	TCSIZE					m_FieldSize;	//!< フィールドのサイズ（枠を含む）
	std::vector<uint32_t>	m_Field;		//!< フィールドのビットマップ（枠を含む）
	uint32_t				m_LineFilled;	//!< １ラインが埋まった状態（枠を含まない）

	//
	// method
	//
public:
	/*!
		標準のコンストラクタです。
		標準的なフィールドサイズは Width=10、Height=18 です。
		@param[in] Width 枠を含まないフィールドの幅
		@param[in] Height 枠を含まないフィールドの高さ
		@param[in] Gravity フィールドの落下重力（1=0.1G）
	*/
	TCFIELD(int8_t Width, int8_t Height, size_t Gravity = 1)
		: m_Gravity(Gravity),
		m_FieldSize(0, 0),
		m_Field(),
		m_LineFilled(0)
	{
		if (Width < FIELD_WIDTH_MIN) {
			Width = FIELD_WIDTH_MIN;
		}
		if (Width >= FIELD_WIDTH_MAX) {
			Width = FIELD_WIDTH_MAX;
		}
		if (Height < TCPIECE::PIECE_WIDTH) {
			Height = TCPIECE::PIECE_WIDTH;
		}
		m_FieldSize = TCSIZE(Width + 2, Height + 2);	// +2 is for frame

		m_LineFilled = TCBIT::GetMask(Width) << 1;
		uint32_t	LineFrame = ((m_LineFilled << 1) | (m_LineFilled >> 1)) & ~m_LineFilled;

		m_Field.resize(m_FieldSize.GetY());
		for (int8_t yy = 0; yy < m_FieldSize.GetY(); yy++) {
			m_Field[yy] = LineFrame;
		}
		m_Field[0                     ] |= m_LineFilled;
		m_Field[m_FieldSize.GetY() - 1] |= m_LineFilled;

	} // end of TCFIELD::ctor

	/*!
		コピーコンストラクタです。
		各メンバはコピー元のインスタンスに初期化されます。
		@param[in] Rhs コピー元のインスタンス
	*/
	TCFIELD(const TCFIELD &Rhs)
		: m_Gravity(Rhs.m_Gravity),
		m_FieldSize(Rhs.m_FieldSize),
		m_Field(Rhs.m_Field),
		m_LineFilled(Rhs.m_LineFilled)
	{
	} // end of TCFIELD::ctor

	/*!
		代入演算子です。
		各メンバはコピー元のインスタンスに初期化されます。
		@param[in] Rhs コピー元のインスタンス
		@return このインスタンス
	*/
	TCFIELD	&operator=(const TCFIELD &Rhs)
	{
		m_Gravity    = Rhs.m_Gravity;
		m_FieldSize  = Rhs.m_FieldSize;
		m_Field      = Rhs.m_Field;
		m_LineFilled = Rhs.m_LineFilled;
		return *this;

	} // end of TCFIELD::operator=

	/*!
		ゲームを開始するため、フィールドの状態を初期化します。
	*/
	void	StartGame()
	{
		for (int8_t yy = 1; yy < m_FieldSize.GetY() - 1; yy++) {
			m_Field[yy] &= ~m_LineFilled;
		}

	} // end of TCFIELD::StartGame

	/*!
		重力係数を取得します。
		@return 重力係数
	*/
	size_t	GetGravity() const
	{
		return m_Gravity;

	} // end of TCFIELD::GetGravity

	/*!
		重力係数を設定します。
		プレイ中に変更することも可能です。
		@param[in] Gravity 重力係数
	*/
	void	SetGravity(size_t Gravity)
	{
		m_Gravity = Gravity;

	} // end of TCFIELD::SetGravity

	/*!
		枠を含むフィールドのサイズを取得します。
		枠のサイズは、幅が左右で＋２、高さが上下で＋２です。
		@return 枠を含むフィールドのサイズ
	*/
	TCSIZE	GetSize() const
	{
		return m_FieldSize;

	} // end of TCFIELD::GetFieldSize

	/*!
		フィールドの１段分のビットマップ（形状）を取得します。
		フィールド最下端の１段（Y=0）は枠（地面）です。
		フィールド最上端の１段は枠（天井）です。
		フィールド下側の枠のすぐ内側の段は、Y=1 です。
		フィールド最右端の１列（得られたビットマップの bit0）は枠（右壁）です。
		フィールド最左端の１列は枠（左壁）です。
		フィールド右側の枠のすぐ内側の列は、得られたビットマップの bit1 です。
		@param[in] PosY 高さ（Y 値、0～フィールド高さ+1）
		@return 指定された段のビットマップ
	*/
	uint32_t	GetBitmap(int8_t PosY) const
	{
		return (PosY < 0) || (static_cast<uint8_t>(PosY) >= m_Field.size()) ? 0 : m_Field[PosY];

	} // end of GetBitmap

	/*!
		フィールドの１マス分のビットマップを取得します。
		フィールド最下端の１段（Y=0）は枠（地面）です。
		フィールド最上端の１段は枠（天井）です。
		フィールド下側の枠のすぐ内側の段は、Y=1 です。
		フィールド最右端の１列（X=0）は枠（右壁）です。
		フィールド最左端の１列は枠（左壁）です。
		フィールド右側の枠のすぐ内側の列は、X=1 です。
		@param[in] PosX 横位置（X 値、0～フィールド幅+1）
		@param[in] PosY 縦位置（Y 値、0～フィールド高さ+1）
		@retval 0 指定されたマスは空き
		@retval 1 指定されたマスは埋まり
	*/
	uint32_t	GetBitmap(int8_t PosX, int8_t PosY) const
	{
		return (PosY < 0) || (static_cast<uint8_t>(PosY) >= m_Field.size()) ? 0 : (m_Field[PosY] & TCBIT::Get(PosX) ? 1 : 0);

	} // end of GetBitmap

	/*!
		フィールドの１段が埋まった状態のビットマップを取得します。
		ラインが１段埋まった状態かどうかをチェックする際などに利用します。
		@return １段埋まった状態のビットマップ
	*/
	uint32_t	GetLineFilled() const
	{
		return m_LineFilled;

	} // end of GetLineFilled

	/*!
		フィールド上に指定されたピースを配置できるかどうかを返します。
		@param[in] Piece 配置するピース
		@retval true 配置できる
		@retval false 配置できない
	*/
	bool	IsSetPiece(const TCPIECE &Piece) const
	{
		return IsSetPiece(Piece, Piece.m_Pos);

	} // end of TCFIELD::IsSetPiece

	/*!
		フィールド上に指定されたピースを配置できるかどうかを返します。
		@param[in] Piece 配置するピース（ただし位置は参照されない。代わりに Pos が参照される）
		@param[in] Pos 配置する位置
		@retval true 配置できる
		@retval false 配置できない
	*/
	bool	IsSetPiece(const TCPIECE &Piece, const TCPOS &Pos) const
	{
		std::vector<uint32_t>	bitmap = TCPIECE::GetBitmap(Piece);
		for (int8_t yy = 0; yy < TCPIECE::PIECE_WIDTH; yy++) {
			if (
				(0 <= Pos.GetY() + yy) && (Pos.GetY() + yy < m_FieldSize.GetY()) &&
				(TCBIT::Shift(bitmap[yy], Pos.GetX()) & m_Field[Pos.GetY() + yy])
			) {
				return false;
			}
		}
		return true;

	} // end of TCFIELD::IsSetPiece

	/*!
		フィールド上に指定されたピースを配置します。
		@param[in] Piece 配置するピース
		@return フィールド
	*/
	TCFIELD	&SetPiece(const TCPIECE &Piece)
	{
		SetPiece(Piece, Piece.m_Pos);
		return *this;

	} // end of TCFIELD::SetPiece

	/*!
		フィールド上に指定されたピースを配置します。
		@param[in] Piece 配置するピース（ただし位置は参照されない）
		@param[in] Pos 配置する位置
		@return フィールド
	*/
	TCFIELD	&SetPiece(const TCPIECE &Piece, const TCPOS &Pos)
	{
		std::vector<uint32_t>	bitmap = TCPIECE::GetBitmap(Piece);
		for (int8_t yy = 0; yy < TCPIECE::PIECE_WIDTH; yy++) {
			if ((1 <= Pos.GetY() + yy) && (Pos.GetY() + yy < m_FieldSize.GetY() - 1)) {
				m_Field[Pos.GetY() + yy] |= TCBIT::Shift(bitmap[yy], Pos.GetX());
			}
		}
		return *this;

	} // end of TCFIELD::SetPiece

#if 0
	void	UnsetPiece(const TCPIECE &Piece)
	{
		UnsetPiece(Piece, Piece.m_Pos);

	} // end of TCFIELD::UnsetPiece

	void	UnsetPiece(const TCPIECE &Piece, const TCPOS &Pos)
	{
		std::vector<uint32_t>	bitmap = TCPIECE::GetBitmap(Piece);
		for (int8_t yy = 0; yy < TCPIECE::PIECE_WIDTH; yy++) {
			if ((1 <= Pos.GetY() + yy) && (Pos.GetY() + yy < m_FieldSize.GetY() - 1)) {
				m_Field[yy + Pos.GetY()] &= ~TCBIT::Shift(bitmap[yy], Pos.GetX());
			}
		}

	} // end of TCFIELD::UnsetPiece
#endif

	/*!
		フィールド上で揃っているラインの数を数えます。
		@return 揃っているライン数
	*/
	int8_t	CheckLines() const
	{
		int8_t	Lines = 0;
		for (int8_t yy = 1; yy < m_FieldSize.GetY() - 1; yy++) {
			if ((~m_Field[yy] & m_LineFilled) == 0) {
				Lines++;
			}
		}
		return Lines;

	} // end of TCFIELD::CheckLines

	/*!
		フィールド上で揃っているラインを取り除き、間を詰めます。
		@return フィールド
	*/
	TCFIELD	&RemoveLines()
	{
		int8_t	Filled = 0;
		for (int8_t yy = 1; yy < m_FieldSize.GetY() - 1; yy++) {
			if ((~m_Field[yy] & m_LineFilled) == 0) {
				Filled++;
				continue;
			}
			m_Field[yy - Filled] = m_Field[yy];
		}
		for (int8_t yy = 0; yy < Filled; yy++) {
			m_Field[m_FieldSize.GetY() - 2 - yy] &= ~m_LineFilled;
		}
		return *this;

	} // end of TCFIELD::RemoveLines

	/*!
		フィールドにあるスポットの数を数えます。
		スポットとは、埋まっていないマスのうち、その上側に１つ以上の埋まっているマスが存在するものを指します。
		@return 空いているスポット数
	*/
	size_t	CountSpots() const
	{
		size_t		Value = 0;
		uint32_t	mask = 0;
		for (int8_t yy = m_FieldSize.GetY() - 2; yy >= 1; yy--) {
			uint32_t	bitmap = m_Field[yy] & m_LineFilled;
			Value += TCBIT::Count(~bitmap & mask, m_FieldSize.GetX());
			mask |= bitmap;
		}
		return Value;

	} // end of TCFIELD::CountSpots
};



//----------------------------------------------------------------------------
#endif	// TCFIELD_H
