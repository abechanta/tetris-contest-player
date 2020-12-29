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

#ifndef TCSCORE_H
#define TCSCORE_H
//! @file

#include <string>
#include <strstream>
#include <vector>
#include "tctype.h"

//----------------------------------------------------------------------------
/*!
	プレイヤの戦績を保持するためのクラスです。
	１ラウンドのみの戦績や過去の累積的な戦績どちらにも対応しています。
	加算に対応しています。
*/
class TCSCORE
{
	//
	// assign
	//
private:
	static const size_t	PIECES_PER_ROUND = 1000;

	//
	// variable
	//
private:
	ssize_t					m_Rounds;		//!< ラウンド数（累計）
	ssize_t					m_Pieces;		//!< ピース数（累計）
	std::vector<ssize_t>	m_Lines;		//!< ライン数（累計）／同時ライン消しの回数（累計）
	ssize_t					m_Penalties;	//!< ペナルティ数（累計）
	ssize_t					m_SpotsInc;		//!< スポット面積の増加回数（累計）
	ssize_t					m_SpotsDec;		//!< スポット面積の減少回数（累計）

	//
	// method
	//
public:
	/*!
		デフォルトのコンストラクタです。
		各メンバはゼロで初期化されます。
	*/
	TCSCORE()
		: m_Rounds(0),
		m_Pieces(0),
		m_Penalties(0),
		m_SpotsInc(0),
		m_SpotsDec(0)
	{
		m_Lines.resize(5);

	} // end of TCSCORE::ctor

	std::string	GetString() const
	{
		std::strstream	Value;
		Value
			<< "P=" << GetPieces()
			<< "&L=" << GetLines()
			<< "&L1=" << m_Lines[1]
			<< "&L2=" << m_Lines[2]
			<< "&L3=" << m_Lines[3]
			<< "&L4=" << m_Lines[4]
			<< "&X=" << GetPenalties()
			<< "&SI=" << m_SpotsInc
			<< "&SD=" << m_SpotsDec
			<< "&PR=" << GetPieceScore()
			<< "&LR=" << GetLineScore()
			<< "&RR=" << GetRecoverScore()
			<< std::ends;
		return std::string(Value.str());

	} // end of TCSCORE::GetString

	/*!
		戦績をクリアします（各メンバはゼロで初期化されます）。
	*/
	void	Clear()
	{
		m_Rounds = 0;
		m_Pieces = 0;
		m_Lines.resize(0);
		m_Lines.resize(5);
		m_Penalties = 0;
		m_SpotsInc = 0;
		m_SpotsDec = 0;

	} // end of TCSCORE::Clear

	/*!
		戦績を合算します。
		複数のラウンド成績を合算するときに使用します。
		@param[in] Rhs ラウンド成績のインスタンス
		@return このインスタンス
	*/
	TCSCORE	&operator+=(const TCSCORE &Rhs)
	{
		m_Rounds += Rhs.m_Rounds;
		m_Pieces += Rhs.m_Pieces;
		for (size_t ii = 0; ii < m_Lines.size(); ii++) {
			m_Lines[ii] += Rhs.m_Lines[ii];
		}
		m_Penalties += Rhs.m_Penalties;
		m_SpotsInc += Rhs.m_SpotsInc;
		m_SpotsDec += Rhs.m_SpotsDec;
		return *this;

	} // end of TCSCORE::operator+=

	/*!
		保持されているラウンド数を返します。
		@return ラウンド数
	*/
	ssize_t	GetRounds() const
	{
		return m_Rounds;

	} // end of TCSCORE::GetRounds

	/*!
		ラウンドの上限ピース数を返します。
		@return ラウンドの上限ピース数
	*/
	ssize_t	GetRoundPieces() const
	{
		return PIECES_PER_ROUND;

	} // end of TCSCORE::GetRoundPieces

	/*!
		保持されているピース数を返します。
		@return ピース数
	*/
	ssize_t	GetPieces() const
	{
		return m_Pieces;

	} // end of TCSCORE::GetPieces

