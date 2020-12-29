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

#ifndef TCSTREAMWRITER_H
#define TCSTREAMWRITER_H
//! @file

#include <strstream>
#include "tctype.h"

//----------------------------------------------------------------------------
/*!
*/
class TCSTREAMWRITER
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
public:
	/*!
		文字列出力を担うクラスのインスタンスです。
	*/
	TCSTREAMWRITER() {}

	/*!
		デストラクタです。
	*/
	virtual ~TCSTREAMWRITER() {}

	/*!
		入力した文字列を出力します。
		@return 成功したかどうか
	*/
	virtual	std::strstream	&Get() = 0;

	/*!
		入力した文字列を出力します。
		@return 成功したかどうか
	*/
	virtual	bool	Submit() = 0;
};



//----------------------------------------------------------------------------
#endif // TCSTREAMWRITER_H
