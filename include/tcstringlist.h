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

#ifndef TCSTRINGLIST_H
#define TCSTRINGLIST_H
//! @file

#include <cstdlib>
#include <map>
#include <string>
#include <strstream>
#include "tctype.h"
#include "tcstring.h"

//----------------------------------------------------------------------------
/*!
	パラメータ文字列からキーワードと値（文字列や数値）を切り出すためのクラスです。
*/
class TCSTRINGLIST
{
	//
	// assign
	//

	//
	// variable
	//
private:
	std::map<std::string, std::string>	m_Container;	//!< 文字列コンテナ

	//
	// method
	//
public:
	/*!
		コンストラクタです。
		引数に渡されたパラメータ文字列を分析し、内部コンテナに保持します。
		@param[in] strLine パラメータ文字列
		@param[in] delim1 パラメータ文字列のパラメータ列を区切る文字
		@param[in] delim2 パラメータ列を区切る文字
	*/
	TCSTRINGLIST(const char *strLine, char delim1 = '&', char delim2 = '=')
		: m_Container()
	{
		std::string	line(strLine);
		while (line.size() > 0) {
			std::string	kk;
			std::string	vv;
			vv = TCSTRING::Chop(line, delim1);
			kk = TCSTRING::Chop(vv, delim2);
			if (kk.size() > 0) {
				m_Container[kk] = vv;
			}
		}

	} // end of TCSTRINGLIST::ctor

	/*!
		コンストラクタです。
		引数に渡されたパラメータ文字列を分析し、内部コンテナに保持します。
		@param[in] Line パラメータ文字列
		@param[in] delim1 パラメータ文字列のパラメータ列を区切る文字
		@param[in] delim2 パラメータ列を区切る文字
	*/
	TCSTRINGLIST(const std::string &line_, char delim1 = '&', char delim2 = '=')
		: m_Container()
	{
		std::string	line(line_);
		while (line.size() > 0) {
			std::string	kk;
			std::string	vv;
			vv = TCSTRING::Chop(line, delim1);
			kk = TCSTRING::Chop(vv, delim2);
			if (kk.size() > 0) {
				m_Container[kk] = vv;
			}
		}

	} // end of TCSTRINGLIST::ctor

	/*!
		内部コンテナにキー文字列がいくつあるかを返します。
		@return キー文字列の数
	*/
	size_t	GetKeyCount() const
	{
		return m_Container.size();

	} // end of TCSTRINGLIST::GetKeyCount

	/*!
		内部コンテナを文字列に展開します。
		@return 展開された文字列
	*/
	std::string	GetString() const
	{
		std::strstream	rv;
		for (std::map<std::string, std::string>::const_iterator it = m_Container.begin(); it != m_Container.end(); it++) {
			rv
				<< ((it == m_Container.begin()) ? "" : "&")
				<< it->first
				<< ((it->second.length() > 0) ? "=" : "")
				<< it->second;
		}
		rv << std::ends;
		return std::string(rv.str());

	} // end of TCSTRINGLIST::GetString

	/*!
		内部コンテナにキー文字列を指定して、対応する数値を取得します。
		@param[in] strKey キー文字列
		@return キー文字列に対応する数値
	*/
	ssize_t	GetInteger(const char *strKey)
	{
		return GetInteger(std::string(strKey));

	} // end of TCSTRINGLIST::GetInteger

	/*!
		内部コンテナにキー文字列を指定して、対応する数値を取得します。
		@param[in] Key キー文字列
		@return キー文字列に対応する数値
	*/
	ssize_t	GetInteger(const std::string &Key)
	{
		return strtol(m_Container[Key].c_str(), NULL, 10);

	} // end of TCSTRINGLIST::GetInteger

	/*!
		内部コンテナにキー文字列を指定して、対応する文字列を取得します。
		@param[in] strKey キー文字列
		@return キー文字列に対応する文字列
	*/
	std::string	GetString(const char *strKey)
	{
		return GetString(std::string(strKey));

	} // end of TCSTRINGLIST::GetString

	/*!
		内部コンテナにキー文字列を指定して、対応する文字列を取得します。
		@param[in] Key キー文字列
		@return キー文字列に対応する文字列
	*/
	std::string	GetString(const std::string &Key)
	{
		return m_Container[Key];

	} // end of TCSTRINGLIST::GetString
};



//----------------------------------------------------------------------------
#endif	// TCSTRINGLIST_H
