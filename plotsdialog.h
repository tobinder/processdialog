//Copyright (c) 2013 Tobias Binder.
//
//This software was developed at the University of Heidelberg by
//Tobias Binder and Arthur Kuehlwein.
//Enquiries shall be directed to tobias.binder@iwr.uni-heidelberg.de.
//
//Redistribution and use in source and binary forms, with or without 
//modification, are permitted provided that the following conditions are met:
//
//- Redistributions of source code must retain the above copyright notice,
//  this list of conditions and the following disclaimer.
//- Redistributions in binary form must reproduce the above copyright notice, 
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//- All advertising materials mentioning features or use of this software must 
//  display the following acknowledgement: ``This product includes the
//  processdialog package developed by Tobias Binder and others''.
//- The name of the author must not be used to endorse or promote products 
//  derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
//WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
//MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
//EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
//OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
//WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
//OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
//ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#ifndef PLOTSDIALOG_H
#define PLOTSDIALOG_H

#include <QDialog>
#include <QProcess>

#include "ui_plotsdialog.h"

class PlotsDialog : public QDialog
{
    Q_OBJECT   

public:
    PlotsDialog(QWidget *parent = 0);
    Ui::PlotsDialog *ui;

    QString getPath(QString pathAndFilename = "");
    QString getFilename(QString pathAndFilename = "");
    bool fileExists(const char* filename = "");
    bool directoryExists(const char* pzPath = "");

    void loadSettings();
    void loadSettingsLegacy();

private slots:
    void on_image100Browse_clicked();
    void on_image200Browse_clicked();
    void on_image300Browse_clicked();
    void on_image400Browse_clicked();
    void on_image500Browse_clicked();
    void on_image600Browse_clicked();
    void on_image101Browse_clicked();
    void on_image201Browse_clicked();
    void on_image301Browse_clicked();
    void on_image401Browse_clicked();
    void on_image501Browse_clicked();
    void on_image601Browse_clicked();
    void on_predictionBrowse_clicked();
    void on_cloudyPlotsBrowse_clicked();
    void cloudyBands();

    void on_profilesBrowse_clicked();
    void on_histogramPlotsBrowse_clicked();
    void doubleHistogram();

    void readProcessStdout();
    void readProcessError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError error);
    void killProcess();

    void saveSettings();
    void setSuffix();

    void updateImage100FileName();
    void updateImage200FileName();
    void updateImage300FileName();
    void updateImage400FileName();
    void updateImage500FileName();
    void updateImage600FileName();
    void updateImage101FileName();
    void updateImage201FileName();
    void updateImage301FileName();
    void updateImage401FileName();
    void updateImage501FileName();
    void updateImage601FileName();
    void updatePredictionDirectoryName();
    void updateCloudyPlotsDirectoryName();
    void updateProfilesDirectoryName();
    void updateHistogramPlotsDirectoryName();

private:
    QProcess process;

    QString parametersFileName;
    QString suffix;

    QString image100FileName;
    QString image200FileName;
    QString image300FileName;
    QString image400FileName;
    QString image500FileName;
    QString image600FileName;
    QString image101FileName;
    QString image201FileName;
    QString image301FileName;
    QString image401FileName;
    QString image501FileName;
    QString image601FileName;
    QString imageDirectoryName;
    QString predictionDirectoryName;
    QString cloudyPlotsDirectoryName;

    QString profilesDirectoryName;
    QString histogramPlotsDirectoryName;

    bool processRunning;
    bool noNewLine;

    void disableAll();
    void checkPathesSet();

    void errorPrediction();
};

#endif
