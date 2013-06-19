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
#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>
#include <QProcess>

#include "ui_processdialog.h"

#include "labeldialog.h"
#include "plotsdialog.h"

#include <sstream>

class ProcessDialog : public QDialog, private Ui::ProcessDialog
{
    Q_OBJECT

public:    
    ProcessDialog(QWidget *parent = 0);    

    QString getPath(QString pathAndFilename = "");
    QString getFilename(QString pathAndFilename = "");    

    bool fileExists(const char* filename = "");
    bool directoryExists(const char* pzPath = "");
    void checkDirEmpty(QString initialName, QString &newDir, QLineEdit &target);

    LabelDialog labelDialog;
    PlotsDialog plotsDialog;

private slots:
    void loadSettings();
    void loadSettingsLegacy();
    void loadSettingsEmpty();
    void quit();
    void saveSettings();

    void on_parametersBrowse_clicked();
    void on_thumbsBrowse_clicked();
    void labeling();
    void plots();   

    void on_mosaicBrowse_clicked();
    void on_relativeBrowse_clicked();
    void on_stitchedBrowse_clicked();
    void on_batchListBrowse_clicked();
    void stitchImages();

    void on_originalBrowse_clicked();
    void on_preprocessedBrowse_clicked();
    void on_imageBrowse_clicked();
    void preprocessImage();
    void preprocessImage2();
    void createThumbs();
    void reduceThumbs();

    void on_pixelFeaturesBrowse_clicked();
    void on_probMapBrowse_clicked();
    void on_segmentationBrowse_clicked();
    void on_boundaryFeaturesBrowse_clicked();
    void on_imageBrowse1_clicked();
    void segmentImage();
    void corrSegm();

    void on_predictionBrowse_clicked();
    void on_plotsBrowse_clicked();
    void on_profilesBrowse_clicked();
    void on_imageBrowse2_clicked();
    void loadProb();
    void loadSegm();
    void correctNetwork();
    void extractNetwork();
    void getParameters();
    void depthProfiles();
    void findSubgrains();

    void on_screenshotsBrowse_clicked();
    void on_imageBrowse3_clicked();
    void viewGrains();
    void viewBoundaries();
    void viewJunctions();
    void analyzeJunctions();
    void grainOverview();
    void boundaryOverview();
    void localCurvature();

    void on_screenshotsBrowse2_clicked();
    void on_analyzeListBrowse_clicked();
    void plotParam();
    void listParam();
    void plotCorrelation();
    void analyze();
    void newProfiles();

    void readProcessStdout();
    void readProcessError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError error);
    void killProcess();

    void use_DefRf();
    void use_DefRf0();
    void hide_Paths();

    void updateThumbsDirectoryName();
    void updateMosaicDirectoryName();
    void updateRelativeDirectoryName();
    void updateStitchedDirectoryName();
    void updateBatchListFileName();
    void updateImageDirectoryName();
    void updatePreprocessedDirectoryName();
    void updateParametersFileName();
    void updatePixelFeaturesDirectoryName();
    void updateProbMapDirectoryName();
    void updateSegmentationDirectoryName();
    void updateBoundaryFeaturesDirectoryName();
    void updatePredictionDirectoryName();
    void updatePlotsDirectoryName();
    void updateProfilesDirectoryName();
    void updateScreenshotsDirectoryName();
    void updateScreenshotsDirectoryName2();
    void updateAnalyzeListFileName();
    void setSuffix();

    bool isProfileChanged();
    void updateProfileLabel();
    void changeActiveProfile();
    void refreshSelectedImages(int index);

    void createNewProfile();
    void deleteProfile();
    void updateAvailableProfiles();
    void changeProfileName();

    void write(QByteArray text, bool clear);
    void checkInstallation();

private:
    QProcess process;

    QString parametersFileName;
    QString thumbsDirectoryName;

    QString mosaicDirectoryName;
    QString relativeDirectoryName;
    QString stitchedDirectoryName;
    QString batchListFileName;

    QString imageDirectoryName;
    QString preprocessedDirectoryName;
    QStringList selectedImages;

    QString pixelFeaturesDirectoryName;
    QString probMapDirectoryName;
    QString segmentationDirectoryName;
    QString boundaryFeaturesDirectoryName;
    QStringList selectedImages1;

    QString predictionDirectoryName;
    QString plotsDirectoryName;
    QString profilesDirectoryName;
    QString suffix;
    QStringList selectedImages2;

    QString screenshotsDirectoryName;
    QString selectedImage3;

    QString screenshotsDirectoryName2;
    QString analyzeListFileName;

    bool stitchingRunning;
    bool bubblePreprocessingRunning;
    bool correctSegmentationRunning;
    bool correctSegmentationRunning2;
    int  testNr;
    bool processRunning;
    bool noNewLine;

    bool useDefRf;
    bool useDefRf0;
    bool hidePaths;

    void disableAll();
    void checkPathesSet();

    void errorPreprocessed();
    void errorProbMap();
    void errorSegmentation();
    void errorPrediction();
    void errorProfiles();    
};

#endif
