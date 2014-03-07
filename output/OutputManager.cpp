/*
 * ouputmanager.cpp
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
#include "OutputManager.h"

#include "OutputMethodDebug.h"
#include "OutputMethodMM.h"
#include "OutputMethodQC.h"

OutputManager::OutputManager() {
	outmethod = NULL;
}

OutputManager::~OutputManager() {
	if( outmethod != NULL )
	{
		delete outmethod;
	}
}

void OutputManager::on_outmessage(OutputMessage msg)
{
	if( msg.eventtype == ON)
	{
		emit debugOutput("switched ON");
		emit updateOutputState(true);

		if( outmethod != NULL )
		{
			outmethod->start();
		}
	}
	else
	{
		emit debugOutput("switched OFF");
		emit updateOutputState(false);
		if( outmethod != NULL )
		{
			outmethod->stop();
		}

	}
}

void OutputManager::init(QSettings *setting)
{
	settinglink = setting;

	currentmethod = (methods) settinglink->value("system/output/method").toInt();

	switch( currentmethod )
	{
		case DEBUG:
			outmethod = new OutputMethodDebug();
			break;
		case QUANCOM:
			outmethod = new OutputMethodQC();
			break;
		case MULTIMEDIA:
			outmethod = new OutputMethodMM();
			break;
	}
	outmethod->init( settinglink );

}

void OutputManager::reinit()
{
	currentmethod = (methods) settinglink->value("system/output/method").toInt();
	if( outmethod != NULL)
	{
		delete outmethod;
	}

	switch( currentmethod )
	{
		case DEBUG:
			outmethod = new OutputMethodDebug();
			break;
		case QUANCOM:
			outmethod = new OutputMethodQC();
			break;
		case MULTIMEDIA:
			outmethod = new OutputMethodMM();
			break;
	}

	outmethod->init( settinglink );
}
