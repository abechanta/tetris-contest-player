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

#ifdef COMMENT

・落下口付近の積み上げ減点
・目標 TCPIECE の配列化（回転入れ対応・転がし対応）
・遺伝的アルゴリズム

#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include "tctype.h"
#include "tcevent.h"
#include "tcfield.h"
#include "tcpath.h"
#include "tcpiece.h"
#include "tcplayer.h"
#include "tcrandomgenerator.h"
#include "tcsize.h"
#include "tcstringlist.h"
#include "atu.h"

//----------------------------------------------------------------------------
TCDECLARE_TCPLAYER()

class ATPLAYER1 : public TCPLAYER
{
	TCDEFINE_TCPLAYER();

	//
	// assign
	//
public:
	static const size_t	DEBUG_LEVEL_DEFAULT = DEBUG;
private:
	class FETCHER
	{
		//
		// assign
		//

		//
		// variable
		//
	private:
		std::vector<int8_t>					m_Array;
		std::vector<int8_t>::const_iterator	m_Current;

		//
		// method
		//
	public:
		FETCHER(const std::string &filename)
		{
			std::ifstream	iniFile(filename.c_str());
			while (iniFile) {
				int		data;
				char	dump;
				iniFile >> data;
				iniFile >> dump;
				m_Array.push_back(static_cast<int8_t>(data));
			}
			m_Current = m_Array.begin();

		} // end of FETCHER::ctor

		int8_t	Get()
		{
			return (m_Current != m_Array.end()) ? *m_Current++ : 0;

		} // end of FETCHER::Get
	};

	//
	// variable
	//
private:
	static size_t		m_DebugLevel;
	TCPATH				m_Path;
	std::ofstream		m_LogFile;
	std::ofstream		m_ResFile;

	TCPIECE				m_Target;
	ssize_t				m_EvalPts;
	int8_t				m_HeightRatio;
	int8_t				m_EdgePosiRatio;
	int8_t				m_EdgeNegaRatio;
	std::vector<int8_t>	m_SpotPts;
	std::vector<int8_t>	m_LinePts;
	std::vector<int8_t>	m_DiffPts;
	std::vector<int8_t>	m_CliffPts;
	std::vector<int8_t>	m_OverPts;

	//
	// method
	//
public:
	static bool	IsAvailable(const char *strRule)
	{
		TCSTRINGLIST	RuleList(strRule);
		return RuleList.GetString("RULE") == "LONGRUN";

	} // end of ATPLAYER1::IsAvailable