	/*!
		保持しているピース数を増加させます。
		ピース数は「達成率」を算出するのに使用されます。
		@param[in] Add 増加させるピース数（省略時は 1）
	*/
	void	AddPieces(ssize_t Add = 1)
	{
		if (m_Rounds == 0) {
			m_Rounds = 1;
		}
		m_Pieces += Add;

	} // end of TCSCORE::AddPieces

	/*!
		保持されているライン数を返します。
		@return ライン数
	*/
	ssize_t	GetLines() const
	{
		return m_Lines[0];

	} // end of TCSCORE::GetLines

	/*!
		保持しているライン数を増加させます。
		一度に消したライン数は、合計のライン数に反映されるだけでなく、「ライン率」を算出するのに使用されます。
		@param[in] Lines 消したライン数（1～4）
	*/
	void	AddLines(ssize_t Lines)
	{
		if (m_Rounds == 0) {
			m_Rounds = 1;
		}
		if ((1 <= Lines) && (Lines <= 4)) {
			m_Lines[0] += Lines;	// add as a total lines
			m_Lines[Lines]++;		// add as times of lines removed
		}

	} // end of TCSCORE::AddLines

	/*!
		保持されているペナルティ数を返します。
		@return ペナルティ数
	*/
	ssize_t	GetPenalties() const
	{
		return m_Penalties;

	} // end of TCSCORE::GetPenalties

	/*!
		保持しているペナルティ数を増加させます。
		ペナルティ数は「達成率」を算出するのに使用されます。
		@param[in] Add 増加させるペナルティ（省略時は 1）
	*/
	void	AddPenalties(ssize_t Add = 1)
	{
		if (m_Rounds == 0) {
			m_Rounds = 1;
		}
		m_Penalties += Add;

	} // end of TCSCORE::AddPenalties

	/*!
		保持しているスポット面積の増減回数を集計します。
		スポット面積とは、ゲームプレイ中のフィールド内にある穴の面積です。
		スポット面積の増減回数は「復活率」を算出するのに使用されます。
		@param[in] OldCount スポット面積が変化する前のスポット面積
		@param[in] NowCount スポット面積が変化した後のスポット面積
	*/
	void	AddSpots(size_t OldSpots, size_t NowSpots)
	{
		if (m_Rounds == 0) {
			m_Rounds = 1;
		}
		if (NowSpots > OldSpots) {
			m_SpotsInc++;
		} else if (NowSpots < OldSpots) {
			m_SpotsDec++;
		}

	} // end of TCSCORE::AddSpots

	/*!
		「達成率」を算出します。
		達成率は１ラウンドのピース数のうちの何パーセントをペナルティ無しに消化したか、で計算されます。
		@return 達成率（0～100）
	*/
	ssize_t	GetPieceScore() const
	{
		ssize_t	Pieces = (m_Pieces < PIECES_PER_ROUND) ? m_Pieces : PIECES_PER_ROUND;
		return (Pieces < m_Penalties) ? 0 : ((100 * (Pieces - m_Penalties)) / PIECES_PER_ROUND);

	} // end of TCSCORE::GetPieceScore

	/*!
		「ライン率」を算出します。
		ライン率は１回のライン消去で何ラインを消したか、で計算されます。
		@return ライン率（0～400）
	*/
	ssize_t	GetLineScore() const
	{
		ssize_t	Times = 0;
		for (size_t ii = 1; ii < m_Lines.size(); ii++) {
			Times += m_Lines[ii];
		}
		return (Times == 0) ? 0 : ((100 * m_Lines[0]) / Times);

	} // end of TCSCORE::GetLineScore

	/*!
		「復活率」を算出します。
		復活率はスポット面積の増加回数に対するスポット面積の減少回数、で計算されます。
		@return 復活率（0～100、100 を超えることもあり得る）
	*/
	ssize_t	GetRecoverScore() const
	{
		return (m_SpotsInc == 0) ? 0 : ((100 * m_SpotsDec) / m_SpotsInc);

	} // end of TCSCORE::GetRecoverScore
};



//----------------------------------------------------------------------------
#endif	// TCSCORE_H
