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

#ifndef TCPIECE_H
#define TCPIECE_H
//! @file

#include <vector>
#include "tctype.h"
#include "tcpos.h"
#include "tcrandomgenerator.h"
#include "tcsize.h"

//----------------------------------------------------------------------------
#define U32FROMBITARRAY_1(b3, b2, b1, b0)	\
	(\
		(((b3) ? 1 : 0) << 3) |\
		(((b2) ? 1 : 0) << 2) |\
		(((b1) ? 1 : 0) << 1) |\
		(((b0) ? 1 : 0) << 0)\
	)

#define U32FROMBITARRAY(b15, b14, b13, b12, b11, b10, b9, b8, b7, b6, b5, b4, b3, b2, b1, b0)	\
	U32FROMBITARRAY_1(b3, b2, b1, b0),\
	U32FROMBITARRAY_1(b7, b6, b5, b4),\
	U32FROMBITARRAY_1(b11, b10, b9, b8),\
	U32FROMBITARRAY_1(b15, b14, b13, b12)

/*!
	ピースの状態を保持するためのクラスです。
	コピー・代入に対応しています。
*/
class TCPIECE
{
	//
	// assign
	//
public:
	static const int8_t		PIECE_WIDTH	= 4;

	enum ROT
	{
		ROT_MAX			= 4,
	};
	enum FIG
	{
		FIG_I		= 0,		// figure I
		FIG_O		= 1,		// figure O
		FIG_S		= 2,		// figure S
		FIG_Z		= 3,		// figure Z
		FIG_J		= 4,		// figure J
		FIG_L		= 5,		// figure L
		FIG_T		= 6,		// figure T
		FIG_MAX		= 7,
		FIG_VOID	= 7,
	};

	//
	// variable
	//
private:
	static TCSIZE	m_FieldSize;	//!< フィールドサイズ
public:
	uint8_t			m_Fig;			//!< ピースの形状（TCPIECE::FIG）
	uint8_t			m_Rot;			//!< ピースの回転状態（0:回転なし、1:左１回、2:左２回、3:左３回）
	TCPOS			m_Pos;			//!< ピースの位置

	//
	// method
	//
public:
	/*!
		フィールドのサイズをクラスフィールドに設定します。
		ピースの出現位置を決定するために必要です。
		ゲームプレイを開始する前に設定しなければなりません。
		@param[in] FieldSize フィールドのサイズ
	*/
	static void	SetField(const TCSIZE &FieldSize)
	{
		m_FieldSize = FieldSize;

	} // end of TCPIECE::SetFieldSize

	/*!
		デフォルトのコンストラクタです。
		形状は未知、回転状態はゼロ、位置はゼロに初期化されます。
		主に未知のピースを生成するのに使用します。
	*/
	TCPIECE()
		: m_Fig(FIG_VOID),
		m_Rot(0),
		m_Pos(0, 0)
	{
	} // end of TCPIECE::ctor

	/*!
		標準のコンストラクタです。
		各メンバはそれぞれの引数の値に初期化されます。
		@param[in] Fig ピースの形状
		@param[in] Rot ピースの回転状態
		@param[in] Pos ピースの位置
	*/
	TCPIECE(uint8_t Fig, uint8_t Rot, const TCPOS &Pos)
		: m_Fig((Fig < FIG_MAX) ? Fig : 0),
		m_Rot((Rot < ROT_MAX) ? Rot : 0),
		m_Pos(Pos)
	{
	} // end of TCPIECE::ctor

	/*!
		コピーコンストラクタです。
		各メンバは指定されたインスタンスの各メンバに初期化されます。
		@param[in] Rhs コピー元のインスタンス
	*/
	TCPIECE(const TCPIECE &Rhs)
		: m_Fig(Rhs.m_Fig),
		m_Rot(Rhs.m_Rot),
		m_Pos(Rhs.m_Pos)
	{
	} // end of TCPIECE::ctor

