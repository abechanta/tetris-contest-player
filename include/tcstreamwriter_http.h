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

#ifndef TCSTREAMWRITER_HTTP_H
#define TCSTREAMWRITER_HTTP_H
//! @file

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#include <string>
#include <strstream>
#include "tctype.h"
#include "tcstreamwriter.h"
#include "tcstring.h"

//----------------------------------------------------------------------------
/*!
	FIXME
*/
class TCSTREAMWRITER_HTTP : public TCSTREAMWRITER
{
	//
	// assign
	//

	//
	// variable
	//
protected:
	std::strstream		m_Content;	//!< 文字列の格納先ストリーム
	std::string			m_Agent;	//!< HTTP クライアントのエージェント名
	std::string			m_Host;		//!< 文字列出力先となる HTTP サーバ名
	uint16_t			m_Port;		//!< 文字列出力先となる HTTP ポート番号
	std::string			m_Rsrc;		//!< 文字列出力先となる HTTP リソース名

	//
	// method
	//
public:
	/*!
		文字列出力を担うクラスのインスタンスです。
		FIXME
	*/
	TCSTREAMWRITER_HTTP(const std::string &Filename)
		: TCSTREAMWRITER(),
		m_Content(),
		m_Agent(),
		m_Host(),
		m_Port(INTERNET_DEFAULT_PORT),
		m_Rsrc()
	{
		{
			std::strstream	Value;
			Value
				<< "tetcon/"
				<< TCSTRING::Sanitize(__DATE__)
				<< "-"
				<< TCSTRING::Sanitize(__TIME__)
				<< std::ends;
			m_Agent.assign(Value.str());
		}
		{
			std::string	ParseLeft(Filename);
			std::string	Extract;

			Extract = TCSTRING::Chop(ParseLeft, ':');
			if (Extract.compare("http") != 0) {
				TCVERBOSE("unknown scheme.");
				return;
			}
			Extract = TCSTRING::Chop(ParseLeft, '/');
			if (Extract.compare("") != 0) {
				TCVERBOSE("unknown uri format.");
				return;
			}
			Extract = TCSTRING::Chop(ParseLeft, '/');
			if (Extract.compare("") != 0) {
				TCVERBOSE("unknown uri format.");
				return;
			}
			Extract = TCSTRING::Chop(ParseLeft, '/');
			if (Extract.compare("") == 0) {
				TCVERBOSE("unknown uri format.");
				return;
			}

			m_Host = TCSTRING::Chop(Extract, ':');
			if (Extract.size() > 0) {
				m_Port = static_cast<uint16_t>(atol(Extract.c_str()));
			}
			m_Rsrc = ParseLeft;
//			m_Rsrc.append(1, '/');
//			m_Rsrc.append(ParseLeft);
		}

	} // end of TCSTREAMWRITER_HTTP::ctor

	/*!
		デストラクタです。
	*/
	virtual ~TCSTREAMWRITER_HTTP()
	{
	} // end of TCSTREAMWRITER_HTTP::dtor
	/*!
		入力した文字列を出力します。
		@return 成功したかどうか
	*/
	virtual	std::strstream	&Get()
	{
		return m_Content;

	} // end of TCSTREAMWRITER_HTTP::Get

	/*!
		入力した文字列を出力します。
		@return 成功したかどうか
	*/
	virtual	bool	Submit()
	{
		HRESULT	rv = S_OK;

		do {
			HINTERNET	hSession;
			hSession = WinHttpOpen(
				TCSTRING::Widen(m_Agent).c_str(),
				WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
				WINHTTP_NO_PROXY_NAME,
				WINHTTP_NO_PROXY_BYPASS,
				0	// synch
			);
			if (hSession == NULL) {
				TCVERBOSE("http client fails w/error.");
				break;
			}

			do {
				HINTERNET	hConnection;
				hConnection = WinHttpConnect(
					hSession,
					TCSTRING::Widen(m_Host).c_str(),
					m_Port,
					0	// reserved
				);
				if (hConnection == NULL) {
					TCVERBOSE("http client fails w/error.");
					break;
				}

				do {
					HINTERNET	hRequest;
					hRequest = WinHttpOpenRequest(
						hConnection,
						L"POST",
						TCSTRING::Widen(m_Rsrc).c_str(),
						0,	// http ver.1.1
						WINHTTP_NO_REFERER,
						WINHTTP_DEFAULT_ACCEPT_TYPES,
						0	// not ssl
					);
					if (hRequest == NULL) {
						TCVERBOSE("http client fails w/error.");
						break;
					}

					do {
						BOOL	bSend;
						bSend = WinHttpSendRequest(
							hRequest,
							WINHTTP_NO_ADDITIONAL_HEADERS,
							0,	// header size
							(LPVOID)(TCSTRING::Widen(m_Content.str()).c_str()),	// FIXME
							sizeof(wchar_t) * static_cast<int>(wcslen(TCSTRING::Widen(m_Content.str()).c_str())),
							sizeof(wchar_t) * static_cast<int>(wcslen(TCSTRING::Widen(m_Content.str()).c_str())),	// total size
							NULL	// context
						);
						if (!bSend) {
							TCVERBOSE("http client fails w/error.");
							break;
						}

						BOOL	bRecv;
						bRecv = WinHttpReceiveResponse(
							hRequest,
							NULL	// reserved
						);
						if (!bRecv) {
							TCVERBOSE("http client fails w/error.");
							break;
						}

						DWORD	StatusCode;
						DWORD	StatusCodeSize = sizeof(StatusCode);
						BOOL	bQuery;
						bQuery = WinHttpQueryHeaders(
							hRequest,
							WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
							WINHTTP_HEADER_NAME_BY_INDEX,
							&StatusCode,
							&StatusCodeSize,
							WINHTTP_NO_HEADER_INDEX
						);
						if (!bQuery) {
							TCVERBOSE("http client fails w/error.");
							break;
						}
						if (StatusCode != HTTP_STATUS_OK) {
							TCVERBOSE("http client fails w/error.");
							break;
						}

						BOOL	bRead;
						char	Buffer[1024];
						DWORD	Size;
						bRead = WinHttpReadData(
							hRequest,
							Buffer,
							sizeof(Buffer),
							&Size
						);
						if (!bRead) {
							TCVERBOSE("http client fails w/error.");
							break;
						}

						// TODO
						std::cout << Buffer;

					} while (false);
					rv = GetLastError();	// FIXME
					WinHttpCloseHandle(hRequest);

				} while (false);
				rv = GetLastError();	// FIXME
				WinHttpCloseHandle(hConnection);

			} while (false);
			rv = GetLastError();	// FIXME
			WinHttpCloseHandle(hSession);

		} while (false);

		return true;

	} // end of TCSTREAMWRITER_HTTP::Submit
};



//----------------------------------------------------------------------------
#endif // TCSTREAMWRITER_HTTP_H
