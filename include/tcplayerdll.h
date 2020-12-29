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

#ifndef TCPLAYERDLL_H
#define TCPLAYERDLL_H
//! @file

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <string>
#include "tctype.h"
#include "tcpath.h"
#include "tcplayer.h"

//----------------------------------------------------------------------------
/*!
	TCPLAYER インスタンスを生成する DLL を保持するためのクラスです。
*/
class TCPLAYERDLL
{
	//
	// assign
	//
private:
	/*!
		DLL に含まれる文字列情報を返すインタフェースの型です。
	*/
	typedef const char*(_cdecl *PFGETSTRING)();

	/*!
		DLL に含まれる TCPLAYER インスタンスを生成するインタフェースの型です。
	*/
	typedef TCPLAYER*(_cdecl *PFCREATEINSTANCE)(const char *strPath, const char *strRule);

	//
	// variable
	//
private:
	HINSTANCE		m_hDll;		//!< DLL のハンドル
	TCPLAYER		*m_pPlayer;	//!< TCPLAYER のインスタンス
	TCPATH			m_LibName;	//!< DLL のファイル名

	//
	// method
	//
public:
	/*!
		デフォルトコンストラクタです。
		各メンバはゼロで初期化されます。
		@param[in] strLibName DLL のファイル名
	*/
	TCPLAYERDLL(const char *strLibName)
		: m_hDll(NULL),
		m_pPlayer(NULL),
		m_LibName(strLibName)
	{
	} // end of TCPLAYERDLL::ctor

	/*!
		デストラクタです。
		Close が実行されます。
	*/
	~TCPLAYERDLL()
	{
		Close();

	} // end of TCPLAYERDLL::dtor

	/*!
		指定されたルールに沿うインスタンスを生成します。
		@param[in] strRule ルールを示す文字列
		@retval true 成功
		@retval false 失敗（DLL が見つからない or 文字列情報を取得できない or ルールに対応した TCPLAYER インスタンスを生成できない）
	*/
	bool	Open(const char *strRule)
	{
		if (m_hDll) {
			// unload first in order to reload
			FreeLibrary(m_hDll);
			m_hDll = NULL;
		}

		m_hDll = LoadLibraryA(m_LibName.Get().c_str());
		if (!m_hDll) {
			return false;
		}

		PFGETSTRING			pfGetName = (PFGETSTRING)GetProcAddress(m_hDll, "tcGetName");
		if (!pfGetName) {
			return false;
		}
#if DEBUG
		TCVERBOSE(pfGetName());
#endif

		PFCREATEINSTANCE	pfCreateInstance = (PFCREATEINSTANCE)GetProcAddress(m_hDll, "tcCreateInstance");
		if (!pfCreateInstance) {
			return false;
		}

		m_pPlayer = pfCreateInstance(m_LibName.Get().c_str(), strRule);
		if (!m_pPlayer) {
			return false;
		}

		return true;

	} // end of TCPLAYERDLL::Open

	/*!
		生成したインスタンスやリソースをクローズします。
		デストラクタから実行されているので、このメソッドをコールしなくてもリソースリークは発生しません。
	*/
	void	Close()
	{
		if (m_pPlayer) {
			delete m_pPlayer;
			m_pPlayer = NULL;
		}

		if (m_hDll) {
			FreeLibrary(m_hDll);
			m_hDll = NULL;
		}

	} // end of TCPLAYERDLL::Close

	/*!
		保持している TCPLAYER のインスタンスを返します。
		@return DLL に対応する TCPLAYER インスタンス
	*/
	TCPLAYER	*Get() const
	{
		return m_pPlayer;

	} // end of TCPLAYERDLL::Get
};



//----------------------------------------------------------------------------
#endif	// TCPLAYERDLL_H
