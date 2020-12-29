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

#ifndef TCPLAYFIELD_H
#define TCPLAYFIELD_H
//! @file

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#include <string>
#include <strstream>
#include "tctype.h"
#include "tcevent.h"
#include "tcfield.h"
#include "tcpiece.h"
#include "tcplayer.h"
#include "tcrandomgenerator.h"
#include "tcscore.h"
#include "tcsize.h"
#include "tcview.h"

//----------------------------------------------------------------------------
/*!
	プレイフィールド全体を保持するクラスです。
*/
class TCPLAYFIELD
{
	//
	// assign
	//
private:
	static const size_t		LAND_BONUS_FRAME = 10;		//!< ピースが接地した瞬間に取得できるボーナスフレーム数
	static const ssize_t	PENALTY_ON_EXCEPTION = 100;	//!< 例外排出時のペナルティ

	enum TCSEQ
	{
		SEQ_NOT_PLAYING		= 0,	//!< プレイ開始前の状態
		SEQ_PREP			= 1,	//!< ピースが落下する直前のフレーム
		SEQ_FALL			= 2,	//!< ピース落下中の状態
		SEQ_CHECK			= 3,	//!< ピースが着地して固まった状態
		SEQ_LINE_OUT		= 4,	//!< ラインが揃った状態
		SEQ_GAME_OVER		= 5,	//!< プレイ終了の状態
		SEQ_MAX
	};

	//
	// variable
	//
private:
	TCRANDOMGENERATOR		m_Random;		//!< 乱数ジェネレータ
	TCSEQ					m_Seq;			//!< ゲームシーケンス
	ssize_t					m_SeqFrame;		//!< 同じゲームシーケンスを何フレーム継続しているか
	size_t					m_HoverFrame;	//!< 次のピース落下（１段）までの係数（移動直後は 0、同じ高さにとどまると重力係数に応じて増加し、10 を超えると１段落下、20 を超えると２段落下する）
	size_t					m_LandFrame;	//!< ピースが接地してから固まるまでの係数（接地していなかったら 0、接地したら 1 から増加し、ボーナスフレーム数を超えると固まる）
	size_t					m_SpotCount;	//!< フィールドに存在するスポット面積
	TCEVENT					m_NextEvent;	//!< 次のフレームでプレイヤに渡すべきイベント
	TCFIELD					m_Field;		//!< フィールド
	TCPIECE					m_CtrlPiece;	//!< 操作中の落下ピース（SEQ_CHECK 時は参照できない）
	TCPIECE					m_NextPiece;	//!< 次の落下ピース（SEQ_PERP 時は参照できない）
	TCSCORE					m_Score;		//!< ラウンドのスコア

	//
	// method
	//
public:
	/*!
		標準のコンストラクタです。
		@param[in] Width フィールドの幅
		@param[in] Height フィールドの高さ
		@param[in] Gravity 重力係数
	*/
	TCPLAYFIELD(int8_t Width, int8_t Height, size_t Gravity)
		: m_Random(0),
		m_Seq(SEQ_NOT_PLAYING),
		m_SeqFrame(0),
		m_HoverFrame(0),
		m_LandFrame(0),
		m_SpotCount(0),
		m_NextEvent(0),
		m_Field(Width, Height, Gravity),
		m_CtrlPiece(),
		m_NextPiece(),
		m_Score()
	{
		TCPIECE::SetField(m_Field.GetSize());

	} // end of TCPLAYFIELD::ctor

	/*!
		コピーコンストラクタです。
		各メンバはコピー元のメンバに初期化されます。
		@param[in] Rhs コピー元のインスタンス
	*/
	TCPLAYFIELD(const TCPLAYFIELD &Rhs)
		: m_Random(Rhs.m_Random),
		m_Seq(Rhs.m_Seq),
		m_SeqFrame(Rhs.m_SeqFrame),
		m_HoverFrame(Rhs.m_HoverFrame),
		m_LandFrame(Rhs.m_LandFrame),
		m_SpotCount(Rhs.m_SpotCount),
		m_NextEvent(Rhs.m_NextEvent),
		m_Field(Rhs.m_Field),
		m_CtrlPiece(Rhs.m_CtrlPiece),
		m_NextPiece(Rhs.m_NextPiece),
		m_Score(Rhs.m_Score)
	{
	} // end of TCPLAYFIELD::ctor