	/*!
		代入演算子です。
		各メンバは指定されたインスタンスの各メンバに初期化されます。
		@param[in] Rhs コピー元のインスタンス
		@return このインスタンス
	*/
	TCPIECE &operator=(const TCPIECE &Rhs)
	{
		m_Fig = Rhs.m_Fig;
		m_Rot = Rhs.m_Rot;
		m_Pos = Rhs.m_Pos;
		return *this;

	} // end of TCPIECE::operator=

	/*!
		ピースの形状を乱数による変更し、
		ピースの回転状態をゼロに変更し、
		ピースの位置をフィールド所定の落下前位置に変更します。
		@param[in/out] Random 乱数インスタンス
	*/
	void	Randomize(TCRANDOMGENERATOR &Random)
	{
		m_Fig = static_cast<uint8_t>(Random.Get() % FIG_MAX);
		m_Rot = 0;
		m_Pos = TCPOS((m_FieldSize.GetX() - PIECE_WIDTH) / 2, m_FieldSize.GetY() - 1);
		if (m_Fig == FIG_I) {
			m_Pos += TCPOS(0, -1);
		}

	} // end of TCPIECE::Randomize

	/*!
		ピースを無効にします。
		落下中のピースがフィールドに着地したときに利用されます。
		ピースが無効かどうかは、m_Fig が FIG_VOID かどうかを確認することで判断できます。
	*/
	void	Invalidate()
	{
		m_Fig = FIG_VOID;
		m_Rot = 0;
		m_Pos = TCPOS((m_FieldSize.GetX() - PIECE_WIDTH) / 2, m_FieldSize.GetY() - 1);

	} // end of TCPIECE::Invalidate

	/*!
		ピースの形状による回転パターン数を返します。
		@retval 0 引数異常
		@retval 1～4 回転パターン数
	*/
	uint8_t	GetRotMax() const
	{
		return GetRotMax(m_Fig);

	} //end of TCPIECE::GetRotMax

	/*!
		ピースの形状による回転パターン数を返します。
		@param[in] Piece ピースのインスタンス
		@retval 0 引数異常
		@retval 1～4 回転パターン数
	*/
	static uint8_t	GetRotMax(const TCPIECE &Piece)
	{
		return GetRotMax(Piece.m_Fig);

	} //end of TCPIECE::GetRotMax

	/*!
		ピースの形状による回転パターン数を返します。
		@param[in] Fig ピースの形状
		@retval 0 引数異常
		@retval 1～4 回転パターン数
	*/
	static uint8_t	GetRotMax(uint8_t Fig)
	{
		static const uint8_t		RotMax[FIG_MAX] = {
			2,		// FIG_I
			1,		// FIG_O
			2,		// FIG_S
			2,		// FIG_Z
			4,		// FIG_J
			4,		// FIG_L
			4,		// FIG_T
		};

		if (
			(Fig >= FIG_MAX)
		) {
			return 0;
		}

		return RotMax[Fig];

	} //end of TCPIECE::GetRotMax

	/*!
		ピースの形状・回転状態による上端オフセット（落下ブロックの Y 位置に対する上端余白までの距離）を返します。
		上端オフセットは、ピースが落下前位置から落下開始位置へ移動するときに使用されます。
		@retval 0 引数異常
		@retval -4～4 上端オフセット
	*/
	int8_t	GetTopOfsY() const
	{
		return GetTopOfsY(m_Fig, m_Rot);

	} // end of TCPIECE::GetTopOfsY

	/*!
		ピースの形状・回転状態による上端オフセット（落下ブロックの Y 位置に対する上端余白までの距離）を返します。
		上端オフセットは、ピースが落下前位置から落下開始位置へ移動するときに使用されます。
		@param[in] Piece ピースのインスタンス
		@retval 0 引数異常
		@retval -4～4 上端オフセット
	*/
	static int8_t	GetTopOfsY(const TCPIECE &Piece)
	{
		return GetTopOfsY(Piece.m_Fig, Piece.m_Rot);

	} // end of TCPIECE::GetTopOfsY