	ATPLAYER1(const char *strPath, const char *strRule)
		: TCPLAYER(strPath, strRule),
		m_Path(strPath),
		m_LogFile(),
		m_ResFile(),
		m_Target(),
		m_EvalPts(0)
	{
#if DEBUG
		if (
			(m_DebugLevel >= 1) &&
			(m_InstanceId < 8)
		) {
			std::string	LogFile;
			LogFile.append(GetName()).append("\\").append(1, "01234567"[m_InstanceId]).append(".log");
			m_LogFile.open(m_Path.Concat(LogFile).c_str(), std::ios::out);
			TCASSERT(m_LogFile.is_open(), "file not opened.");
		}
#endif
		if (
			(m_InstanceId < 8)
		) {
			std::string	ResFile;
			ResFile.append(GetName()).append("\\").append("<yymmdd>_<hhmmss>_").append(1, "01234567"[m_InstanceId]).append(".txt");
			ResFile = TCSTRING::Embed(ResFile);
			m_ResFile.open(m_Path.Concat(ResFile).c_str(), std::ios::out);
			TCASSERT(m_ResFile.is_open(), "file not opened.");
		}

		{
			FETCHER	param(m_Path.Concat(std::string(GetName()).append(".ini")));
			int8_t	count;

			m_HeightRatio = param.Get();
			m_EdgePosiRatio = param.Get();
			m_EdgeNegaRatio = param.Get();
			count = param.Get();
			for (int8_t ii = 0; ii < count; ii++) {
				m_SpotPts.push_back(param.Get());
			}
			count = param.Get();
			for (int8_t ii = 0; ii < count; ii++) {
				m_LinePts.push_back(param.Get());
			}
			count = param.Get();
			for (int8_t ii = 0; ii < count; ii++) {
				m_DiffPts.push_back(param.Get());
			}
			count = param.Get();
			for (int8_t ii = 0; ii < count; ii++) {
				m_CliffPts.push_back(param.Get());
			}
			count = param.Get();
			for (int8_t ii = 0; ii < count; ii++) {
				m_OverPts.push_back(param.Get());
			}
		}

		{
			// tweak params
			TCRANDOMGENERATOR	randgen(0);
			for (size_t ii = 0; ii < m_InstanceId; ii++) {
				randgen.Get();
			}
			switch ((randgen.Get() / 17) % 26) {
			case 0:		m_HeightRatio++; break;
			case 1:		m_HeightRatio--; break;
			case 2:		m_EdgePosiRatio++; break;
			case 3:		m_EdgePosiRatio--; break;
			case 4:		m_EdgeNegaRatio++; break;
			case 5:		m_EdgeNegaRatio--; break;
			case 6:		if (m_SpotPts.size() > 1) { m_SpotPts.pop_back(); } break;
			case 7:		m_SpotPts.push_back(m_SpotPts.back() + m_SpotPts.back() / 2); break;
			case 8:		m_SpotPts[randgen.Get() % m_SpotPts.size()]++; break;
			case 9:		m_SpotPts[randgen.Get() % m_SpotPts.size()]--; break;
			case 10:	if (m_LinePts.size() > 1) { m_LinePts.pop_back(); } break;
			case 11:	m_LinePts.push_back(m_LinePts.back() + m_LinePts.back() / 2); break;
			case 12:	m_LinePts[randgen.Get() % m_LinePts.size()]++; break;
			case 13:	m_LinePts[randgen.Get() % m_LinePts.size()]--; break;
			case 14:	if (m_DiffPts.size() > 1) { m_DiffPts.pop_back(); } break;
			case 15:	m_DiffPts.push_back(m_DiffPts.back() + m_DiffPts.back() / 2); break;
			case 16:	m_DiffPts[randgen.Get() % m_DiffPts.size()]++; break;
			case 17:	m_DiffPts[randgen.Get() % m_DiffPts.size()]--; break;
			case 18:	if (m_CliffPts.size() > 1) { m_CliffPts.pop_back(); } break;
			case 19:	m_CliffPts.push_back(m_CliffPts.back() + m_CliffPts.back() / 2); break;
			case 20:	m_CliffPts[randgen.Get() % m_CliffPts.size()]++; break;
			case 21:	m_CliffPts[randgen.Get() % m_CliffPts.size()]--; break;
			case 22:	if (m_OverPts.size() > 1) { m_OverPts.pop_back(); } break;
			case 23:	m_OverPts.push_back(m_OverPts.back() + m_OverPts.back() / 2); break;
			case 24:	m_OverPts[randgen.Get() % m_OverPts.size()]++; break;
			case 25:	m_OverPts[randgen.Get() % m_OverPts.size()]--; break;
			default:	TCASSERT(false, "unexpected."); break;
			} // endswitch
		}

		m_ResFile
			<< "#HR=" << static_cast<ssize_t>(m_HeightRatio)
			<< "&EP=" << static_cast<ssize_t>(m_EdgePosiRatio)
			<< "&EN=" << static_cast<ssize_t>(m_EdgeNegaRatio)
			<< "&SP=" << ATU::GetParamString(m_SpotPts)
			<< "&LP=" << ATU::GetParamString(m_LinePts)
			<< "&DP=" << ATU::GetParamString(m_DiffPts)
			<< "&CP=" << ATU::GetParamString(m_CliffPts)
			<< "&OP=" << ATU::GetParamString(m_OverPts)
			<< std::endl
			<< std::endl;

	} // end of ATPLAYER1::ctor

	~ATPLAYER1()
	{
	} // end of ATPLAYER1::dtor

	void	StartGame(const TCFIELD &Field)
	{
	} // end of ATPLAYER1::StartGame

	void	EndGame(const TCSCORE &Score)
	{
		m_ResFile
			<< Score.GetString()
			<< std::endl;

	} // end of ATPLAYER1::EndGame

