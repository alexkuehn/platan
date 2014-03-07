/*
 * inputmanager.cpp
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
#include "InputManager.h"

#include <QString>

static const msgtabletype msgtable[] =
{
	{ "EO", EYEOPEN},
	{ "EC", EYECLOSE},
	{ "TL", TRACKERLOST},
	{ "UP", SYSTEMUP},
	{ "DN", SYSTEMDOWN}
};

InputManager::InputManager()
{
	socket = NULL;
}

InputManager::~InputManager()
{
	if( socket != NULL )
	{
		delete socket;
	}
}

void InputManager::init(QSettings *setting)
{
	settinglink = setting;

	socket = new QUdpSocket(this);
	socket->bind( settinglink->value("system/net/port").toInt() );
	emit debugOutput(tr("bound to UDP port:") + settinglink->value("system/net/port").toString() );

	connect( socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()) );

	emit debugOutput(tr("inputmanager initialized"));
}

void InputManager::reinit()
{
	socket->close();
	socket->bind( settinglink->value("system/net/port").toInt() );
	emit debugOutput(tr("rebound to UDP port:") + settinglink->value("system/net/port").toString() );
	//appparent->debugOutput("Input manager initialized");

}

void InputManager::readPendingDatagrams()
{
	while( socket->hasPendingDatagrams() )
	{
		QByteArray datagram;
		datagram.resize(socket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderport;

		socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderport);

		QString messagestring = datagram;

		emit debugOutput(tr("received data from: ") + sender.toString() + tr(" with ") + messagestring );

		processMessage( messagestring );

	}
}

void InputManager::processMessage(QString messagestring)
{
	// parse the message
	QRegExp msgexp ("^([A-Z]{2})(\\d{0,6})");

	if( msgexp.indexIn( messagestring ) >= 0 )
	{
		QString msgcode = msgexp.cap(1);
		QString msgtime = msgexp.cap(2);
		InputMessage decodedmessage;
		decodedmessage.timestamp = msgtime.toULong();

		emit debugOutput( tr("received messagecode:") + msgcode + tr(" with timestamp: ") + msgtime );

		bool found = false;

		for( unsigned int i=0; i < (sizeof(msgtable)/sizeof(msgtable[0])); i++)
		{
			if( msgcode == msgtable[i].msgstring )
			{
				found = true;
				decodedmessage.eventtype = msgtable[i].eventtype;
			}

		}

		if( found == false )
		{
			emit debugOutput( tr("message not decodeable"));
		}
		else
		{
			emit message( decodedmessage );
		}
	}

}