	/*!
		ピースの形状・回転状態による上端オフセット（落下ブロックの Y 位置に対する上端余白までの距離）を返します。
		上端オフセットは、ピースが落下前位置から落下開始位置へ移動するときに使用されます。
		@param[in] Fig ピースの形状
		@param[in] Rot ピースの回転状態
		@retval 0 引数異常
		@retval -4～4 上端オフセット
	*/
	static int8_t	GetTopOfsY(uint8_t Fig, uint8_t Rot)
	{
		static const int8_t	TopOfsY[FIG_MAX][ROT_MAX] = {
			{	// FIG_I
				-3+1,	-4+1,	-3+1,	-4+1,
			},
			{	// FIG_O
				-2,		-2,		-2,		-2,
			},
			{	// FIG_S
				-2,		-3,		-2,		-3,
			},
			{	// FIG_Z
				-2,		-3,		-2,		-3,
			},
			{	// FIG_J
				-2,		-3,		-2,		-3,
			},
			{	// FIG_L
				-2,		-3,		-2,		-3,
			},
			{	// FIG_T
				-2,		-3,		-2,		-3,
			},
		};

		if (
			(Fig >= FIG_MAX) ||
			(Rot >= ROT_MAX)
		) {
			return 0;
		}

		return TopOfsY[Fig][Rot];

	} // end of TCPIECE::GetTopOfsY

	/*!
		ピースの形状・回転状態による右端オフセット（落下ブロックの X 位置に対する右端余白までの距離）を返します。
		右端オフセットは、ピースの落下中に右移動の限界位置を取得するときに使用されます。
		@retval 0 引数異常
		@retval -4～4 右端オフセット
	*/
	int8_t	GetRightOfsX() const
	{
		return GetRightOfsX(m_Fig, m_Rot);

	} // end of TCPIECE::GetRightOfsX

	/*!
		ピースの形状・回転状態による右端オフセット（落下ブロックの X 位置に対する右端余白までの距離）を返します。
		右端オフセットは、ピースの落下中に右移動の限界位置を取得するときに使用されます。
		@param[in] Piece ピースのインスタンス
		@retval 0 引数異常
		@retval -4～4 右端オフセット
	*/
	static int8_t	GetRightOfsX(const TCPIECE &Piece)
	{
		return GetRightOfsX(Piece.m_Fig, Piece.m_Rot);

	} // end of TCPIECE::GetRightOfsX

	/*!
		ピースの形状・回転状態による右端オフセット（落下ブロックの X 位置に対する右端余白までの距離）を返します。
		右端オフセットは、ピースの落下中に右移動の限界位置を取得するときに使用されます。
		@param[in] Fig ピースの形状
		@param[in] Rot ピースの回転状態
		@retval 0 引数異常
		@retval -4～4 右端オフセット
	*/
	static int8_t	GetRightOfsX(uint8_t Fig, uint8_t Rot)
	{
		static const int8_t	RightOfsX[FIG_MAX][ROT_MAX] = {
			{	// FIG_I
				0,		-1,		0,		-1,
			},
			{	// FIG_O
				-1,		-1,		-1,		-1,
			},
			{	// FIG_S
				-1,		-2,		-1,		-2,
			},
			{	// FIG_Z
				-1,		-1,		-1,		-1,
			},
			{	// FIG_J
				-1,		-1,		-1,		-2,
			},
			{	// FIG_L
				-1,		-1,		-1,		-2,
			},
			{	// FIG_T
				-1,		-1,		-1,		-2,
			},
		};

		if (
			(Fig >= FIG_MAX) ||
			(Rot >= ROT_MAX)
		) {
			return 0;
		}

		return RightOfsX[Fig][Rot];

	} // end of TCPIECE::GetRightOfsX

