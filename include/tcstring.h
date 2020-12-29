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

#ifndef TCSTRING_H
#define TCSTRING_H
//! @file

#include <ctime>
#include <iomanip>
#include <string>
#include <strstream>
#include "tctype.h"

//----------------------------------------------------------------------------
/*!
	文字列演算をまとめたクラスです。
*/
namespace TCSTRING
{
	//
	// assign
	//

	//
	// variable
	//

	//
	// method
	//
	/*!
		パラメータ文字列を、指定された区切り文字の前後で２つの文字列に分割します。
		区切り文字が複数ある場合、先頭の区切り文字が優先されます。
		@param[in/out] line パラメータ文字列（コール後は区切り文字以降の文字列）
		@param[in] delim 区切り文字
		@return 区切り文字以前の文字列
	*/
	std::string	Chop(std::string &line, char delim)
	{
		std::string	rv(line);
		size_t	ofs = rv.find(delim);
		if (ofs == std::string::npos) {
			rv = line;
			line.erase();
		} else {
			rv = line.substr(0, ofs);
			line = line.substr(ofs + 1);
		}
		return rv;

	} // end of TCSTRING::Chop

	/*!
		string を１６進数とみなして vector<uint8_t> に変換します。
		１６進数に使われない文字（0～9 および A～F）は０とみなされます。
		末尾の桁が足りない場合は、末尾の桁は無視されます。
		@param[in] line 入力文字列
		@return 出力バイト列
	*/
	std::vector<uint8_t>	Decode(const std::string &line)
	{
		std::vector<uint8_t>	rv;
		size_t	digit = 0;
		uint8_t	sum = 0;
		for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
			sum = (sum << 4) | (
				('0' <= *it) && (*it <= '9') ? *it - '0' : (
					('A' <= *it) && (*it <= 'F') ? 10 + *it - 'A' : (
						('a' <= *it) && (*it <= 'f') ? 10 + *it - 'a' : 0
					)
				)
			);
			if (++digit >= 2) {
				rv.push_back(sum);
				digit = 0;
				sum = 0;
			}
		}
		return rv;

	} // end of TCSTRING::Decode

	/*!
		下記特定文字列の置き換えを行います。
		<yymmdd> → 日付から取得した年月日。
		<hhmmss> → 日付から取得した時分秒。
		@param[in] line 入力文字列
		@return 変換後の文字列
	*/
	std::string	Embed(const std::string &line)
	{
		std::string	rv(line);
		std::string::size_type	idx;

		idx = rv.find("<yymmdd>");
		if (idx != rv.npos) {
			rv.erase(idx, 8);
			std::strstream	yymmdd;
			__time32_t		nowTime;
			struct tm		nowCalendar;
			_time32(&nowTime);
			_localtime32_s(&nowCalendar, &nowTime);
			yymmdd
				<< std::setw(2) << std::setfill('0') << (nowCalendar.tm_year % 100)
				<< std::setw(2) << std::setfill('0') << (nowCalendar.tm_mon + 1)
				<< std::setw(2) << std::setfill('0') << nowCalendar.tm_mday
				<< std::ends;
			rv.insert(idx, std::string(yymmdd.str()));
		}

		idx = rv.find("<hhmmss>");
		if (idx != rv.npos) {
			rv.erase(idx, 8);
			std::strstream	hhmmss;
			__time32_t		nowTime;
			struct tm		nowCalendar;
			_time32(&nowTime);
			_localtime32_s(&nowCalendar, &nowTime);
			hhmmss
				<< std::setw(2) << std::setfill('0') << nowCalendar.tm_hour
				<< std::setw(2) << std::setfill('0') << nowCalendar.tm_min
				<< std::setw(2) << std::setfill('0') << nowCalendar.tm_sec
				<< std::ends;
			rv.insert(idx, std::string(hhmmss.str()));
		}

		return rv;

	} // end of TCSTRING::Embed

	/*!
		string を１６進数とみなして vector<uint8_t> に変換します。
		１６進数に使われない文字（0～9 および A～F）は０とみなされます。
		末尾の桁が足りない場合は、末尾の桁は無視されます。
		@param[in] line 入力文字列
		@return 出力バイト列
	*/
	std::string	Encode(const std::vector<uint8_t> &line)
	{
		std::string	rv;
		static const char	*strDigit = "0123456789ABCDEF";
		for (std::vector<uint8_t>::const_iterator it = line.begin(); it != line.end(); ++it) {
			rv.append(1, strDigit[(*it >> 4) & 0x0f]);
			rv.append(1, strDigit[(*it >> 0) & 0x0f]);
		}
		return rv;

	} // end of TCSTRING::Encode

	/*!
		文字列から「英字、数字、スペース、ドット、ハイフン、アンダースコア、スラッシュ」以外の文字を除去します。
		@param[in] line 入力文字列
		@return サニタイズ後の文字列
	*/
	std::string	Sanitize(const std::string &line)
	{
		// omit minor symbols
		std::string	rv;
		for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
			if (
				isalnum(*it) ||
				strchr(" .-_/", *it)
			) {
				rv.append(1, *it);
			}
		}
		return rv;

	} // end of TCSTRING::Sanitize

	/*!
		string を wstring に変換します。
		@param[in] line 入力文字列
		@return 出力文字列
	*/
	std::wstring	Widen(const std::string &line)
	{
		std::wstring	rv;

		for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
			rv.append(1, static_cast<wchar_t>(*it));
		}
		return rv;

	} // end of TCSTRING::Widen
};



//----------------------------------------------------------------------------
#endif	// TCSTRING_H
