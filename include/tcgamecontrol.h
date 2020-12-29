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

#ifndef TCGAMECONTROL_H
#define TCGAMECONTROL_H
//! @file

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")	// for gethostname
#include <list>
#include <map>
#include <string>
#include <strstream>
#include <vector>
#include "tctype.h"
#include "tcplayerdll.h"
#include "tcplayfield.h"
#include "tcrandomgenerator.h"
#include "tcstreamwriter.h"
#include "tcstreamwriter_file.h"
#include "tcstring.h"
#include "tcstringlist.h"
#include "tcview.h"
#include "tcview_console.h"

//----------------------------------------------------------------------------
/*!
	FIXME
*/
class TCGAMECONTROL
{
	//
	// assign
	//
	typedef std::map<TCPLAYFIELD*, TCPLAYERDLL*>	GAMECONTENT;
	typedef std::pair<TCPLAYFIELD*, TCEVENT>		EVENTX;
	typedef std::list<EVENTX>						EVENTQUEUE;


private:
	class VIEW
	{
		//
		// assign
		//
	private:
		static const ssize_t		PLAYFIELD_POS_X = 4;
		static const ssize_t		PLAYFIELD_POS_Y = 1;
		static const ssize_t		PLAYFIELD_WIDTH = 19;
		static const ssize_t		TILE_SIZE = 6;
		static const uint8_t		CHR_VOID = ' ';
		static const uint8_t		CHR_FRAME = '#';
		static const uint8_t		CHR_BLOCK = '@';
		static const uint8_t		CHR_PIECE = 'X';

		//
		// variable
		//
	private:
		TCVIEW						*m_pView;
		std::map<std::string, size_t>	m_Effect;

		//
		// method
		//
	public:
		VIEW(TCVIEW *pView)
			: m_pView(pView),
			m_Effect()
		{
			TCASSERT(m_pView != NULL, "alloc null.");

		} // end of TCGAMECONTROL::VIEW::ctor

		~VIEW()
		{
			delete m_pView;
			m_pView = NULL;

		} // end of TCGAMECONTROL::VIEW::dtor

		bool	IsDrawingEffect(const std::string &key)
		{
			return (m_Effect[key] > 0);

		} // end of TCGAMECONTROL::VIEW::IsDrawingEffect

		void	StartEffect(const std::string &key)
		{
			m_Effect[key] = 1;	// kick to start effect

		} // end of TCGAMECONTROL::VIEW::StartEffect

		void	Draw(const GAMECONTENT &container, size_t round, EVENTQUEUE &queue)
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			m_pView->Clear();
			{
				static ssize_t	cnt = 0;

				if (IsDrawingEffect("RoundStart")) {
					Draw_RoundStart(TCVIEW::LOCATOR(0, 0, 0), round);
				} else {
					Draw_Bg();
					Draw_Round(TCVIEW::LOCATOR(0, 0, 0), round);
				}

				TCVIEW::LOCATOR	myloc(PLAYFIELD_POS_X, PLAYFIELD_POS_Y, 0);

				for (GAMECONTENT::const_iterator it = container.begin(); it != container.end(); ++it) {
					bool bGameOver = (
						!it->first->IsPlaying() &&
						((cnt & 0x1f) < 0x18)
					);
					Draw_PlayField(
						myloc,
						bGameOver,
						it->first->GetScore(),
						it->first->GetField(),
						it->first->GetCtrlPiece(),
						it->first->GetNextPiece(),
						it->second->Get()
					);
					if (false) {	// FIXME
#if 1
						while (queue.begin() != queue.end()) {
							if (queue.begin()->first != it->first) {
								break;
							}
							m_pView->DrawFont(myloc + TCVIEW::LOCATOR(1, 0, 0), 'v');
							queue.pop_front();
						}
#endif
						m_pView->DrawFont(myloc + TCVIEW::LOCATOR(0, 0, 0), "0123456789"[queue.size()]);
					}
					myloc += TCVIEW::LOCATOR(PLAYFIELD_WIDTH, 0, 0);
				}
				cnt++;
			}
			m_pView->Paint();

		} // end of TCGAMECONTROL::VIEW::Draw

		void	Draw_Bg() const
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			static ssize_t	ofs = 0;
			ssize_t	sizeX = m_pView->GetSize().GetX();
			ssize_t	sizeY = m_pView->GetSize().GetY();

