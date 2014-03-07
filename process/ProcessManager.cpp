/*
 * processmanager.cpp
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
#include "ProcessManager.h"



ProcessManager::ProcessManager(QWidget* parent)
{
	timer = new QTimer(this);
	holdtimer = new QTimer(this);
	connect( timer, SIGNAL(timeout()), this, SLOT(on_timer()) );
	connect( holdtimer, SIGNAL(timeout()), this, SLOT(on_holdtimer()) );
	actualmessage = NULL;
	is_debouncing = false;
	state = STATEWAIT;
}

ProcessManager::~ProcessManager()
{
	this->reset();
	if( actualmessage != NULL)
	{
		delete actualmessage;
	}
	delete timer;
	delete holdtimer;
}

void ProcessManager::init(QSettings *setting)
{
	settinglink = setting;

	current_strategy = (strategies)settinglink->value("system/proc/strategy").toInt();
	reset_on_lost = settinglink->value("system/proc/loosereset").toBool();

	if( settinglink->value("system/proc/cmdinv", 0).toBool() == true  )
	{
		triggermsg = EYEOPEN;
	}
	else
	{
		triggermsg = EYECLOSE;
	}

}

void ProcessManager::reinit()
{
	// we need to reinit the process, so reset
	this->reset();

	current_strategy = (strategies)settinglink->value("system/proc/strategy").toInt();
	reset_on_lost = settinglink->value("system/proc/loosereset").toBool();

	if( settinglink->value("system/proc/cmdinv").toBool() == true  )
	{
		triggermsg = EYEOPEN;
	}
	else
	{
		triggermsg = EYECLOSE;
	}
}

void ProcessManager::on_message(InputMessage msg)
{
	// we decide if the message leads to a reset or needs debounced further processing
	if( (msg.eventtype == EYEOPEN) || (msg.eventtype == EYECLOSE) )
	{
		if( is_debouncing == false )
		{
			actualmessage = new InputMessage;
			actualmessage->eventtype = msg.eventtype;
			is_debouncing = true;
			timer->setInterval( settinglink->value("system/proc/filterdelay").toInt() );
			timer->setSingleShot( true );
			timer->start();
		}
		else
		{
			timer->stop();
			actualmessage->eventtype = msg.eventtype;
			timer->setInterval( settinglink->value("system/proc/filterdelay").toInt() );
			timer->setSingleShot( true );
			timer->start();
		}
	}
	else
	{
		if( reset_on_lost == true )
		{
			this->reset();
		}
	}
}

void ProcessManager::reset()
{
	// reset all timers and state machines and set output to off
	timer->stop();
	holdtimer->stop();
	is_debouncing = false;
	OutputMessage outmsg;
	outmsg.eventtype = OFF;
	emit outmessage( outmsg );

	if( actualmessage != NULL)
	{
		delete actualmessage;
	}

	state = STATEWAIT;
}

void ProcessManager::on_timer()
{
	is_debouncing = false;

	emit debugOutput("debounced");
	processStrategy(actualmessage->eventtype);

	delete actualmessage;
	actualmessage = NULL;
}

void ProcessManager::on_holdtimer()
{
	OutputMessage outmsg;
	outmsg.eventtype = OFF;
	emit outmessage( outmsg );

	state = STATEWAIT;
}


void ProcessManager::processStrategy(inputmessage_event msg)
{
	switch(current_strategy)
	{
		case STRATEGY_WHILE:
			{
				switch(state)
				{
					case STATEWAIT:
						if( msg == triggermsg )
						{
							OutputMessage outmsg;
							outmsg.eventtype = ON;
							emit outmessage( outmsg );
							state = STATEON;
						}
						break;
					case STATEON:
						if( msg != triggermsg )
						{
							OutputMessage outmsg;
							outmsg.eventtype = OFF;
							emit outmessage( outmsg );
							state = STATEWAIT;
						}
						break;
					default:
						emit debugOutput("undefined WHILE state");
						break;
				}
			}
			break;

		case STRATEGY_TOGGLE:
		{
			switch(state)
			{
				case STATEWAIT:
					if( msg == triggermsg )
					{
						OutputMessage outmsg;
						outmsg.eventtype = ON;
						emit outmessage( outmsg );
						state = STATEON;
					}
					break;
				case STATEON:
					if( msg != triggermsg )
					{
						state = STATEWAITON;
					}
					break;
				case STATEWAITON:
					if( msg == triggermsg )
					{
						OutputMessage outmsg;
						outmsg.eventtype = OFF;
						emit outmessage( outmsg );
						state = STATEWAITOFF;
					}
					break;
				case STATEWAITOFF:
					if( msg != triggermsg )
					{
						state = STATEWAIT;
					}
					break;

				default:
					emit debugOutput("undefined TOGGLE state");
					break;
			}
		}

			break;
		case STRATEGY_TIMED:
		{
			switch(state)
			{
				case STATEWAIT:
					if( msg == triggermsg )
					{
						OutputMessage outmsg;
						outmsg.eventtype = ON;
						emit outmessage( outmsg );
						state = STATEON;
						holdtimer->setInterval( settinglink->value("system/proc/timerdelay").toInt());
						holdtimer->setSingleShot( true );
						holdtimer->start();
					}
					break;
				case STATEON:
					break;
				default:
					emit debugOutput("undefined TIMED state");
					break;
			}
		}

			break;
		default:
			emit debugOutput("undefined strategy");
			break;
	}
}
