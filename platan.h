/*
 * platan.h
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
#ifndef PLATAN_H
#define PLATAN_H

#include <QtGui>
#include <qsettings.h>
#include "ui_platan.h"
#include "PlatanApp.h"

class platan : public QWidget
{
    Q_OBJECT

public:
    platan(QWidget *parent = 0);
    ~platan();
    void show();

    void debugOut(QString outstring);

public slots:
    void on_debug_output(QString outstring);
    void on_update_icon( bool state );


protected:
    void initValues();
    void saveValues();
    void closeEvent(QCloseEvent *event);


private slots:
	void on_Quit_clicked();
	void on_Apply_clicked();
	void on_Cancel_clicked();

	void on_netport_valueChanged();
	void on_cmdinversion_stateChanged();
	void on_filterdelay_valueChanged();
	void on_filterdelay_2_valueChanged();
	void on_loosereset_stateChanged();
	void on_outputmethod_currentIndexChanged();
	void on_swstrategy_currentIndexChanged();
	void on_quancomcardlist_currentIndexChanged();
	void on_quancomchannel_valueChanged();
	void on_setMediaFile_clicked();

private:
    Ui::platanClass ui;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *setupAction;
    QAction *quitAction;

    QSettings *settings;

    PlatanApp *platanapp;
    bool app_is_configured;


};


#endif // PLATAN_H
