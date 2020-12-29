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

#ifndef ATU_H
#define ATU_H

#include <strstream>
#include "tctype.h"
#include "tcbit.h"

//----------------------------------------------------------------------------
namespace ATU
{
	//
	// variable
	//

	//
	// method
	//
	size_t	CountBit(uint32_t bitmap, size_t width = 32)
	{
#if 0
		size_t	Value = bitmap;
		Value = ((Value >> 0) & 0x55555555U) + ((Value >> 1) & 0x55555555U);
		Value = ((Value >> 0) & 0x33333333U) + ((Value >> 2) & 0x33333333U);
		Value = ((Value >> 0) & 0x0F0F0F0FU) + ((Value >> 4) & 0x0F0F0F0FU);
		Value = ((Value >> 0) & 0x00FF00FFU) + ((Value >> 8) & 0x00FF00FFU);
		Value = ((Value >> 0) & 0x0000FFFFU) + ((Value >> 16) & 0x0000FFFFU);
		return Value;
#else
		static const size_t	BitCount[32] = {
			0+0,	0+1,	0+1,	0+2,	0+1,	0+2,	0+2,	0+3,
			1+0,	1+1,	1+1,	1+2,	1+1,	1+2,	1+2,	1+3,
			1+0,	1+1,	1+1,	1+2,	1+1,	1+2,	1+2,	1+3,
			2+0,	2+1,	2+1,	2+2,	2+1,	2+2,	2+2,	2+3,
		};

		size_t	Value = 0;
		for (size_t ii = 0; ii < width; ii += 5) {
			Value += BitCount[bitmap & 0x0000001FU];
			bitmap >>= 5;
		}
		return Value;
#endif

	} // end of ATU::CountBit

	size_t	CountSpots(const TCFIELD &Field)
	{
		size_t		Value = 0;
		uint32_t	BlockFoundMask = 0;	// 0 means vacant space above.
		for (int8_t yy = Field.GetSize().GetY() - 2; yy >= 1; yy--) {	// from TOP to BOTTOM
			uint32_t	Bitmap = Field.GetBitmap(yy) & Field.GetLineFilled();
			Value += ATU::CountBit(~Bitmap & BlockFoundMask, Field.GetSize().GetX());
			BlockFoundMask |= Bitmap;
		}
		return Value;

	} // end of ATU::CountSpots

	std::vector<int8_t>	GetSpots(const TCFIELD &Field)
	{
		std::vector<int8_t>	Value;
		Value.resize(Field.GetSize().GetY());
		uint32_t	BlockFoundMask = 0;	// 0 means vacant space above.
		for (int8_t yy = Field.GetSize().GetY() - 2; yy >= 1; yy--) {	// from TOP to BOTTOM
			uint32_t	Bitmap = Field.GetBitmap(yy) & Field.GetLineFilled();
			Value[yy] = static_cast<int8_t>(ATU::CountBit(~Bitmap & BlockFoundMask, Field.GetSize().GetX()));
			BlockFoundMask |= Bitmap;
		}
		return Value;

	} // end of ATU::GetSpots

	std::vector<int8_t>	GetHeight(const TCFIELD &Field)
	{
		std::vector<int8_t>	Value;
		Value.resize(Field.GetSize().GetX());
		uint32_t	BlockFoundMask = 0;	// 0 means vacant space above.
		for (int8_t yy = Field.GetSize().GetY() - 2; yy >= 1; yy--) {	// from TOP to BOTTOM
			uint32_t	Bitmap = Field.GetBitmap(yy) & Field.GetLineFilled() & ~BlockFoundMask;
			if (Bitmap) {
				for (int8_t xx = 1; xx < Field.GetSize().GetX() - 1; xx++) {
					if (Bitmap & TCBIT::Get(xx)) {
						Value[xx] = yy;
					}
				}
			}
			BlockFoundMask |= Bitmap;
		}
		return Value;

	} // end of ATU::GetHeight