			ofs++;
			for (ssize_t ii = 0; ii < sizeX; ii++) {
				m_pView->DrawFont(TCVIEW::LOCATOR(ii, sizeY - ((ofs + (sizeX - ii)) % sizeY), 0), "-/|\\"[ofs & 0x00000003U]);
			}

		} // end of TCGAMECONTROL::VIEW::Draw_Bg

		void	Draw_Round(const TCVIEW::LOCATOR &loc, size_t round) const
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			static ssize_t	ofs = 0;
			ssize_t	sizeX = m_pView->GetSize().GetX();

			std::strstream	ss;
			ss
				<< "ROUND "
				<< std::setw(3) << std::setfill('0') << round
				<< "      "
				<< std::ends;
			std::string		content(ss.str());

			ssize_t	size = static_cast<ssize_t>(content.size());
			TCVIEW::LOCATOR	myloc = loc + TCVIEW::LOCATOR(-(ofs % size), 0, 0);

			for (ssize_t ii = 0; ii < sizeX + size; ii += size) {
				m_pView->DrawFont(myloc, content);
				myloc += TCVIEW::LOCATOR(size, 0, 0);
			}
			ofs++;

		} // end of TCGAMECONTROL::VIEW::Draw_Round

		void	Draw_PlayField(const TCVIEW::LOCATOR &loc, bool bGameOver, const TCSCORE &Score, const TCFIELD &Field, const TCPIECE &CtrlPiece, const TCPIECE &NextPiece, const TCPLAYER *pPlayer) const
		{
			TCASSERT(m_pView != NULL, "fatal error.");
			TCASSERT(pPlayer != NULL, "fatal error.");

			{
				TCVIEW::LOCATOR	locFO = loc + TCVIEW::LOCATOR(0, 1, 0) + TCVIEW::LOCATOR(Field.GetSize().GetX() - 1, Field.GetSize().GetY() - 1, 0);
				Draw_Field(locFO, Field);
				Draw_Piece(locFO, CtrlPiece);
				Draw_Piece(locFO, NextPiece);
			}
			{
				TCVIEW::LOCATOR	locD = loc + TCVIEW::LOCATOR(0, 1, 0) + TCVIEW::LOCATOR(0, Field.GetSize().GetY(), 0);
				Draw_ParamString(locD + TCVIEW::LOCATOR(0, 2, 0), "", TCSTRING::Sanitize(std::string(pPlayer->GetName())));
				if (bGameOver) {
					Draw_ParamPercent(locD + TCVIEW::LOCATOR(0, 0, 0), "P/", Score.GetPieceScore());
					Draw_ParamRatio  (locD + TCVIEW::LOCATOR(0, 1, 0), "Lx", Score.GetLineScore());
				} else {
					Draw_ParamInteger(locD + TCVIEW::LOCATOR(0, 0, 0), "P:", Score.GetPieces());
					Draw_ParamInteger(locD + TCVIEW::LOCATOR(0, 1, 0), "L:", Score.GetLines());
					{
						TCVIEW::LOCATOR	locDR = locD + TCVIEW::LOCATOR(7, 0, 0);
						Draw_ParamPercent(locDR + TCVIEW::LOCATOR(0, 0, 0), "R:", Score.GetRecoverScore());
						Draw_ParamInteger(locDR + TCVIEW::LOCATOR(0, 1, 0), "X:", Score.GetPenalties());
					}
				}
			}
			if (bGameOver) {
				std::string	content("GAMEOVER");
				TCVIEW::LOCATOR	locC = loc + TCVIEW::LOCATOR(
					static_cast<ssize_t>(Field.GetSize().GetX() / 2 - content.size() / 2),
					static_cast<ssize_t>(Field.GetSize().GetY() / 2),
					1
				);
				m_pView->DrawFont(locC, content);
			}

		} // end of TCGAMECONTROL::VIEW::Draw_PlayField

		void	Draw_Field(const TCVIEW::LOCATOR &locFO, const TCFIELD &Field) const
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			// ground
			for (int8_t xx = 0; xx < Field.GetSize().GetX(); xx++) {
				m_pView->Draw(locFO - TCVIEW::LOCATOR(xx, 0, 0), CHR_FRAME);
			}

			// game field
			for (int8_t yy = 1; yy < (Field.GetSize().GetY() - 1); yy++) {
				m_pView->Draw(locFO - TCVIEW::LOCATOR(0, yy, 0), CHR_FRAME);
				for (int8_t xx = 1; xx < (Field.GetSize().GetX() - 1); xx++) {
					m_pView->Draw(locFO - TCVIEW::LOCATOR(xx, yy, 0), Field.GetBitmap(xx, yy) ? CHR_BLOCK : CHR_VOID);
				}
				m_pView->Draw(locFO - TCVIEW::LOCATOR((Field.GetSize().GetX() - 1), yy, 0), CHR_FRAME);
			}

			// ceil
			for (int8_t xx = 0; xx < Field.GetSize().GetX(); xx++) {
				if (
					(xx < (Field.GetSize().GetX() - TCPIECE::PIECE_WIDTH) / 2) ||
					((Field.GetSize().GetX() - TCPIECE::PIECE_WIDTH) / 2 + TCPIECE::PIECE_WIDTH <= xx)
				) {
					m_pView->Draw(locFO - TCVIEW::LOCATOR(xx, (Field.GetSize().GetY() - 1), 0), CHR_FRAME);
				}
			}

		} // end of TCGAMECONTROL::VIEW::Draw_Field

		void	Draw_Piece(const TCVIEW::LOCATOR &locFO, const TCPIECE Piece) const
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			if (Piece.m_Fig == TCPIECE::FIG_VOID) {
				return;
			}
			for (int8_t yy = 0; yy < TCPIECE::PIECE_WIDTH; yy++) {
				uint32_t	mask = 1 << 0;
				uint32_t	line = Piece.GetBitmap()[yy];
				for (int8_t xx = 0; xx < TCPIECE::PIECE_WIDTH; xx++) {
					if (line & mask) {
						m_pView->Draw(locFO - TCVIEW::LOCATOR(Piece.m_Pos.GetX() + xx, Piece.m_Pos.GetY() + yy, 0), CHR_PIECE);
					}
					mask <<= 1;
				}
			}

		} // end of TCGAMECONTROL::VIEW::Draw_Piece

		void	Draw_ParamString(const TCVIEW::LOCATOR &loc, std::string prefix, const std::string &param) const
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			std::strstream	content;
			content
				<< prefix
				<< param
				<< std::ends;
			m_pView->DrawFont(loc, std::string(content.str()));

		} // end of TCGAMECONTROL::VIEW::Draw_ParamString

		void	Draw_ParamInteger(const TCVIEW::LOCATOR &loc, std::string prefix, size_t param) const
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			std::strstream	content;
			content
				<< prefix
				<< std::setw(4) << param
				<< std::ends;
			m_pView->DrawFont(loc, std::string(content.str()));

		} // end of TCGAMECONTROL::VIEW::Draw_ParamInteger

		void	Draw_ParamPercent(const TCVIEW::LOCATOR &loc, std::string prefix, size_t param) const
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			std::strstream	content;
			content
				<< prefix
				<< std::setw(3) << param
				<< '%'
				<< std::ends;
			m_pView->DrawFont(loc, std::string(content.str()));

		} // end of TCGAMECONTROL::VIEW::Draw_ParamPercent

		void	Draw_ParamRatio(const TCVIEW::LOCATOR &loc, std::string prefix, size_t param) const
		{
			TCASSERT(m_pView != NULL, "fatal error.");

			std::strstream	content;
			content
				<< prefix
				<< std::setw(1) << (param / 100)
				<< '.'
				<< std::setw(2) << std::setfill('0') << (param % 100)
				<< std::ends;
			m_pView->DrawFont(loc, std::string(content.str()));

		} // end of TCGAMECONTROL::VIEW::Draw_ParamRatio

		void	Draw_RoundStart(const TCVIEW::LOCATOR &loc, size_t round)
		{
			TCASSERT(m_pView != NULL, "fatal error.");

#if 0
			//
			// エフェクト不要
			//
			m_RoundStartEffectCounter = 0;
			for (ssize_t yy = 0; yy < View.GetSize().GetY(); yy++) {
				View.DrawFont(Loc + TCVIEW::LOCATOR(0, yy, 1), std::string(View.GetSize().GetX(), ' ')));
			}