	/*!
		代入演算子です。
		各メンバはコピー元のメンバに初期化されます。
		@param[in] Rhs 代入元のインスタンス
		@return このインスタンス
	*/
	TCPLAYFIELD	&operator=(const TCPLAYFIELD &Rhs)
	{
		m_Random = Rhs.m_Random;
		m_Seq = Rhs.m_Seq;
		m_SeqFrame = Rhs.m_SeqFrame;
		m_HoverFrame = Rhs.m_HoverFrame;
		m_LandFrame = Rhs.m_LandFrame;
		m_SpotCount = Rhs.m_SpotCount;
		m_NextEvent = Rhs.m_NextEvent;
		m_Field = Rhs.m_Field;
		m_CtrlPiece = Rhs.m_CtrlPiece;
		m_NextPiece = Rhs.m_NextPiece;
		m_Score = Rhs.m_Score;
		return *this;

	} // end of TCPLAYFIELD::operator=

	/*!
		プレイ中のプレイヤのラウンド成績を取得します。
		@return プレイヤのラウンド成績
	*/
	TCSCORE	GetScore() const
	{
		return m_Score;

	} // end of TCPLAYFIELD::GetScore

	/*!
		フィールドの状態を取得します。
		@return フィールド
	*/
	TCFIELD	GetField() const
	{
		return m_Field;

	} // end of TCPLAYFIELD::GetField

	/*!
		フィールドのサイズを取得します。
		@return フィールドのサイズ
	*/
	TCSIZE	GetFieldSize() const
	{
		return m_Field.GetSize();

	} // end of TCPLAYFIELD::GetFieldSize

	/*!
		プレイヤが操作中のコントロールピースを取得します。
		操作不可能な期間中（コントロールピースが接地して固まったとき）にこの関数を呼ぶと、
		無効なピース（FIG_VOID）が返されます。
		@return プレイヤのラウンド成績
	*/
	TCPIECE	GetCtrlPiece() const
	{
		return m_CtrlPiece;

	} // end of TCPLAYFIELD::GetCtrlPiece

	/*!
		次に落下するネクストピースを取得します。
		コントロールピースが接地して次のコントロールピースが出現する直前にこの関数を呼ぶと、
		無効なピース（FIG_VOID）が返されます。
		@return プレイヤのラウンド成績
	*/
	TCPIECE	GetNextPiece() const
	{
		return m_NextPiece;

	} // end of TCPLAYFIELD::GetNextPiece

	/*!
		プレイ中かどうかを返します。
		@retval true プレイ中である
		@retval false プレイ中でない
	*/
	bool	IsPlaying() const
	{
		return m_Seq != SEQ_NOT_PLAYING;

	} // end of TCPLAYFIELD::IsPlaying

	/*!
		ペナルティを与えます。
		@param[in] Penalty ペナルティの大きさ
		@param[in] bDisqualified 失格させるかどうか
	*/
	void	GivePenalty(ssize_t Penalty, bool bDisqualified)
	{
		m_Score.AddPenalties(Penalty);
		if (bDisqualified) {
			m_Seq = SEQ_NOT_PLAYING;
		}

	} // end of TCPLAYFIELD::GivePenalty

	/*!
		プレイを開始します。
		すでにプレイ中のときは何もしません。
		@param[in] Random 乱数ジェネレータ
		@param[in/out] pPlayer プレイヤ入力のインスタンス
	*/
	TCEVENT	StartGame(const TCRANDOMGENERATOR &Random, TCPLAYER *pPlayer)
	{
		TCASSERT(pPlayer, "fatal.");

		if (m_Seq != SEQ_NOT_PLAYING) {
			return 0;
		}

		m_Random = Random;
		m_Seq = SEQ_PREP;
		m_SeqFrame = 0;
		m_HoverFrame = 0;
		m_LandFrame = 0;
		m_NextEvent = 0;
		m_SpotCount = 0;

		m_Field.StartGame();
		m_CtrlPiece.Randomize(m_Random);
		m_NextPiece.Randomize(m_Random);
		m_Score.Clear();

		try {
			pPlayer->StartGame(m_Field);
		} catch (...) {
			GivePenalty(PENALTY_ON_EXCEPTION, true);
			return static_cast<TCEVENT>(TCEVENT_GAME_OVER);
		}

		return 0;

	} // end of TCPLAYFIELD::StartGame

