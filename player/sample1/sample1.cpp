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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "tctype.h"
#include "tcevent.h"
#include "tcfield.h"
#include "tcpiece.h"
#include "tcplayer.h"
#include "tcsize.h"
#include "tcstringlist.h"

//----------------------------------------------------------------------------
TCDECLARE_TCPLAYER()

class SAMPLE1 : public TCPLAYER
{
	TCDEFINE_TCPLAYER()

private:
	int8_t		m_SpdX;	//!< 右に進むか、左に進むか
	uint8_t		m_Rot;	//!< 左回転を何回させるか

public:
	static bool	IsAvailable(const char *strRule)
	{
		TCSTRINGLIST	RuleList(strRule);
		return RuleList.GetString("RULE") == "LONGRUN";

	} // end of SAMPLE1::IsAvailable

	SAMPLE1(const char *strPath, const char *strRule)
		: TCPLAYER(strPath, strRule),
		m_SpdX(-1),
		m_Rot(0)
	{
	}

	~SAMPLE1()
	{
	}

	void	StartGame(const TCFIELD &Field)
	{
		m_SpdX = -1;
		m_Rot = 0;
	}

	void	EndGame(const TCSCORE &Score)
	{
	}

	TCEVENT	Exec(TCEVENT Event, const TCFIELD &Field, const TCPIECE &CtrlPiece, const TCPIECE &NextPiece)
	{
		if (Event & TCEVENT_GAME_OVER) {	// ゲームオーバーになったら、、、
			// 何もしない
			return 0;
		}

		if (Event & TCEVENT_PREP) {	// ピースが落ちる瞬間は、、、
			// 左回転数を設定する
			m_Rot = static_cast<uint8_t>(m_InstanceId) % CtrlPiece.GetRotMax();
		}

		if (Event & TCEVENT_FALL) {	// ピース落下中に、、、
			if (Event & TCEVENT_nMOVE_R) {	// 右移動ができなかったら、、、
				// 左に移動するようになる
				m_SpdX = +1;
			}
			if (Event & TCEVENT_nMOVE_L) {	// 左移動ができなかったら、、、
				// 右に移動するようになる
				m_SpdX = -1;
			}
		}

		// 最後にキー入力を生成する
		return ((CtrlPiece.m_Rot != m_Rot) ? TCEVENT_ROT_L : 0) |
			((m_SpdX > 0) ? TCEVENT_MOVE_L : 0) |
			((m_SpdX < 0) ? TCEVENT_MOVE_R : 0);
	}
};

TCEXPORT_TCPLAYER(SAMPLE1, "SAMPLE1/0.00", "Abe Chanta")
