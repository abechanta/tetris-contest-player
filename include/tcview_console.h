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

#ifndef TCVIEW_CONSOLE_H
#define TCVIEW_CONSOLE_H
//! @file

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include "tctype.h"
#include "tcview.h"

//----------------------------------------------------------------------------
/*!
	コンソール画面出力です。
*/
class TCVIEW_CONSOLE : public TCVIEW
{
	//
	// assign
	//
protected:
	static const CELL	CELL_BLANK = ' ';

	//
	// variable
	//
protected:
	int8_t											m_Size[TC::XYZ];	//!< マップのサイズ
	std::vector<std::vector<std::vector<CELL> > >	m_Map;				//!< マップ (x,y,z) = m_Map[z][y][x]

	//
	// method
	//
public:
	TCVIEW_CONSOLE(int8_t Width, int8_t Height, int8_t Layer)
		: TCVIEW(Width, Height, Layer)
	{
		m_Size[TC::X] = Width;
		m_Size[TC::Y] = Height;
		m_Size[TC::Z] = Layer;

		// cleanup m_Map
		m_Map.resize(m_Size[TC::Z]);
		m_Map[0].resize(m_Size[TC::Y]);
		m_Map[0][0].resize(m_Size[TC::X]);
		m_Map[0][0][0] = CELL_BLANK;
		m_Map[0][0].assign(m_Size[TC::X], m_Map[0][0][0]);
		m_Map[0].assign(m_Size[TC::Y], m_Map[0][0]);
		m_Map.assign(m_Size[TC::Z], m_Map[0]);

	} // end of TCVIEW_CONSOLE::ctor

	TCSIZE	GetSize() const
	{
		return TCSIZE(m_Size[TC::X], m_Size[TC::Y]);

	} // end of TCVIEW_CONSOLE::GetSize

	void	Clear()
	{
		// cleanup m_Map
		m_Map.resize(m_Size[TC::Z]);
		m_Map[0].resize(m_Size[TC::Y]);
		m_Map[0][0].resize(m_Size[TC::X]);
		m_Map[0][0][0] = CELL_BLANK;
		m_Map[0][0].assign(m_Size[TC::X], m_Map[0][0][0]);
		m_Map[0].assign(m_Size[TC::Y], m_Map[0][0]);
		m_Map.assign(m_Size[TC::Z], m_Map[0]);

	} // end of TCVIEW_CONSOLE::Clear

	void	Paint()
	{
		HANDLE	handle = GetStdHandle(STD_OUTPUT_HANDLE);
		::COORD	pos =
		{
			0,	// X
			0,	// Y
		};
		SetConsoleCursorPosition(handle, pos);

		for (ssize_t yy = 0; yy < m_Size[TC::Y]; yy++) {
			for (ssize_t xx = 0; xx < m_Size[TC::X]; xx++) {
				uint8_t	Cid = m_Map[0][yy][xx];
				for (ssize_t zz = 1; zz < m_Size[TC::Z]; zz++) {
					if (m_Map[zz][yy][xx] != CELL_BLANK) {
						Cid = m_Map[zz][yy][xx];
					}
				}
				std::cout << static_cast<char>(Cid);
			}
			if (yy == m_Size[TC::Y] - 1) {
				break;	// skip the last endl output
			}
			std::cout << std::endl;
		}

	} // end of TCVIEW_CONSOLE::Paint

	void	Draw(const LOCATOR &Loc, CELL Cid)
	{
		if (
			((Loc.m_Pos[TC::X] < 0) || (m_Size[TC::X] <= Loc.m_Pos[TC::X])) ||
			((Loc.m_Pos[TC::Y] < 0) || (m_Size[TC::Y] <= Loc.m_Pos[TC::Y])) ||
			((Loc.m_Pos[TC::Z] < 0) || (m_Size[TC::Z] <= Loc.m_Pos[TC::Z]))
		) {
			return;
		}

		m_Map[Loc.m_Pos[TC::Z]][Loc.m_Pos[TC::Y]][Loc.m_Pos[TC::X]] = Cid;

	} // end of TCVIEW_CONSOLE::Draw

	void	DrawFont(const LOCATOR &Loc, char Chr, TRANSFUNC pfTrans = NULL)
	{
		if (
			((Loc.m_Pos[TC::X] < 0) || (m_Size[TC::X] <= Loc.m_Pos[TC::X])) ||
			((Loc.m_Pos[TC::Y] < 0) || (m_Size[TC::Y] <= Loc.m_Pos[TC::Y])) ||
			((Loc.m_Pos[TC::Z] < 0) || (m_Size[TC::Z] <= Loc.m_Pos[TC::Z]))
		) {
			return;
		}

		if (!pfTrans) {
			pfTrans = Trans_Default;
		}
		m_Map[Loc.m_Pos[TC::Z]][Loc.m_Pos[TC::Y]][Loc.m_Pos[TC::X]] = pfTrans(Chr);

	} // end of TCVIEW_CONSOLE::DrawFont

	void	Draw(const LOCATOR &Loc, const std::vector<CELL> &CidLine)
	{
		LOCATOR	TmpLoc = Loc;
		for (std::vector<CELL>::const_iterator src = CidLine.begin(); src != CidLine.end(); ++src) {
			Draw(TmpLoc, *src);
			TmpLoc += LOCATOR(1, 0, 0);
		}

	} // end of TCVIEW_CONSOLE::Draw

	void	DrawFont(const LOCATOR &Loc, const std::string &ChrLine, TRANSFUNC pfTrans = NULL)
	{
		LOCATOR	TmpLoc = Loc;
		for (std::string::const_iterator src = ChrLine.begin(); src != ChrLine.end(); ++src) {
			DrawFont(TmpLoc, *src, pfTrans);
			TmpLoc += LOCATOR(1, 0, 0);
		}

	} // end of TCVIEW_CONSOLE::DrawFont

	void	Draw(const LOCATOR &Loc, const std::vector<std::vector<CELL> > &CidRect)
	{
		LOCATOR	TmpLoc = Loc;
		for (std::vector<std::vector<CELL> >::const_iterator src = CidRect.begin(); src != CidRect.end(); ++src) {
			Draw(TmpLoc, *src);
			TmpLoc += LOCATOR(0, 1, 0);
		}

	} // end of TCVIEW_CONSOLE::Draw

	void	DrawFont(const LOCATOR &Loc, const std::vector<std::string> &ChrRect, TRANSFUNC pfTrans = NULL)
	{
		LOCATOR	TmpLoc = Loc;
		for (std::vector<std::string>::const_iterator src = ChrRect.begin(); src != ChrRect.end(); ++src) {
			DrawFont(TmpLoc, *src, pfTrans);
			TmpLoc += LOCATOR(0, 1, 0);
		}

	} // end of TCVIEW_CONSOLE::DrawFont
};



//----------------------------------------------------------------------------
#endif // TCVIEW_CONSOLE_H
