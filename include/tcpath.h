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

#ifndef TCPATH_H
#define TCPATH_H
//! @file

#include <string>
#include "tctype.h"
#include "tcstring.h"

//----------------------------------------------------------------------------
/*!
	パス名演算をまとめたクラスです。
	URI は扱えません。
*/
class TCPATH
{
	//
	// assign
	//

	//
	// variable
	//
private:
	std::string					m_Name;		//!< パス名の本体
	size_t						m_Offset;	//!< 一番末尾に近いパスデリミタの位置

	//
	// method
	//
public:
	/*!
		標準のコンストラクタです。絶対パス／相対パスどちらも指定可能です。
		@param[in] Name パス名
	*/
	TCPATH(const std::string Name)
		: m_Name(Name),
		m_Offset(std::string::npos)
	{
		m_Offset = m_Name.find_last_of("/\\");

	} // end of TCPATH::ctor

	/*!
		コピーコンストラクタです。
		各メンバはコピー元のインスタンスに初期化されます。
		@param[in] Rhs コピー元のインスタンス
	*/
	TCPATH(const TCPATH &Rhs)
		: m_Name(Rhs.m_Name),
		m_Offset(Rhs.m_Offset)
	{
	} // end of TCPATH::ctor

	/*!
		代入演算子です。
		各メンバはコピー元のインスタンスに初期化されます。
		@param[in] Rhs コピー元のインスタンス
		@return このインスタンス
	*/
	TCPATH	&operator=(const TCPATH &Rhs)
	{
		m_Name   = Rhs.m_Name;
		m_Offset = Rhs.m_Offset;
		return *this;

	} // end of TCPATH::operator=

	/*!
		パス名を取得します。
		@return パス名
	*/
	std::string	Get() const
	{
		return m_Name;

	} // end of TCPATH::Get

	/*!
		パスのフォルダ名を取得します。末尾にはパスデリミタが残ります。
		@return フォルダ名
	*/
	std::string	GetFolder() const
	{
		std::string	rv(m_Name);
		if (m_Offset == std::string::npos) {
			// not found
			rv.erase(0);
			rv.append(".\\");
		} else {
			// found
			rv.erase(m_Offset);
			rv.append(1, '\\');
		}
		return rv;

	} // end of TCPATH::GetFolder

	/*!
		パスのファイル名を取得します。
		@return ファイル名
	*/
	std::string	GetFile() const
	{
		std::string	rv(m_Name);
		if (m_Offset != std::string::npos) {
			// found
			rv.erase(0, m_Offset + 1);
		}
		return rv;

	} // end of TCPATH::GetFile

	/*!
		パス名とファイル名を連結して取得します。
		パス名にファイル名が含まれているときは、ファイル名が置き換えられます。

		下記特定文字列の置き換えも行います。
		<yymmdd> → 日付から取得した年月日。
		<hhmmss> → 日付から取得した時分秒。
		@param[in] Name ファイル名
		@return 連結されたパス名
	*/
	std::string	Concat(const std::string &File) const
	{
		std::string	rv(GetFolder());
		rv.append(File);
		return rv;

	} // end of TCPATH::Concat
};



//----------------------------------------------------------------------------
#endif	// TCPATH_H