	std::vector<int8_t>	GetSpotHeight(const TCFIELD &Field)
	{
		std::vector<int8_t>	Value;
		Value.resize(Field.GetSize().GetX());
		uint32_t	BlockFoundMask = 0;	// 0 means vacant space above.
		uint32_t	SpotFoundMask = 0;	// 1 means spot found above.
		uint32_t	Bitmap;
		for (int8_t yy = Field.GetSize().GetY() - 2; yy >= 1; yy--) {	// from TOP to BOTTOM
			Bitmap = Field.GetBitmap(yy) & Field.GetLineFilled() & ~SpotFoundMask;
			if (Bitmap) {
				for (int8_t xx = 1; xx < Field.GetSize().GetX() - 1; xx++) {
					if (Bitmap & TCBIT::Get(xx)) {
						Value[xx]++;
					}
				}
			}
			BlockFoundMask |= Field.GetBitmap(yy) & Field.GetLineFilled();
			SpotFoundMask |= ~Field.GetBitmap(yy) & Field.GetLineFilled() & BlockFoundMask;
		}

		Bitmap = ~SpotFoundMask & Field.GetLineFilled();
		if (Bitmap) {
			for (int8_t xx = 1; xx < Field.GetSize().GetX() - 1; xx++) {
				if (Bitmap & TCBIT::Get(xx)) {
					Value[xx] = 0;
				}
			}
		}
		return Value;

	} // end of ATU::GetSpotHeight

	TCPIECE	LimitD(const TCFIELD &Field, const TCPIECE &Piece)
	{
		TCPIECE	Value = Piece;
		while (Field.IsSetPiece(Value.MoveY(-1))) {
			Value = Value.MoveY(-1);
		}
		return Value;

	} // end of ATU::LimitD

	TCPIECE	LimitR(const TCFIELD &Field, const TCPIECE &Piece)
	{
		TCPIECE	Value = Piece;
		while (Field.IsSetPiece(Value.MoveX(-1))) {
			Value = Value.MoveX(-1);
		}
		return Value;

	} // end of ATU::LimitR

	TCPIECE	LimitL(const TCFIELD &Field, const TCPIECE &Piece)
	{
		TCPIECE	Value = Piece;
		while (Field.IsSetPiece(Value.MoveX(+1))) {
			Value = Value.MoveX(+1);
		}
		return Value;

	} // end of ATU::LimitL

	TCFIELD	operator +(const TCFIELD &Field, const TCPIECE &Piece)
	{
		TCFIELD	Value = Field;
		Value.SetPiece(Piece);
		return Value;

	}  // end of ATU::operator +(TCFIELD, TCPIECE)

	bool	operator ==(const TCPIECE &Lhs, const TCPIECE &Rhs)
	{
		return Lhs.m_Pos.GetX() == Rhs.m_Pos.GetX();

	} // end of ATU::operator ==

	bool	operator !=(const TCPIECE &Lhs, const TCPIECE &Rhs)
	{
		return Lhs.m_Pos.GetX() != Rhs.m_Pos.GetX();

	} // end of ATU::operator !=

	TCEVENT	GetMoveEvent(const TCPIECE &NowPiece, const TCPIECE &TargetPiece)
	{
		return (
			((NowPiece.m_Pos.GetX() > TargetPiece.m_Pos.GetX()) ? TCEVENT_MOVE_R : 0) |
			((NowPiece.m_Pos.GetX() < TargetPiece.m_Pos.GetX()) ? TCEVENT_MOVE_L : 0)
		);

	} // end of ATU::GetMoveEvent

	TCEVENT	GetRotEvent(const TCPIECE &NowPiece, const TCPIECE &TargetPiece, ssize_t RotSign = 1)
	{
		TCEVENT	Value = 0;
		uint8_t	RotMax = NowPiece.GetRotMax();

		switch (RotMax) {
		case 1:
		default:
			Value = 0;
			break;

		case 2:
			Value = ((NowPiece.m_Rot % RotMax) != (TargetPiece.m_Rot % RotMax)) ? TCEVENT_ROT_L : 0;
			break;

		case 4:
			switch ((TargetPiece.m_Rot + 4 - NowPiece.m_Rot) % RotMax) {
			case 0:
			default:
				Value = 0;
				break;

			case 1:
				Value = TCEVENT_ROT_L;
				break;

			case 2:
				Value = (RotSign >= 0) ? TCEVENT_ROT_L : TCEVENT_ROT_R;
				break;

			case 3:
				Value = TCEVENT_ROT_R;
				break;

			} // endswitch ((NowPiece.m_Rot + 4 - TargetPiece.m_Rot) % RotMax)
			break;

		} // endswitch RotMax

		return Value;

	} // end of ATU::GetRotEvent

	std::string	GetParamString(const std::vector<int8_t> &Param)
	{
		std::strstream	Value;
		for (std::vector<int8_t>::const_iterator it = Param.begin(); it != Param.end(); ++it) {
			Value
				<< (it == Param.begin() ? '(' : ',')
				<< static_cast<ssize_t>(*it);
		}
		Value
			<< ')'
			<< std::ends;
		return Value.str();

	} // end of ATU::GetParamString
};



//----------------------------------------------------------------------------
#endif	// ATU_H
