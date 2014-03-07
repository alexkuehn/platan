/*
 * processmanager.h
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
#ifndef PROCESSMANAGER_H_
#define PROCESSMANAGER_H_

#include <QObject>
#include <QTimer>
#include <QSettings>

#include "../input/inputmessage.h"
#include "../output/outputmessage.h"

enum strategies
{
	STRATEGY_WHILE=0,
	STRATEGY_TOGGLE,
	STRATEGY_TIMED
};

#define STATEWAIT 0
#define STATEON 1
#define STATEWAITON 2
#define STATEWAITOFF 3


class ProcessManager : public QObject
{
	Q_OBJECT

public:
	ProcessManager(QWidget* parent);
	virtual ~ProcessManager();
	void init(QSettings *setting);
	void reinit();

signals:
	void debugOutput(QString outstring);
	void outmessage(OutputMessage outmsg );


private slots:
	void on_message(InputMessage msg);
	void on_timer();
	void on_holdtimer();

private:
	QSettings *settinglink;
	QTimer *timer;
	QTimer *holdtimer;
	InputMessage *actualmessage;
	bool is_debouncing;
	strategies current_strategy;
	bool reset_on_lost;
	inputmessage_event triggermsg;
	quint8 state;

	void reset();
	void processStrategy(inputmessage_event msg);

};

#endif /* PROCESSMANAGER_H_ */