	/*!
		ピースの形状・回転状態による左端オフセット（落下ブロックの X 位置に対する左端余白までの距離）を返します。
		左端オフセットは、ピースの落下中に左移動の限界位置を取得するときに使用されます。
		@retval 0 引数異常
		@retval -4～4 左端オフセット
	*/
	int8_t	GetLeftOfsX() const
	{
		return GetLeftOfsX(m_Fig, m_Rot);

	} // end of GetLeftOfsX

	/*!
		ピースの形状・回転状態による左端オフセット（落下ブロックの X 位置に対する左端余白までの距離）を返します。
		左端オフセットは、ピースの落下中に左移動の限界位置を取得するときに使用されます。
		@param[in] Piece ピースのインスタンス
		@retval 0 引数異常
		@retval -4～4 左端オフセット
	*/
	static int8_t	GetLeftOfsX(const TCPIECE &Piece)
	{
		return GetLeftOfsX(Piece.m_Fig, Piece.m_Rot);

	} // end of TCPIECE::GetLeftOfsX

	/*!
		ピースの形状・回転状態による左端オフセット（落下ブロックの X 位置に対する左端余白までの距離）を返します。
		左端オフセットは、ピースの落下中に左移動の限界位置を取得するときに使用されます。
		@param[in] Fig ピースの形状
		@param[in] Rot ピースの回転状態
		@retval 0 引数異常
		@retval -4～4 左端オフセット
	*/
	static int8_t	GetLeftOfsX(uint8_t Fig, uint8_t Rot)
	{
		static const int8_t	LeftOfsX[FIG_MAX][ROT_MAX] = {
			{	// FIG_I
				-4,		-2,		-4,		-2,
			},
			{	// FIG_O
				-3,		-3,		-3,		-3,
			},
			{	// FIG_S
				-4,		-4,		-4,		-4,
			},
			{	// FIG_Z
				-4,		-3,		-4,		-3,
			},
			{	// FIG_J
				-4,		-3,		-4,		-4,
			},
			{	// FIG_L
				-4,		-3,		-4,		-4,
			},
			{	// FIG_T
				-4,		-3,		-4,		-4,
			},
		};

		if (
			(Fig >= FIG_MAX) ||
			(Rot >= ROT_MAX)
		) {
			return 0;
		}

		return LeftOfsX[Fig][Rot];

	} // end of TCPIECE::GetLeftOfsX

	/*!
		ピースのビットマップ形状を vector<uint32_t> で返します。
		vector には uint32_t が 4 つ（＝PIECE_WIDTH）含まれています。
		各要素の添え字は Y 値に、各要素内のビット位置は X 値に対応しており、ビットが On の場所にブロックが存在します。
		例えば 1 つ目の要素は Y=0（最下段）を、4 つ目の要素は Y=3（最上段）を示しています。
		各要素の bit0 は X=0（右端）を、bit3 は X=3（左端）を示しています。
		@retval 空の vector 引数異常
		@retval ビットマップ形状
	*/
	std::vector<uint32_t>	GetBitmap() const
	{
		return GetBitmap(m_Fig, m_Rot);

	} // end of GetBitmap

	/*!
		ピースのビットマップ形状を vector<uint32_t> で返します。
		vector には uint32_t が 4 つ（＝PIECE_WIDTH）含まれています。
		各要素の添え字は Y 値に、各要素内のビット位置は X 値に対応しており、ビットが On の場所にブロックが存在します。
		例えば 1 つ目の要素は Y=0（最下段）を、4 つ目の要素は Y=3（最上段）を示しています。
		各要素の bit0 は X=0（右端）を、bit3 は X=3（左端）を示しています。
		@param[in] Piece ピースのインスタンス
		@retval 空の vector 引数異常
		@retval ビットマップ形状
	*/
	static std::vector<uint32_t>	GetBitmap(const TCPIECE &Piece)
	{
		return GetBitmap(Piece.m_Fig, Piece.m_Rot);

	} // end of TCPIECE::GetBitmap