	/*!
		ゲームを１フレームだけ進行させます。
		@param[in/out] pPlayer プレイヤ入力のインスタンス
		@return 発生した主なイベント
	*/
	TCEVENT	Tick(TCPLAYER *pPlayer)
	{
		TCASSERT(pPlayer, "fatal.");

		TCEVENT	evt = 0;
		TCSEQ	SeqOld = m_Seq;

		try {
			switch (m_Seq) {
			case SEQ_NOT_PLAYING:
				m_Seq = Tick_NotPlaying(pPlayer, evt);
				break;

			case SEQ_PREP:
				m_Seq = Tick_Prep(pPlayer, evt);
				break;

			case SEQ_FALL:
				m_Seq = Tick_Fall(pPlayer, evt);
				break;

			case SEQ_CHECK:
				m_Seq = Tick_Check(pPlayer, evt);
				break;

			case SEQ_LINE_OUT:
				m_Seq = Tick_LineOut(pPlayer, evt);
				break;

			case SEQ_GAME_OVER:
				m_Seq = Tick_GameOver(pPlayer, evt);
				break;

			default:
				TCASSERT(false, "unexpected.");
				break;

			} // endswitch m_Seq

		} catch (...) {
			GivePenalty(PENALTY_ON_EXCEPTION, true);
			return static_cast<TCEVENT>(TCEVENT_GAME_OVER);
		}

		m_SeqFrame = (SeqOld == m_Seq) ? m_SeqFrame + 1 : 0;
		return evt;

	} // end of TCPLAYFIELD::Tick

protected:
	TCSEQ	Tick_NotPlaying(TCPLAYER *pPlayer, TCEVENT &evt)
	{
		return SEQ_NOT_PLAYING;

	} // end of TCPLAYFIELD::Tick_NotPlaying

	TCSEQ	Tick_Prep(TCPLAYER *pPlayer, TCEVENT &evt)
	{
		//
		// prep exec
		//
		m_CtrlPiece = m_NextPiece;
		m_NextPiece = TCPIECE();
		m_Score.AddPieces();

		//
		// exec
		//
		m_NextEvent &= TCEVENT_MASK_PRE;
		m_NextEvent |= TCEVENT_PREP;
		m_NextEvent = pPlayer->Exec(m_NextEvent, m_Field, m_CtrlPiece, m_NextPiece);

		//
		// post exec
		//
		m_NextPiece.Randomize(m_Random);

		// rotate before falling
		if (
			(m_NextEvent & TCEVENT_ROT_L) &&
			(m_NextEvent & TCEVENT_ROT_R)
		) {
			// 相殺される
			m_NextEvent &= ~TCEVENT_ROT;
		}
		if (
			(m_NextEvent & TCEVENT_ROT_L) ||
			(m_NextEvent & TCEVENT_ROT_R)
		) {
			m_CtrlPiece = m_CtrlPiece.Rotate((m_NextEvent & TCEVENT_ROT_L) ? +1 : -1);
		}

		// first falling, piece come out
		m_CtrlPiece = m_CtrlPiece.MoveY(m_CtrlPiece.GetTopOfsY());

		if (!m_Field.IsSetPiece(m_CtrlPiece)) {
			// coudn't place, i.e. gameover
			m_NextEvent |= TCEVENT_GAME_OVER;
			m_Field.SetPiece(m_CtrlPiece);
			m_CtrlPiece.Invalidate();
			pPlayer->EndGame(m_Score);

			return SEQ_GAME_OVER;
		}

		m_HoverFrame = 0;
		m_LandFrame = 0;

		// repeat for huge gravity
		for (size_t yy = 10; yy < m_Field.GetGravity(); yy += 10) {
			if (!m_Field.IsSetPiece(m_CtrlPiece.MoveY(-1))) {
				break;
			}
			m_CtrlPiece = m_CtrlPiece.MoveY(-1);
		}

		// land bonus check
		if (m_Field.IsSetPiece(m_CtrlPiece.MoveY(-1))) {
				m_NextEvent |= TCEVENT_LAND;
				m_LandFrame = 1;
		}

		return SEQ_FALL;

	} // end of TCPLAYFIELD::Tick_Prep