#else
			//
			// タイリング
			//
			if (m_Effect["RoundStart"] <= 0) {
				return;
			}

			if (m_Effect["RoundStart"] >= 1 + TILE_SIZE + TILE_SIZE) {
				m_Effect["RoundStart"] = 0;
				return;
			}

			std::vector<std::string>	tile;
			for (ssize_t yy = 0; yy < TILE_SIZE; yy++) {
				std::string	content;
				size_t	len = min(TILE_SIZE, max(0, TILE_SIZE + TILE_SIZE - (yy + (static_cast<int>(m_Effect["RoundStart"]) - 1))));
				content.append(len, '#');
				content.append(TILE_SIZE - len, ' ');
				tile.push_back(content);
			}

			for (ssize_t yy = 0; yy < m_pView->GetSize().GetY(); yy += static_cast<ssize_t>(tile.size())) {
				for (ssize_t xx = 0; xx < m_pView->GetSize().GetX(); xx += static_cast<ssize_t>(tile[0].size())) {
					m_pView->DrawFont(loc + TCVIEW::LOCATOR(xx, yy, 1), tile);
				}
			}

			m_Effect["RoundStart"]++;
#endif

		} // end of TCGAMECONTROL::VIEW::Draw_RoundStart
	};

	//
	// variable
	//