	/*!
		ピースのビットマップ形状を vector<uint32_t> で返します。
		vector には uint32_t が 4 つ（＝PIECE_WIDTH）含まれています。
		各要素の添え字は Y 値に、各要素内のビット位置は X 値に対応しており、ビットが On の場所にブロックが存在します。
		例えば 1 つ目の要素は Y=0（最下段）を、4 つ目の要素は Y=3（最上段）を示しています。
		各要素の bit0 は X=0（右端）を、bit3 は X=3（左端）を示しています。
		@param[in] Fig ピースの形状
		@param[in] Rot ピースの回転状態
		@retval 空の vector 引数異常
		@retval ビットマップ形状
	*/
	static std::vector<uint32_t>	GetBitmap(uint8_t Fig, uint8_t Rot)
	{
		static const uint32_t	PieceBitmap[FIG_MAX][ROT_MAX][PIECE_WIDTH] = {
			{	// FIG_O
				{	// ROT=0
					U32FROMBITARRAY(
						0, 0, 0, 0,
						1, 1, 1, 1,
						0, 0, 0, 0,
						0, 0, 0, 0
					),
				},
				{	// ROT=1
					U32FROMBITARRAY(
						0, 0, 1, 0,
						0, 0, 1, 0,
						0, 0, 1, 0,
						0, 0, 1, 0
					),
				},
				{	// ROT=2
					U32FROMBITARRAY(
						0, 0, 0, 0,
						1, 1, 1, 1,
						0, 0, 0, 0,
						0, 0, 0, 0
					),
				},
				{	// ROT=3
					U32FROMBITARRAY(
						0, 0, 1, 0,
						0, 0, 1, 0,
						0, 0, 1, 0,
						0, 0, 1, 0
					),
				},
			},
			{	// FIG_O
				{	// ROT=0
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 1, 1, 0,
						0, 1, 1, 0
					),
				},
				{	// ROT=1
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 1, 1, 0,
						0, 1, 1, 0
					),
				},
				{	// ROT=2
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 1, 1, 0,
						0, 1, 1, 0
					),
				},
				{	// ROT=3
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 1, 1, 0,
						0, 1, 1, 0
					),
				},
			},
			{	// FIG_S
				{	// ROT=0
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 1, 1, 0,
						1, 1, 0, 0
					),
				},
				{	// ROT=1
					U32FROMBITARRAY(
						0, 0, 0, 0,
						1, 0, 0, 0,
						1, 1, 0, 0,
						0, 1, 0, 0
					),
				},
				{	// ROT=2
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 1, 1, 0,
						1, 1, 0, 0
					),
				},
				{	// ROT=3
					U32FROMBITARRAY(
						0, 0, 0, 0,
						1, 0, 0, 0,
						1, 1, 0, 0,
						0, 1, 0, 0
					),
				},
			},
			{	// FIG_Z
				{	// ROT=0
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						1, 1, 0, 0,
						0, 1, 1, 0
					),
				},
				{	// ROT=1
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 1, 0,
						0, 1, 1, 0,
						0, 1, 0, 0
					),
				},
				{	// ROT=2
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						1, 1, 0, 0,
						0, 1, 1, 0
					),
				},
				{	// ROT=3
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 1, 0,
						0, 1, 1, 0,
						0, 1, 0, 0
					),
				},
			},
			{	// FIG_J
				{	// ROT=0
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						1, 1, 1, 0,
						0, 0, 1, 0
					),
				},
				{	// ROT=1
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 1, 1, 0,
						0, 1, 0, 0,
						0, 1, 0, 0
					),
				},
				{	// ROT=2
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						1, 0, 0, 0,
						1, 1, 1, 0
					),
				},
				{	// ROT=3
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 1, 0, 0,
						0, 1, 0, 0,
						1, 1, 0, 0
					),
				},
			},
			{	// FIG_L
				{	// ROT=0
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						1, 1, 1, 0,
						1, 0, 0, 0
					),
				},
				{	// ROT=1
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 1, 0, 0,
						0, 1, 0, 0,
						0, 1, 1, 0
					),
				},
				{	// ROT=2
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 1, 0,
						1, 1, 1, 0
					),
				},
				{	// ROT=3
					U32FROMBITARRAY(
						0, 0, 0, 0,
						1, 1, 0, 0,
						0, 1, 0, 0,
						0, 1, 0, 0
					),
				},
			},
			{	// FIG_T
				{	// ROT=0
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						1, 1, 1, 0,
						0, 1, 0, 0
					),
				},
				{	// ROT=1
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 1, 0, 0,
						0, 1, 1, 0,
						0, 1, 0, 0
					),
				},
				{	// ROT=2
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 1, 0, 0,
						1, 1, 1, 0
					),
				},
				{	// ROT=3
					U32FROMBITARRAY(
						0, 0, 0, 0,
						0, 1, 0, 0,
						1, 1, 0, 0,
						0, 1, 0, 0
					),
				},
			},
		};

		if (
			(Fig >= FIG_MAX) ||
			(Rot >= ROT_MAX)
		) {
			return std::vector<uint32_t>();
		}

		std::vector<uint32_t>	Value;
		for (int8_t ii = 0; ii < PIECE_WIDTH; ii++) {
			Value.push_back(PieceBitmap[Fig][Rot][ii]);
		}
		return Value;

	} // end of TCPIECE::GetBitmap

	/*!
		ピースを指定位置に移動させたときのインスタンスを返します。
		@param[in] PosX 横位置（X 値）
		@param[in] PosY 縦位置（Y 値）
		@return 移動後のピースのインスタンス
	*/
	TCPIECE Place(int8_t PosX, int8_t PosY) const
	{
		return Place(TCPOS(PosX, PosY));

	} // end of TCPIECE::Place

	/*!
		ピースを指定位置に移動させたときのインスタンスを返します。
		@param[in] Pos 移動後の位置
		@return 移動後のピースのインスタンス
	*/
	TCPIECE Place(const TCPOS &Pos) const
	{
		return TCPIECE(m_Fig, m_Rot, Pos);

	} // end of TCPIECE::Place

	/*!
		ピースを指定数だけ横に移動させたときのインスタンスを返します。
		@param[in] OfsX 横の移動量
		@return 移動後のピースのインスタンス
	*/
	TCPIECE MoveX(int8_t OfsX) const
	{
		TCPIECE	Value(*this);
		Value.m_Pos += TCPOS(OfsX, 0);
		return Value;

	} // end of TCPIECE::MoveX

	/*!
		ピースを指定数だけ縦に移動させたときのインスタンスを返します。
		@param[in] OfsY 縦の移動量
		@return 移動後のピースのインスタンス
	*/
	TCPIECE MoveY(int8_t OfsY) const
	{
		TCPIECE	Value(*this);
		Value.m_Pos += TCPOS(0, OfsY);
		return Value;

	} // end of TCPIECE::MoveY

	/*!
		ピースを指定数だけ回転させたときのインスタンスを返します。
		@param[in] Rot 回転量（正数なら左回転、負数なら右回転）
		@return 回転後のピースのインスタンス
	*/
	TCPIECE	Rotate(int8_t Rot) const
	{
		TCPIECE	Value(*this);
		while (Rot < 0) {
			Rot += ROT_MAX;
		}
		Value.m_Rot = static_cast<uint8_t>(Value.m_Rot + Rot);
		while (Value.m_Rot >= ROT_MAX) {
			Value.m_Rot -= ROT_MAX;
		}
		return Value;

	} // end of TCPIECE::Rotate
};

#ifdef TCDECLARE_OBJECT
TCSIZE	TCPIECE::m_FieldSize = TCSIZE(10, 20);
#endif	// TCDECLARE_OBJECT



//----------------------------------------------------------------------------
#endif	// TCPIECE_H
