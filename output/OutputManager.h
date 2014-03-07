/*
 * outputmanager.h
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
#ifndef OUTPUTMANAGER_H_
#define OUTPUTMANAGER_H_

#include <QObject>
#include <QSettings>

#include "outputmessage.h"
#include "OutputMethod.h"



enum methods
{
	DEBUG,
	QUANCOM,
	MULTIMEDIA
};

class OutputManager : public QObject
{
	Q_OBJECT

public:
	OutputManager();
	virtual ~OutputManager();
	void init(QSettings *setting);
	void reinit();

signals:
	void debugOutput(QString outstring);
	void updateOutputState(bool state);

private slots:
	void on_outmessage(OutputMessage msg);

private:
	QSettings *settinglink;
	OutputMethod *outmethod;
	methods currentmethod;

};

#endif /* OUTPUTMANAGER_H_ */
