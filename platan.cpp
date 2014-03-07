/*
 * platan.cpp
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
#include "platan.h"

platan::platan(QWidget *parent)
    : QWidget(parent)
{


	ui.setupUi(this);


	setupAction = new QAction(tr("&Setup"),this);
	connect( setupAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("&Quit"),this);
	connect( quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	trayIconMenu = new QMenu(this),
	trayIconMenu->addAction(setupAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);

	trayIcon->setIcon( QIcon(QString::fromUtf8(":/platan/icons/platan_err_off.svg")) );
	trayIcon->show();

	settings = new QSettings( "AKu", "platan");

	initValues();

	platanapp = new PlatanApp(this);

	if( (settings->value("system/isconfigured", 0).toInt()) != 0 )
	{
		app_is_configured = true;
		ui.Apply->setEnabled(false);
		platanapp->init(settings);
	}
	else
	{
		ui.Cancel->setEnabled(false);
		app_is_configured = false;
	}


	ui.diagnosistext->insertPlainText( "platan started\n");




}

void platan::show()
{
	QWidget::show();
	if( app_is_configured==true)
	{
		hide();
	}
}

void platan::on_update_icon(bool state )
{
	if( state == true )
	{
		trayIcon->setIcon( QIcon(QString::fromUtf8(":/platan/icons/platan_err_on.svg")) );
	}
	else
	{
		trayIcon->setIcon( QIcon(QString::fromUtf8(":/platan/icons/platan_err_off.svg")) );
	}

}

void platan::initValues()
{
	ui.netport->setValue( settings->value("system/net/port", 4200 ).toInt() );
	ui.filterdelay->setValue( settings->value("system/proc/filterdelay", 100 ).toInt() );
	ui.filterdelay_2->setValue( settings->value("system/proc/timerdelay", 1000 ).toInt() );
	ui.cmdinversion->setChecked( settings->value("system/proc/cmdinv", 0).toBool() );
	ui.loosereset->setChecked( settings->value("system/proc/loosereset", 1).toBool() );
	ui.swstrategy->setCurrentIndex( settings->value("system/proc/strategy", 0 ).toInt() );
	ui.outputmethod->setCurrentIndex( settings->value("system/output/method",0).toInt() );
	ui.quancomchannel->setValue( settings->value("output/qc/channel",0).toInt() );

	if( settings->value("output/mm/filestatus",0).toBool() == false )
	{
		ui.mediafilename->setText(tr("none"));
		ui.mediafilename->setDisabled(true);
	}
	else
	{
		QString mediafilename = settings->value("output/mm/filename","").toString();

		if( QFile::exists(mediafilename) == true )
		{
			ui.mediafilename->setText( mediafilename);
			ui.mediafilename->setDisabled(false);
		}
		else
		{
			ui.mediafilename->setText(tr("none"));
			ui.mediafilename->setDisabled(true);
		}
	}
}

void platan::saveValues()
{
	settings->setValue("system/net/port", ui.netport->value() );
	settings->setValue("system/proc/filterdelay", ui.filterdelay->value() );
	settings->setValue("system/proc/timerdelay", ui.filterdelay_2->value() );
	settings->setValue("system/proc/cmdinv", ui.cmdinversion->isChecked() );
	settings->setValue("system/proc/loosereset", ui.loosereset->isChecked() );
	settings->setValue("system/proc/strategy", ui.swstrategy->currentIndex() );
	settings->setValue("system/output/method", ui.outputmethod->currentIndex() );
	settings->setValue("output/qc/channel", ui.quancomchannel->value() );

	if( ui.mediafilename->isEnabled() == true )
	{
		settings->setValue("output/mm/filestatus", 1);
		settings->setValue("output/mm/filename", ui.mediafilename->text());
	}
}

void platan::closeEvent(QCloseEvent *event)
{
	if( app_is_configured == true )
	{
		if( trayIcon->isVisible() )
		{
			hide();
			initValues();
			ui.Apply->setEnabled(false);
			event->ignore();
		}
	}
	else
	{
		qApp->quit();
	}
}

void platan::on_Quit_clicked()
{

	qApp->quit();
}

void platan::on_Apply_clicked()
{
	saveValues();
	if( app_is_configured == false )
	{
		app_is_configured = true;
		settings->setValue("system/isconfigured", 1);
		ui.Cancel->setEnabled(true);
		platanapp->init(settings);
	}
	else
	{
		// app was already running
		platanapp->reinit();
	}
	hide();

}




void platan::on_Cancel_clicked()
{
	initValues();
	ui.Apply->setEnabled(false);
	hide();
}

void platan::on_netport_valueChanged()
{
	ui.Apply->setEnabled(true);
}
void platan::on_cmdinversion_stateChanged()
{
	ui.Apply->setEnabled(true);
}

void platan::on_filterdelay_valueChanged()
{
	ui.Apply->setEnabled(true);
}

void platan::on_filterdelay_2_valueChanged()
{
	ui.Apply->setEnabled(true);
}

void platan::on_loosereset_stateChanged()
{
	ui.Apply->setEnabled(true);
}

void platan::on_outputmethod_currentIndexChanged()
{
	ui.Apply->setEnabled(true);
}

void platan::on_swstrategy_currentIndexChanged()
{
	ui.Apply->setEnabled(true);
}

void platan::on_quancomcardlist_currentIndexChanged()
{
	ui.Apply->setEnabled(true);
}

void platan::on_quancomchannel_valueChanged()
{
	ui.Apply->setEnabled(true);
}

void platan::on_debug_output(QString outstring)
{
	ui.diagnosistext->insertPlainText( outstring + "\n");

}

void platan::on_setMediaFile_clicked()
{
	QString filename = QFileDialog::getOpenFileName( this, tr("Open File"),
			"/home",
			tr("Audio Files (*.mp3 *.wma)"));

	if( filename.isNull() == false )
	{
		ui.Apply->setEnabled(true);
		ui.mediafilename->setText( filename);
		ui.mediafilename->setDisabled(false);
	}
}

platan::~platan()
{
	delete platanapp;
}