	TCSEQ	Tick_Fall(TCPLAYER *pPlayer, TCEVENT &evt)
	{
		//
		// prep exec
		//

		//
		// exec
		//
		m_NextEvent &= TCEVENT_MASK_PRE;
		m_NextEvent |= TCEVENT_FALL;
		m_NextEvent = pPlayer->Exec(m_NextEvent, m_Field, m_CtrlPiece, m_NextPiece);

		//
		// post exec
		//
		bool	bRotated = false;
		bool	bMoved = false;

		// rotate while falling
		if (
			(m_NextEvent & TCEVENT_ROT_L) &&
			(m_NextEvent & TCEVENT_ROT_R)
		) {
			// 相殺される
			m_NextEvent &= ~TCEVENT_ROT;
		}
		if (
			(m_NextEvent & TCEVENT_ROT_L) ||
			(m_NextEvent & TCEVENT_ROT_R)
		) {
			TCPIECE	Piece(m_CtrlPiece.Rotate((m_NextEvent & TCEVENT_ROT_L) ? +1 : -1));
			bRotated = m_Field.IsSetPiece(Piece);
			if (bRotated) {
				m_CtrlPiece = Piece;
			} else {
				m_NextEvent |= (m_NextEvent & TCEVENT_ROT) << 8;	// nROT_L or nROT_R
			}
		}

		// move while falling
		if (
			(m_NextEvent & TCEVENT_MOVE_L) &&
			(m_NextEvent & TCEVENT_MOVE_R)
		) {
			// 相殺される
			m_NextEvent &= ~TCEVENT_MOVE;
		}
		if (
			(m_NextEvent & TCEVENT_MOVE_L) ||
			(m_NextEvent & TCEVENT_MOVE_R)
		) {
			TCPIECE	Piece(m_CtrlPiece.MoveX((m_NextEvent & TCEVENT_MOVE_L) ? +1 : -1));
			bMoved = m_Field.IsSetPiece(Piece);
			if (bMoved) {
				m_CtrlPiece = Piece;
			} else {
				m_NextEvent |= (m_NextEvent & TCEVENT_MOVE) << 8;	// nMOVE_L or nMOVE_R
			}
		}

		// fall while falling
		m_HoverFrame += m_Field.GetGravity();
#if 0
		if (!(m_NextEvent & (TCEVENT_MOVE | TCEVENT_ROT))) {
			// 独自ルール：移動・回転どちらも無ければ下入れとして扱う
			m_HoverFrame += 10;
			m_LandFrame += LAND_BONUS_FRAME;
		}
#endif
		if (
			(m_HoverFrame >= 10) ||
			(m_LandFrame >= LAND_BONUS_FRAME)
		) {
			for (size_t yy = 0; yy < m_Field.GetGravity(); yy += 10) {
				if (!m_Field.IsSetPiece(m_CtrlPiece.MoveY(-1))) {
					if (yy == 0) {
						if (m_LandFrame >= LAND_BONUS_FRAME) {
							m_NextEvent |= TCEVENT_LAND;
						} else {
							m_LandFrame++;
						}
					}
					break;
				}
				m_CtrlPiece = m_CtrlPiece.MoveY(-1);
				m_HoverFrame = 0;
				m_LandFrame = 0;
			}
			if (m_Field.IsSetPiece(m_CtrlPiece.MoveY(-1))) {
				if (m_LandFrame == 0) {
					m_LandFrame++;
				}
			}
		} else {
			if (m_LandFrame > 0) {
				m_LandFrame++;
			}
		}

		if (m_NextEvent & TCEVENT_LAND) {
			// piece fixed
			m_Field.SetPiece(m_CtrlPiece);
			m_CtrlPiece.Invalidate();
		}

		return (m_NextEvent & TCEVENT_LAND) ? SEQ_CHECK : SEQ_FALL;

	} // end of TCPLAYFIELD::Tick_Fall

	TCSEQ	Tick_Check(TCPLAYER *pPlayer, TCEVENT &evt)
	{
		if (m_Field.CheckLines() > 0) {
			return SEQ_LINE_OUT;
		}

		size_t	SpotCount = m_Field.CountSpots();
		m_Score.AddSpots(m_SpotCount, SpotCount);
		m_SpotCount = SpotCount;

		return (m_Score.GetPieces() >= m_Score.GetRoundPieces()) ? SEQ_GAME_OVER : SEQ_PREP;

	} // end of TCPLAYFIELD::Tick_Check

	TCSEQ	Tick_LineOut(TCPLAYER *pPlayer, TCEVENT &evt)
	{
		m_Score.AddLines(m_Field.CheckLines());
		m_Field.RemoveLines();
		{
			size_t	SpotCount = m_Field.CountSpots();
			m_Score.AddSpots(m_SpotCount, SpotCount);
			m_SpotCount = SpotCount;
		}

		return (m_Score.GetPieces() >= m_Score.GetRoundPieces()) ? SEQ_GAME_OVER : SEQ_PREP;

	} // end of TCPLAYFIELD::Tick_LineOut

	TCSEQ	Tick_GameOver(TCPLAYER *pPlayer, TCEVENT &evt)
	{
		//
		// exec
		//
		m_NextEvent &= TCEVENT_MASK_PRE;
		m_NextEvent |= TCEVENT_GAME_OVER;
		if (m_Score.GetPieces() >= m_Score.GetRoundPieces()) {
			m_NextEvent |= TCEVENT_ROUND_OVER;
			evt |= TCEVENT_ROUND_OVER;
		} else {
			evt |= TCEVENT_GAME_OVER;
		}
		m_NextEvent = pPlayer->Exec(m_NextEvent, m_Field, m_CtrlPiece, m_NextPiece);

		return SEQ_NOT_PLAYING;

	} // end of TCPLAYFIELD::Tick_GameOver
};



//----------------------------------------------------------------------------
#endif	// TCPLAYFIELD_H
