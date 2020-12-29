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

#ifndef TCCRYPT_H
#define TCCRYPT_H
//! @file

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")
#include <fstream>
#include <string>
#include <strstream>
#include <vector>
#include "tctype.h"
#include "tcstreamwriter.h"
#include "tcstring.h"

//----------------------------------------------------------------------------
/*!
	FIXME
*/
class TCCRYPT
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
	static std::string MakeKey(const std::string &key, const std::string &val)
	{
		std::string	rv;
		HCRYPTPROV	hProv = NULL;
		do {
			if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
				if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
					TCVERBOSE("acquire fails w/error.");
					break;
				}
			}

			HCRYPTHASH	hHash = NULL;
			do {
				if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
					TCVERBOSE("create hash fails w/error.");
					break;
				}

				std::vector<uint8_t>	data0(TCSTRING::Decode(key));
				if (data0.size() > 0) {
					if (!CryptHashData(hHash, &data0.at(0), static_cast<DWORD>(data0.size()), 0)) {
						TCVERBOSE("add hash/0 fails w/error.");
						break;
					}
				}

				std::vector<uint8_t>	data1(GetFileContent(GetThisFilename()));
				if (data1.size() > 0) {
					if (!CryptHashData(hHash, &data1.at(0), static_cast<DWORD>(data1.size()), 0)) {
						TCVERBOSE("add hash/1 fails w/error.");
						break;
					}
				}

				std::vector<uint8_t>	data2(GetContent(val));
				if (data2.size() > 0) {
					if (!CryptHashData(hHash, &data2.at(0), static_cast<DWORD>(data2.size()), 0)) {
						TCVERBOSE("add hash/2 fails w/error.");
						break;
					}
				}
				std::vector<uint8_t>	data3(TCSTRING::Decode(key));
				if (data3.size() > 0) {
					if (!CryptHashData(hHash, &data3.at(0), static_cast<DWORD>(data3.size()), 0)) {
						TCVERBOSE("add hash/3 fails w/error.");
						break;
					}
				}

				DWORD	length;
				if (!CryptGetHashParam(hHash, HP_HASHVAL, 0, &length, 0)) {
					TCVERBOSE("get hash fails w/error.");
					break;
				}

				std::vector<uint8_t>	dataR;
				dataR.resize(length);
				if (!CryptGetHashParam(hHash, HP_HASHVAL, &dataR.at(0), &length, 0)) {
					TCVERBOSE("get hash fails w/error.");
					break;
				}

				// TODO
				rv = TCSTRING::Encode(dataR);

			} while (false);
			if ((hHash != NULL) && !CryptDestroyHash(hHash)) {
				TCVERBOSE("destroy hash fails w/error.");
			}

		} while (false);
		if ((hProv != NULL) && !CryptReleaseContext(hProv, 0)) {
			TCVERBOSE("release fails w/error.");
		}

		return rv;

	} // end of TCCRYPT::MakeKey

private:
	/*!
		文字列の内容をバイト列として取得します。
		@param[in] 文字列
		@return バイト列
	*/
	static std::vector<uint8_t>	GetContent(const std::string &line)
	{
		std::vector<uint8_t>	rv;
		for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
			rv.push_back(*it);
		}
		return rv;

	} // end of TCCRYPT::GetContent

	/*!
		ファイルの内容をバイト列として取得します。
		@param[in] ファイル名
		@return バイト列
	*/
	static std::vector<uint8_t>	GetFileContent(const std::string &filename)
	{
		std::vector<uint8_t>	rv;
		std::ifstream	in(filename.c_str(), std::ios::in | std::ios::binary);
		TCASSERT(in.is_open(), "file not opened.");

		char	ch;
		in.get(ch);
		while (!in.eof()) {
			rv.push_back(static_cast<uint8_t>(ch));
			in.get(ch);
		}
		return rv;

	} // end of TCCRYPT::GetFileContent

	/*!
		実行中のプログラム名を取得します。
		@return プログラム名
	*/
	static std::string	GetThisFilename()
	{
		std::string	rv;
		char	strName[256] = { '\0', };
		if (!GetModuleFileNameA(NULL, strName, sizeof(strName) / sizeof(strName[0]))) {
			TCVERBOSE("::GetModuleFileNameA failed.");
		}
		rv = std::string(strName);
		return rv;

	} // end of TCCRYPT::GetThisFilename
};



//----------------------------------------------------------------------------
#endif // TCCRYPT_H
