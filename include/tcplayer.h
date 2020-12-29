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

#ifndef TCPLAYER_H
#define TCPLAYER_H
//! @file

#include <string>
#include <vector>
#include "tctype.h"
#include "tcevent.h"
#include "tcscore.h"
#include "tcstring.h"

//----------------------------------------------------------------------------
class TCFIELD;
class TCPIECE;

/*!
	派生させた TCPLAYER を DLL で実装するときに必要となる宣言をまとめたマクロです。
	DLL のメインとなる cpp ファイルの先頭に一度だけ配置しなければなりません。
*/
#define TCDECLARE_TCPLAYER()	\
	extern "C"	__declspec(dllexport)	const char*	_cdecl	tcGetName();\
	extern "C"	__declspec(dllexport)	const char*	_cdecl	tcGetAuthor();\
	extern "C"	__declspec(dllexport)	TCPLAYER*	_cdecl	tcCreateInstance(const char *strPath, const char *strRule);

/*!
	派生させた TCPLAYER を DLL で実装するときに必要となるクラス定義をまとめたマクロです。
	派生クラス内のクラス定義に含めなければなりません。
*/
#define TCDEFINE_TCPLAYER()	\
	public:\
		const char	*GetName() const\
		{\
			return tcGetName();\
		}\
		\
		const char	*GetAuthor() const\
		{\
			return tcGetAuthor();\
		}\
		\
	private:

/*!
	派生させた TCPLAYER を DLL で実装するときに必要となる定義をまとめたマクロです。
	DLL のメインとなる cpp ファイルの先頭に一度だけ配置しなければなりません。
	第 1 仮引数は派生クラスの型名。
	第 2 仮引数は派生クラスのプログラム名（文字列）。
	第 3 仮引数は派生クラスのバージョン番号（文字列）。
	第 4 仮引数は派生クラスの作者（文字列）。
*/
#define TCEXPORT_TCPLAYER(USER_PLAYER_T, Name, Author)	\
	__declspec(dllexport)	const char*	_cdecl	tcGetName()\
	{\
		TCASSERT(TCSTRING::Sanitize(std::string(Name)).size() > 0, "invalid name.");\
		return (Name);\
	}\
	\
	__declspec(dllexport)	const char*	_cdecl	tcGetAuthor()\
	{\
	TCASSERT(TCSTRING::Sanitize(std::string(Author)).size() > 0, "invalid author name.");\
		return (Author);\
	}\
	\
	__declspec(dllexport)	TCPLAYER*	_cdecl	tcCreateInstance(const char *strPath, const char *strRule)\
	{\
		return USER_PLAYER_T::IsAvailable(strRule) ? new USER_PLAYER_T(strPath, strRule) : NULL;\
	}\
	size_t	TCPLAYER::m_InstanceCount = 0;



/*!
	プレイヤ入力をシミュレートしている思考ルーチンのインタフェースです。
	プログラマが新たな思考ルーチンを実装するときは、このインタフェースに準拠している必要があります。
*/
class TCPLAYER
{
	//
	// assign
	//

	//
	// variable
	//
private:
	static size_t			m_InstanceCount;	//!< 生成されたインスタンスの数
protected:
	size_t					m_InstanceId;		//!< 同じ思考ルーチンから生成された複数のインスタンスを区別するための ID（0～）
	std::string				m_Path;				//!< 使用すべきワークパス
	std::string				m_Rule;				//!< 競技ルール

	//
	// method
	//
public:
	/*!
		思考ルーチン（＝プレイヤ）が指定されたルールに対応しているかどうかを判断します。
		@param[in] strRule 与えられたルール（「LONGRUN」「VERSUS」など）
	*/
	static bool	IsAvailable(const char * /*strRule*/) { return false; }

	/*!
		思考ルーチン（＝プレイヤ）のコンストラクタです。
		@param[in] strPath ワークとして使用できるパス
		@param[in] strRule 与えられたルール（「LONGRUN」「VERSUS」など）
	*/
	TCPLAYER(const char *strPath, const char *strRule)
		: m_InstanceId(m_InstanceCount++),
		m_Path(strPath),
		m_Rule(strRule)
	{
	} // end of TCPLAYER::ctor

	/*!
		デストラクタです。
	*/
	virtual ~TCPLAYER() {}


	std::string	GetString() const
	{
		std::strstream	Value;
		Value
			<< "NAME=" << TCSTRING::Sanitize(GetName())
			<< "&AUTHOR=" << TCSTRING::Sanitize(GetAuthor())
			<< std::ends;
		return std::string(Value.str());

	} // end of TCPLAYER::GetString

	/*!
		思考ルーチンが持っているプログラム名（バージョン番号を含む）を返します。
		使える文字は英字、数字、スペース、ドット、ハイフン、アンダースコア、スラッシュです。
		プログラム名の表示は 15 文字まで保証されます。
		@return プログラム名（文字列）
	*/
	virtual const char	*GetName() const = 0;

	/*!
		思考ルーチンが持っている作者を返します。
		使える文字は英字、数字、スペース、ドット、ハイフン、アンダースコア、スラッシュです。
		作者の表示は 8 文字まで保証されます。
		@return 作者（文字列）
	*/
	virtual const char *GetAuthor() const = 0;

	/*!
		思考ルーチンに対し、ラウンドスタートを通知します。
		@param[in] Field フィールドのインスタンス
	*/
	virtual void	StartGame(const TCFIELD &Field) = 0;

	/*!
		思考ルーチンに対し、ラウンド終了とそのスコアを通知します。
		@param[in] Score １ラウンド分のスコア
	*/
	virtual void	EndGame(const TCSCORE &Score) = 0;

	/*!
		思考ルーチンに対し、次のプレイヤ入力を要求します。
		@param[in] Event 前のフレーム（プレイヤ入力）以降に発生したイベント（シーケンス＋入力フィードバック）
		@param[in] Field フィールドのインスタンス
		@param[in] CtrlPiece 操作中の落下ピースのインスタンス
		@param[in] NextPiece 次の落下ピースのインスタンス
		@return プレイヤ入力イベント
	*/
	virtual TCEVENT Exec(TCEVENT Event, const TCFIELD &Field, const TCPIECE &CtrlPiece, const TCPIECE &NextPiece) = 0;
};



//----------------------------------------------------------------------------
#endif	// TCPLAYER_H