	TCEVENT	Exec(TCEVENT Event, const TCFIELD &Field, const TCPIECE &CtrlPiece, const TCPIECE &NextPiece)
	{
		if (Event & TCEVENT_GAME_OVER) {
			return 0;
		}

		if (Event & TCEVENT_PREP) {
#if DEBUG
			if (m_DebugLevel >= 1) {
				m_LogFile << "-------------------------------------------------------------------------------" << std::endl;
			}
#endif

			TCPIECE	TargetU, TargetD;
			TargetU = CtrlPiece.MoveY(CtrlPiece.GetTopOfsY() - 1);	// -1 is for frame
			TargetD = ATU::LimitD(Field, TargetU);
			m_EvalPts = Eval(Field, TargetD);
			m_Target = TargetD;

			for (uint8_t Rot = 0; Rot < CtrlPiece.GetRotMax(); Rot++) {
				TargetU = CtrlPiece;
				TargetU.m_Rot = Rot;
				TargetU = TargetU.MoveY(TargetU.GetTopOfsY() - 1);	// -1 is for frame

				if (!Field.IsSetPiece(TargetU)) {
					continue;
				}

				if (Rot != 0) {
					TargetD = ATU::LimitD(Field, TargetU);
					ssize_t	Score = Eval(Field, TargetD);
					if (m_EvalPts < Score) {
						m_EvalPts = Score;
						m_Target = TargetD;
					}
				}

				TCPIECE	LimitL = ATU::LimitL(Field, TargetU);
				while (ATU::operator !=(LimitL, TargetU)) {
					TargetD = ATU::LimitD(Field, LimitL);
					ssize_t	Score = Eval(Field, TargetD);
					if (m_EvalPts < Score) {
						m_EvalPts = Score;
						m_Target = TargetD;
					}
					LimitL = LimitL.MoveX(-1);
				}

				TCPIECE	LimitR = ATU::LimitR(Field, TargetU);
				while (ATU::operator !=(LimitR, TargetU)) {
					TargetD = ATU::LimitD(Field, LimitR);
					ssize_t	Score = Eval(Field, TargetD);
					if (m_EvalPts < Score) {
						m_EvalPts = Score;
						m_Target = TargetD;
					}
					LimitR = LimitR.MoveX(+1);
				}
			}
		}

		if ((Event & TCEVENT_FALL) && (Event & (TCEVENT_nMOVE_L | TCEVENT_nMOVE_R))) {
			// TODO: copy and paste
#if DEBUG
			if (m_DebugLevel >= 1) {
				m_LogFile << "-------------------------------------------------------------------------------" << std::endl;
			}
#endif

			TCPIECE	TargetU, TargetD;
//			TargetU = CtrlPiece.MoveY(CtrlPiece.GetTopOfsY() - 1);	// -1 is for frame
			TargetU = CtrlPiece;
			TargetD = ATU::LimitD(Field, TargetU);
			m_EvalPts = Eval(Field, TargetD);
			m_Target = TargetD;

			for (uint8_t Rot = 0; Rot < CtrlPiece.GetRotMax(); Rot++) {
				TargetU = CtrlPiece;
				TargetU.m_Rot = Rot;
//				TargetU = TargetU.MoveY(TargetU.GetTopOfsY() - 1);	// -1 is for frame

				if (!Field.IsSetPiece(TargetU)) {
					continue;
				}

				if (Rot != 0) {
					TargetD = ATU::LimitD(Field, TargetU);
					ssize_t	Score = Eval(Field, TargetD);
					if (m_EvalPts < Score) {
						m_EvalPts = Score;
						m_Target = TargetD;
					}
				}

				TCPIECE	LimitL = ATU::LimitL(Field, TargetU);
				while (ATU::operator !=(LimitL, TargetU)) {
					TargetD = ATU::LimitD(Field, LimitL);
					ssize_t	Score = Eval(Field, TargetD);
					if (m_EvalPts < Score) {
						m_EvalPts = Score;
						m_Target = TargetD;
					}
					LimitL = LimitL.MoveX(-1);
				}

				TCPIECE	LimitR = ATU::LimitR(Field, TargetU);
				while (ATU::operator !=(LimitR, TargetU)) {
					TargetD = ATU::LimitD(Field, LimitR);
					ssize_t	Score = Eval(Field, TargetD);
					if (m_EvalPts < Score) {
						m_EvalPts = Score;
						m_Target = TargetD;
					}
					LimitR = LimitR.MoveX(+1);
				}
			}
		}

		return ATU::GetMoveEvent(CtrlPiece, m_Target) | ATU::GetRotEvent(CtrlPiece, m_Target);

	} // end of ATPLAYER1::Exec

	ssize_t	Eval(const TCFIELD &Field, const TCPIECE &Piece)
	{
		return Eval(ATU::operator +(Field, Piece));

	} // end of ATPLAYER1::Eval