protected:
	TCSTRINGLIST				m_RuleList;
	TCSTRINGLIST				m_ViewList;
	std::string					m_ReportDst;
	std::string					m_Hostname;

	EVENTQUEUE					m_Queue;
	size_t						m_SkipFrame;	// スキップするフレーム数（1～）

	size_t						m_RoundCount;	// ラウンド数
	size_t						m_Round;		// 現在のラウンド数
	GAMECONTENT					m_Container;

	//
	// method
	//
public:
	static std::string	GetVersion()
	{
		std::strstream	rv;
		rv
			<< TCSTRING::Sanitize(__DATE__)
			<< "-"
			<< TCSTRING::Sanitize(__TIME__)
			<< std::ends;
		return std::string(rv.str());

	} // end of GetVersion

	TCGAMECONTROL(const std::string &RuleOpt, const std::string &ViewOpt, const std::string &ReportDst)
		: m_RuleList(RuleOpt),
		m_ViewList(ViewOpt),
		m_ReportDst(TCSTRING::Embed(ReportDst)),
		m_Hostname(GetHostname()),
		m_Queue(),
		m_SkipFrame(1),
		m_RoundCount(0),
		m_Container()
	{
		TCASSERT(m_RuleList.GetInteger("W") >= 4, "RuleOpt invalid.");
		TCASSERT(m_RuleList.GetInteger("H") >= 4, "RuleOpt invalid.");
		TCASSERT(m_RuleList.GetInteger("G") > 0, "RuleOpt invalid.");
		TCASSERT(m_RuleList.GetInteger("R") > 0, "RuleOpt invalid.");

		TCASSERT(m_ViewList.GetInteger("W") > 0, "ViewOpt invalid.");
		TCASSERT(m_ViewList.GetInteger("H") > 0, "ViewOpt invalid.");
		TCASSERT(m_ViewList.GetInteger("S") > 0, "ViewOpt invalid.");

		m_SkipFrame = m_ViewList.GetInteger("S");
		m_RoundCount = m_RuleList.GetInteger("R");

	} // end of TCGAMECONTROL::ctor

	virtual ~TCGAMECONTROL()
	{
		for (GAMECONTENT::iterator it = m_Container.begin(); it != m_Container.end(); ++it) {
			delete it->first;
//			it->first = NULL;	// MEMO: error C3892 が出てしまうため、ポインタのクリアは map::clear() に委譲した。
			delete it->second;
//			it->second = NULL;	// MEMO: error C3892 が出てしまうため、ポインタのクリアは map::clear() に委譲した。
		}
		m_Container.clear();

	} // end of TCGAMECONTROL::dtor

	std::string	GetString() const
	{
		std::strstream	rv;
		rv
			<< "VER=" << GetVersion()
			<< "&DATE=" << TCSTRING::Embed("<yymmdd><hhmmss>")
			<< "&HOST=" << m_Hostname
			<< "&" << m_RuleList.GetString()
			<< "&R=" << m_Round
			<< std::ends;
		return std::string(rv.str());

	} // end of TCGAMECONTROL::GetString

	void	AddPlayer(const char *strLibName)
	{
		TCPLAYFIELD	*pPlayField = new TCPLAYFIELD(
			static_cast<int8_t>(m_RuleList.GetInteger("W")),
			static_cast<int8_t>(m_RuleList.GetInteger("H")),
			static_cast<int8_t>(m_RuleList.GetInteger("G"))
		);
		TCASSERT(pPlayField != NULL, "alloc null.");

		TCPLAYERDLL	*pPlayerDll = new TCPLAYERDLL(strLibName);
		TCASSERT(pPlayerDll != NULL, "alloc null.");

		// エントリーする
		m_Container[pPlayField] = pPlayerDll;

	} // end of TCGAMECONTROL::AddPlayer

	void	Run()
	{
		// create view
		VIEW	View = VIEW(
			CreateViewer(
				static_cast<int8_t>(m_ViewList.GetInteger("W")),
				static_cast<int8_t>(m_ViewList.GetInteger("H")),
				2
			)
		);

		// open player
		OpenPlayer();

		for (m_Round = 1; m_Round <= m_RoundCount; m_Round++) {
			// start game
			m_Queue.resize(0);
			m_Queue = StartGame();
			View.StartEffect("RoundStart");

			// exec loop
			while (IsPlaying()) {
				// tick
				if (!View.IsDrawingEffect("RoundStart")) {
					// エフェクトが非アクティブのときのみ、ゲームを進行させる
					for (size_t ii = 0; ii < m_SkipFrame; ii++) {
						EVENTQUEUE	queueTick = Tick();
						m_Queue.insert(m_Queue.end(), queueTick.begin(), queueTick.end());
					}
				}

				// draw
				View.Draw(m_Container, m_Round, m_Queue);
			}

			// end game
			EndGame();
		}

	} // end of TCGAMECONTROL::Run

