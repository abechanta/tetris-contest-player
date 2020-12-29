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

//! @file
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>
#define TCDECLARE_OBJECT
#include "tcgamecontrol.h"

static const char	*g_strRuleOpt = "RULE=LONGRUN&W=10&H=18&G=10&R=10";
static const char	*g_strViewOpt = "W=79&H=25&S=1";
static const char	*g_strReportDst = "tetcon_result_<yymmdd>_<hhmmss>.log";



//----------------------------------------------------------------------------
void	PrintCopyright()
{
	std::cerr
		<< "tetcon "
		<< "(build: "
		<< TCGAMECONTROL::GetVersion()
		<< "), Copyright (c) Abe Chanta"
		<< std::endl
		<< std::endl;

} // end of PrintCopyright



void	PrintUsage()
{
	std::cerr
		<< "Usage:" << std::endl
		<< "  tetcon.exe [-rule:<ruleopt>] [-view:<viewopt>] [-report:<dest>] <playerdll> ..." << std::endl
		<< std::endl
		<< "Option:" << std::endl
		<< "  available ruleopt is: RULE=LONGRUN, W=<4..30>, H=<4..30>, G=<1..200>, R=<1..>" << std::endl
		<< "    default is '" << g_strRuleOpt << "'" << std::endl
		<< "  available viewopt is: W=<1..>, H=<1..>, S=<1..>" << std::endl
		<< "    default is '" << g_strViewOpt << "'" << std::endl
		<< "  available dest scheme is: file" << std::endl
		<< "    default is '" << g_strReportDst << "'" << std::endl
		<< std::endl
		<< "Example:" << std::endl
		<< "  > tetcon.exe myplayer1.dll myplayer2.dll myplayer3.dll" << std::endl
		<< "  > tetcon.exe -rule:RULE=LONGRUN&W=10&H=18&G=10&R=1 myplayer1.dll" << std::endl
		<< "  > tetcon.exe -view:W=79&H=24&S=5 myplayer1.dll" << std::endl
		<< "  > tetcon.exe -report:tetcon_result.log myplayer1.dll" << std::endl
		<< std::endl;

} // end of PrintUsage



//----------------------------------------------------------------------------
int main(int argc, const char *argv[])
{
	PrintCopyright();

	std::vector<const char*>	Players;
	for (ssize_t ii = 1; ii < argc; ii++) {
		if (argv[ii][0] == '-') {
			if (strncmp(argv[ii], "-rule:", strlen("-rule:")) == 0) {
				g_strRuleOpt = &argv[ii][strlen("-rule:")];
			} else if (strncmp(argv[ii], "-view:", strlen("-view:")) == 0) {
				g_strViewOpt = &argv[ii][strlen("-view:")];
			} else if (strncmp(argv[ii], "-report:", strlen("-report:")) == 0) {
				g_strReportDst = &argv[ii][strlen("-report:")];
			} else {
				PrintUsage();
				exit(1);
			}
		} else {
			Players.push_back(argv[ii]);
		}
	}
	if (Players.size() == 0) {
		PrintUsage();
		exit(1);
	}

	DWORD	msStart = GetTickCount();
	{
		TCGAMECONTROL	ctrl(g_strRuleOpt, g_strViewOpt, g_strReportDst);
		for (size_t ii = 0; ii < Players.size(); ii++) {
			ctrl.AddPlayer(Players[ii]);
		}
		ctrl.Run();
	}

	DWORD	msEnd = GetTickCount();
	std::cerr << std::endl;
	std::cerr
		<< "tetcon.exe: "
		<< std::setw(2) << ((msEnd - msStart) / (1000 * 60 * 60)) << "h "
		<< std::setw(2) << ((msEnd - msStart) / (1000 * 60)) % 60 << "m "
		<< std::setw(2) << ((msEnd - msStart) / (1000)) % 60 << "s "
		<< std::endl;

	return 0;

} /* end of main */