	ssize_t	Eval(const TCFIELD &oField)
	{
		TCFIELD	Field(oField);
		int8_t	ww = static_cast<int8_t>(Field.GetSize().GetX());
		int8_t	hh = static_cast<int8_t>(Field.GetSize().GetY());

		ssize_t	LinesBonus = 0;
		{
			size_t	idx = Field.CheckLines();
			if (idx >= m_LinePts.size()) {
				idx = m_LinePts.size() - 1;
			}
			LinesBonus += m_LinePts[idx];
		}

		Field.RemoveLines();
		std::vector<int8_t>	Spots = ATU::GetSpots(Field);
		std::vector<int8_t>	Height = ATU::GetHeight(Field);
		std::vector<int8_t>	SpotHeight = ATU::GetSpotHeight(Field);

		ssize_t	SpotDeduct = 0;
		Spots[0] = 0;
		Spots[hh - 1] = 0;
		for (int8_t yy = 1; yy < hh - 1; yy++) {
			size_t	idx = Spots[yy];
			if (idx >= m_SpotPts.size()) {
				idx = m_SpotPts.size() - 1;
			}
			SpotDeduct += m_SpotPts[idx];
		}

		ssize_t	TotalHeightDeduct = 0;
		Height[0] = 0;
		Height[ww - 1] = 0;
		for (int8_t xx = 1; xx < ww - 1; xx++) {
			if (Height[xx] >= (hh - 1) / 2) {
				TotalHeightDeduct += m_HeightRatio * (Height[xx] - (hh - 1) / 2);
			}
		}

		ssize_t	DiffHeightDeduct = 0;
		Height[0] = Height[1];
		Height[ww - 1] = Height[ww - 2];
		for (int8_t xx = 1; xx < ww - 1; xx++) {
			size_t	idx = (Height[xx - 1] > Height[xx]) ? (Height[xx - 1] - Height[xx]) : (Height[xx] - Height[xx - 1]);
			if (idx >= m_DiffPts.size()) {
				idx = m_DiffPts.size() - 1;
			}
			DiffHeightDeduct += m_DiffPts[idx];
		}

		ssize_t	CliffDeduct = 0;
		Height[0] = Height[2];
		Height[ww - 1] = Height[ww - 3];
		for (int8_t xx = 1; xx < ww - 1; xx++) {
			ssize_t	diffR = Height[xx - 1] - Height[xx];
			ssize_t	diffL = Height[xx + 1] - Height[xx];
			if ((diffR > 0) && (diffL > 0)) {
				size_t	idx = diffR + diffL;
				if (idx >= m_CliffPts.size()) {
					idx = m_CliffPts.size() - 1;
				}
				CliffDeduct += m_CliffPts[idx];
			}
		}

		ssize_t	EdgeDeduct = 0;
		if (Height[2] > Height[1]) {
			EdgeDeduct += m_EdgeNegaRatio * (Height[2] - Height[1]);
		} else {
			EdgeDeduct += m_EdgePosiRatio * (Height[2] - Height[1]);
		}
		if (Height[ww - 3] > Height[ww - 2]) {
			EdgeDeduct += m_EdgeNegaRatio * (Height[ww - 3] - Height[ww - 2]);
		} else {
			EdgeDeduct += m_EdgePosiRatio * (Height[ww - 3] - Height[ww - 2]);
		}

		ssize_t	OverSpotDeduct = 0;
		for (int8_t xx = 1; xx < ww - 1; xx++) {
			size_t	idx = SpotHeight[xx];
			if (idx >= m_OverPts.size()) {
				idx = m_OverPts.size() - 1;
			}
			OverSpotDeduct += m_OverPts[idx];
		}

#if DEBUG
		if (m_DebugLevel >= 1) {
			m_LogFile << std::endl;

			m_LogFile
				<< "ID=" << m_InstanceId << std::endl
				<< "LinesBonus=" << LinesBonus << std::endl
				<< "SpotDeduct=" << SpotDeduct << std::endl
				<< "TotalHeightDeduct=" << TotalHeightDeduct << std::endl
				<< "DiffHeightDeduct=" << DiffHeightDeduct << std::endl
				<< "EdgeDeduct=" << EdgeDeduct << std::endl
				<< "CliffDeduct=" << CliffDeduct << std::endl
				<< "OverSpotDeduct=" << OverSpotDeduct << std::endl
				<< "Total=" << LinesBonus - SpotDeduct - TotalHeightDeduct - DiffHeightDeduct - EdgeDeduct - CliffDeduct - OverSpotDeduct << std::endl
				;

#if 1
			for (int8_t yy = 1; yy < hh - 1; yy++) {
				int8_t	tmpY = hh - 1 - yy;
				for (int8_t xx = 1; xx < ww - 1; xx++) {
					int8_t	tmpX = ww - 1 - xx;
					m_LogFile << (oField.GetBitmap(tmpX, tmpY) ? '@' : '.');
				}
				m_LogFile << std::endl;
			}
#else
			for (int8_t yy = 1; yy < hh - 1; yy++) {
				int8_t	tmpY = hh - 1 - yy;
				uint32_t	line = oField.GetBitmap(tmpY);
				uint32_t	mask = TCBIT::Get(ww);
				mask >>= 1;	// dummy for frame
				for (ssize_t xx = 1; xx < ww - 1; xx++) {
					mask >>= 1;
					m_LogFile << ((line & mask) ? '@' : '.');
				}
				m_LogFile << std::endl;
			}
#endif
		}
#endif

		return LinesBonus - SpotDeduct - TotalHeightDeduct - DiffHeightDeduct - EdgeDeduct - CliffDeduct - OverSpotDeduct;

	} // end of ATPLAYER1::Eval
};

TCEXPORT_TCPLAYER(ATPLAYER1, "ATPLAYER1.01", "Abe Chanta")
size_t	ATPLAYER1::m_DebugLevel = ATPLAYER1::DEBUG_LEVEL_DEFAULT;
