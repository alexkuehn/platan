/*
 * outputmethodmm.cpp
 * This file is part of PLATAN.
 *
 * Copyright (c) 2009 Alexander KŸhn (prj@alexkuehn.de)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "OutputMethodMM.h"


#include <windows.h>

using namespace std;

OutputMethodMM::OutputMethodMM() {

	is_playing = false;

}

OutputMethodMM::~OutputMethodMM() {

	//QString mcicmd = "close \"" + settings->value("output/mm/filename","").toString() + "\"";
	//mciSendString( (WCHAR *)mcicmd.unicode(), NULL, 0,0);
	mciSendString( L"close platanmm", NULL, 0,0);

}

void OutputMethodMM::init(QSettings* setdb)
{
	settings = setdb;
	//QString mcicmd = "open \"" + settings->value("output/mm/filename").toString() + "\" type mpegvideo alias platanmm";
	//mciSendString( (WCHAR *)mcicmd.unicode(), NULL, 0,0);
	QString mcicmd = "open \"" + settings->value("output/mm/filename").toString() + "\" type mpegvideo alias platanmm";
	WCHAR * wstr = new WCHAR[mcicmd.size()+1];
	mcicmd.toWCharArray(wstr);
	wstr[mcicmd.size()] = 0;
	// mciSendString( L"open \"c:\\song.mp3\" type mpegvideo alias platanmm", NULL, 0,0);
	mciSendString( wstr, NULL, 0,0);
	delete[] wstr;
	is_playing = false;

}

void OutputMethodMM::start()
{
	if( is_playing == false )
	{
		is_playing = true;
		mciSendString( L"play platanmm repeat", NULL, 0,0);
	}
	else
	{
		mciSendString( L"resume platanmm", NULL, 0,0);
	}
}

void OutputMethodMM::stop()
{
	if( is_playing == true )
	{
		mciSendString( L"pause platanmm", NULL, 0,0);
	}
}