private:
	virtual TCVIEW	*CreateViewer(int8_t PosX, int8_t PosY, int8_t PosZ)
	{
		// 状況をコンソール画面に出力する
		return new TCVIEW_CONSOLE(PosX, PosY, PosZ);

	} // end of TCGAMECONTROL::CreateViewer

	virtual TCSTREAMWRITER	*CreateReporter(const std::string &ReportDst)
	{
		// 結果をファイルに出力する
		return new TCSTREAMWRITER_FILE(ReportDst);

	} // end of TCGAMECONTROL::CreateReporter

	virtual void	OutputResult(const std::string &content)
	{
		// send a result to server
		TCSTREAMWRITER	*pReport = CreateReporter(m_ReportDst);

		pReport->Get()
			<< content
			<< std::ends;
		pReport->Submit();

		delete pReport;
		pReport = NULL;

	} // end of TCGAMECONTROL::OutputResult

	void	OpenPlayer()
	{
		for (GAMECONTENT::const_iterator it = m_Container.begin(); it != m_Container.end(); ++it) {
			bool rv = it->second->Open(m_RuleList.GetString().c_str());
			TCASSERT(rv, "open failed.");
		}

	} // end of TCGAMECONTROL::OpenPlayer

	EVENTQUEUE	StartGame()
	{
		TCRANDOMGENERATOR	randgen(0);
		EVENTQUEUE	queue;
		for (GAMECONTENT::const_iterator it = m_Container.begin(); it != m_Container.end(); ++it) {
			TCEVENT	evt = it->first->StartGame(randgen, it->second->Get());
			if (evt != 0) {
				queue.insert(queue.end(), EVENTX(it->first, evt));
			}
		}
		return queue;

	} // end of TCGAMECONTROL::StartGame

	void	EndGame()
	{
		// setup result content
		std::strstream	rv;
		rv << GetString();
		for (GAMECONTENT::const_iterator it = m_Container.begin(); it != m_Container.end(); ++it) {
			rv
				<< ","
				<< it->second->Get()->GetString()
				<< "&"
				<< it->first->GetScore().GetString();
		}
		rv << std::ends;

		OutputResult(std::string(rv.str()));

	} // end of TCGAMECONTROL::EndGame

	bool	IsPlaying() const
	{
		bool	bPlaying = false;
		for (GAMECONTENT::const_iterator it = m_Container.begin(); it != m_Container.end(); ++it) {
			bPlaying = bPlaying || it->first->IsPlaying();
		}
		return bPlaying;

	} // end of TCGAMECONTROL::IsPlaying

	EVENTQUEUE	Tick()
	{
		EVENTQUEUE	queue;
		for (GAMECONTENT::const_iterator it = m_Container.begin(); it != m_Container.end(); ++it) {
			TCEVENT	evt = it->first->Tick(it->second->Get());
			if (evt != 0) {
				queue.insert(queue.end(), EVENTX(it->first, evt));
			}
		}
		return queue;

	} // end of TCGAMECONTROL::Tick

private:
	/*!
		実行中のホスト名を取得します。
		@param[in] bInit ソケットレイヤの初期化（WSAStartup）を実施するかどうか
		@return ホスト名
	*/
	static std::string	GetHostname(bool bInit = true)
	{
		if (bInit) {
			WSADATA	data;
			WSAStartup(MAKEWORD(2, 0), &data);
		}

		std::string	rv;
		char	strName[64] = { '\0', };
		if (FAILED(gethostname(strName, sizeof(strName)))) {
			TCVERBOSE("::gethostname failed.");
			rv = std::string("(unknown)");
		} else {
			rv = std::string(strName);
		}

		if (bInit) {
			WSACleanup();
		}
		return rv;

	} // end of TCGAMECONTROL::GetHostname
};



//----------------------------------------------------------------------------
#endif // TCGAMECONTROL_H
