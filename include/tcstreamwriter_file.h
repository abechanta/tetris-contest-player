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

#ifndef TCSTREAMWRITER_FILE_H
#define TCSTREAMWRITER_FILE_H
//! @file

#include <fstream>
#include <string>
#include <strstream>
#include "tctype.h"
#include "tcpath.h"
#include "tcstreamwriter.h"

//----------------------------------------------------------------------------
/*!
	FIXME:
*/
class TCSTREAMWRITER_FILE : public TCSTREAMWRITER
{
	//
	// assign
	//

	//
	// variable
	//
protected:
	std::strstream		m_Content;	//!< 文字列の格納先ストリーム
	TCPATH				m_Path;		//!< 文字列出力先となるファイル名

	//
	// method
	//
public:
	/*!
		文字列出力を担うクラスのインスタンスです。
		FIXME
	*/
	TCSTREAMWRITER_FILE(const std::string &Filename)
		: TCSTREAMWRITER(),
		m_Content(),
		m_Path(Filename)
	{
	} // end of TCSTREAMWRITER_FILE::ctor

	/*!
		デストラクタです。
	*/
	virtual ~TCSTREAMWRITER_FILE()
	{
	} // end of TCSTREAMWRITER_FILE::dtor

	/*!
		入力した文字列を出力します。
		@return 成功したかどうか
	*/
	virtual	std::strstream	&Get()
	{
		return m_Content;

	} // end of TCSTREAMWRITER_FILE::Get

	/*!
		入力した文字列を出力します。
		@return 成功したかどうか
	*/
	virtual	bool	Submit()
	{
		std::ofstream	File;
		File.open(m_Path.Get().c_str(), std::ios::out);
		if (File.is_open()) {
			File
				<< m_Content.str()
				<< std::endl;
		}
		TCASSERT(File.is_open(), "file not opened.");
		return File.is_open();

	} // end of TCSTREAMWRITER_FILE::Submit
};



//----------------------------------------------------------------------------
#endif // TCSTREAMWRITER_FILE_H
