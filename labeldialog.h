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
#ifndef LABELDIALOG_H
#define LABELDIALOG_H

#include <QDialog>
#include <QProcess>

#include "ui_labeldialog.h"

class LabelDialog : public QDialog
{
    Q_OBJECT

public:
    LabelDialog(QWidget *parent = 0);
    Ui::LabelDialog *ui;
    QString labelRfDir;
    QString labelRfName;

    QString pixelRfDirectoryName;
    QString pixelRfName;

    QString getPath(QString pathAndFilename = "");
    QString getFilename(QString pathAndFilename = "");
    bool fileExists(const char* filename = "");
    bool directoryExists(const char* pzPath = "");

    void loadSettings();
    void loadSettingsLegacy();

public slots:
    void saveSettings();

private slots:
    void on_originalBrowse_clicked();
    void on_preprocessedBrowse_clicked();
    void on_pixelLabelsBrowse_clicked();
    void on_pixelClassificationBrowse_clicked();
    void on_pixelRfBrowse_clicked();
    void on_pixelProbBrowse_clicked();
    void on_imageBrowse_clicked();
    void updatePixelRfName();
    void pixelLabel();
    void pixelRf();

    void on_segmentationBrowse_clicked();
    void on_boundaryLabelsBrowse_clicked();
    void on_boundaryFeaturesBrowse_clicked();
    void on_boundaryClassificationBrowse_clicked();
    void on_boundaryRfBrowse_clicked();
    void on_imageBrowse2_clicked();
    void updateBoundaryRfName();
    void boundaryLabel();
    void boundaryRf();

    void readProcessStdout();
    void readProcessError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError error);
    void killProcess();

    void updateImageDirectoryName();
    void updatePreprocessedDirectoryName();
    void updatePixelLabelsDirectoryName();
    void updatePixelClassificationDirectoryName();
    void updatePixelRfDirectoryName();
    void updatePixelProbDirectoryName();
    void updateSegmentationDirectoryName();
    void updateBoundaryLabelsDirectoryName();
    void updateBoundaryFeaturesDirectoryName();
    void updateBoundaryClassificationDirectoryName();
    void updateBoundaryRfDirectoryName();
    void setSuffix();

    void refreshSelectedImages(int index);
    void lhide_Paths();

private:
    QProcess process;

    QString parametersFileName;
    QString suffix;

    QString imageDirectoryName;
    QString preprocessedDirectoryName;
    QString pixelLabelsDirectoryName;
    QString pixelClassificationDirectoryName;
    QString pixelProbDirectoryName;   
    QStringList selectedImages;

    QString segmentationDirectoryName;
    QString boundaryLabelsDirectoryName;
    QString boundaryFeaturesDirectoryName;
    QString boundaryClassificationDirectoryName;
    QString boundaryRfDirectoryName;
    QString boundaryRfName;
    QStringList selectedImages2;

    bool pixelClassificationRunning;
    bool boundaryClassificationRunning;
    bool processRunning;
    bool noNewLine;   

    void disableAll();
    void checkPathesSet();
    void checkDirEmpty(QString initialName, QString &newDir, QLineEdit &target);

    void errorPreprocessed();
    void errorPixelProbability();
    void errorSegmentation();

    bool lhidePaths;
};

#endif
