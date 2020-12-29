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

#ifndef TCGAMECONTROL_CON_H
#define TCGAMECONTROL_CON_H
//! @file

#include <string>
#include <vector>
#include "tctype.h"
#include "tccrypt.h"
#include "tcgamecontrol.h"
#include "tcstreamwriter_http.h"

//----------------------------------------------------------------------------
/*!
	FIXME
*/
class TCGAMECONTROL_CON : public TCGAMECONTROL
{
	//
	// assign
	//

	//
	// variable
	//
private:
	std::string					m_ReportKey;

	//
	// method
	//
public:
	TCGAMECONTROL_CON(const std::string &RuleOpt, const std::string &ViewOpt, const std::string &ReportDst, const std::string &ReportKey)
		: TCGAMECONTROL(RuleOpt, ViewOpt, ReportDst),
		m_ReportKey(ReportKey)
	{
	} // end of TCGAMECONTROL_CON::ctor

	virtual ~TCGAMECONTROL_CON()
	{
	} // end of TCGAMECONTROL_CON::dtor

private:
	virtual TCSTREAMWRITER	*CreateReporter(const std::string &ReportDst)
	{
		// 結果をファイルに出力する
		return new TCSTREAMWRITER_HTTP(ReportDst);

	} // end of TCGAMECONTROL_CON::CreateReporter

	virtual void	OutputResult(const std::string &content)
	{
		// send a result to server
		TCSTREAMWRITER	*pReport = CreateReporter(m_ReportDst);

		pReport->Get()
			<< "KEY="
			<< TCCRYPT::MakeKey(m_ReportKey, content)
			<< "&"
			<< content
			<< std::ends;
		pReport->Submit();

		delete pReport;
		pReport = NULL;

	} // end of TCGAMECONTROL_CON::OutputResult
};



//----------------------------------------------------------------------------
#endif // TCGAMECONTROL_CON_H
