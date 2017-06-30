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
#include <QtGui>
#include <iostream>
#include <fstream>
#include "processdialog.h"
#include <dirent.h>

#include "globals.h"

#include "profile.h"

ProcessDialog::ProcessDialog(QWidget *parent): QDialog(parent)
{       
    setupUi(this);

    createNewProfileButton->setIcon(QIcon(QApplication::applicationDirPath() + "/ico_addProfile.png"));
    createNewProfileButton->setIconSize(QSize(16,16));
    deleteProfileButton->setIcon(QIcon(QApplication::applicationDirPath() + "/ico_removeProfile.png"));
    deleteProfileButton->setIconSize(QSize(16,16));
    editProfileNameButton->setIcon(QIcon(QApplication::applicationDirPath() + "/ico_editProfile.png"));
    editProfileNameButton->setIconSize(QSize(16,16));

    disableAll();
    suffixCheckBox->setChecked(false);
    thumbsCheckBox->setChecked(false);
    defRfCheckBox ->setChecked(true);
    defRfCheckBox0->setChecked(true);
    radioButton_LASM->setChecked(true);
    processRunning=false;
    useDefRf = true;
    useDefRf0 = true;
    hidePaths = false;

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(refreshSelectedImages(int)));

    connect(killProcessButton, SIGNAL(clicked()), this, SLOT(killProcess()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(quit()));
    connect(labelButton, SIGNAL(clicked()), this, SLOT(labeling()));
    connect(plotsButton, SIGNAL(clicked()), this, SLOT(plots()));    

    connect(defRfCheckBox, SIGNAL(clicked()), this, SLOT(use_DefRf()));
    connect(defRfCheckBox0, SIGNAL(clicked()), this, SLOT(use_DefRf0()));
    connect(hideSelectedImagesPathsCheckBox, SIGNAL(clicked()), this, SLOT(hide_Paths()));
    connect(radioButton_LASM, SIGNAL(clicked()), this, SLOT(checkPathesSet()));
    connect(radioButton_Binary, SIGNAL(clicked()), this, SLOT(checkPathesSet()));
    connect(radioButton_FA, SIGNAL(clicked()), this, SLOT(checkPathesSet()));

    connect(thumbsCheckBox, SIGNAL(clicked()), this, SLOT(updateThumbsDirectoryName()));
    connect(suffixCheckBox, SIGNAL(clicked()), this, SLOT(setSuffix()));
    connect(stitchStartButton, SIGNAL(clicked()), this, SLOT(stitchImages()));
    connect(preprocessStartButton, SIGNAL(clicked()), this, SLOT(preprocessImage()));
    connect(preprocessStartButton2, SIGNAL(clicked()), this, SLOT(preprocessImage2()));
    connect(thumbsStartButton, SIGNAL(clicked()), this, SLOT(createThumbs()));
    connect(thumbsStartButton2, SIGNAL(clicked()), this, SLOT(reduceThumbs()));
    connect(cisStartButton, SIGNAL(clicked()), this, SLOT(segmentImage()));
    connect(loadProbButton, SIGNAL(clicked()), this, SLOT(loadProb()));
    connect(loadSegmBotton, SIGNAL(clicked()), this, SLOT(loadSegm()));
    connect(correctNetworkButton, SIGNAL(clicked()), this, SLOT(correctNetwork()));
    connect(extractNetworkButton, SIGNAL(clicked()), this, SLOT(extractNetwork()));
    connect(getParametersButton, SIGNAL(clicked()), this, SLOT(getParameters()));
    connect(depthProfilesButton, SIGNAL(clicked()), this, SLOT(depthProfiles()));
    connect(viewGrainsButton, SIGNAL(clicked()), this, SLOT(viewGrains()));
    connect(viewBoundariesButton, SIGNAL(clicked()), this, SLOT(viewBoundaries()));
    connect(viewJunctionsButton, SIGNAL(clicked()), this, SLOT(viewJunctions()));
    connect(junctionAnalysisButton, SIGNAL(clicked()), this, SLOT(analyzeJunctions()));
    connect(grainOverviewButton, SIGNAL(clicked()), this, SLOT(grainOverview()));
    connect(boundaryOverviewButton, SIGNAL(clicked()), this, SLOT(boundaryOverview()));
    connect(localCurvatureButton, SIGNAL(clicked()), this, SLOT(localCurvature()));
    connect(paramPlotButton, SIGNAL(clicked()), this, SLOT(plotParam()));
    connect(paramListButton, SIGNAL(clicked()), this, SLOT(listParam()));
    connect(corrPlotButton, SIGNAL(clicked()), this, SLOT(plotCorrelation()));
    connect(analyzeStartButton, SIGNAL(clicked()), this, SLOT(analyze()));
    connect(analyzeStartButton_2, SIGNAL(clicked()), this, SLOT(newProfiles()));
    connect(subgrainButton, SIGNAL(clicked()), this, SLOT(findSubgrains()));
    connect(corrSegmBotton, SIGNAL(clicked()), this, SLOT(corrSegm()));
    connect(loadSegmHDF5Button, SIGNAL(clicked()), this, SLOT(loadSegmHDF5()));

    connect(thumbsEdit, SIGNAL(textChanged(QString)), this, SLOT(updateThumbsDirectoryName()));
    connect(mosaicEdit, SIGNAL(textChanged(QString)), this, SLOT(updateMosaicDirectoryName()));
    connect(relativeEdit, SIGNAL(textChanged(QString)), this, SLOT(updateRelativeDirectoryName()));
    connect(stitchedEdit, SIGNAL(textChanged(QString)), this, SLOT(updateStitchedDirectoryName()));
    connect(batchListEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBatchListFileName()));
    connect(originalEdit, SIGNAL(textChanged(QString)), this, SLOT(updateImageDirectoryName()));
    connect(preprocessedEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePreprocessedDirectoryName()));
    connect(parametersEdit, SIGNAL(textChanged(QString)), this, SLOT(updateParametersFileName()));
    connect(pixelFeaturesEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePixelFeaturesDirectoryName()));
    connect(probMapEdit, SIGNAL(textChanged(QString)), this, SLOT(updateProbMapDirectoryName()));
    connect(segmentationEdit, SIGNAL(textChanged(QString)), this, SLOT(updateSegmentationDirectoryName()));
    connect(boundaryFeaturesEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBoundaryFeaturesDirectoryName()));
    connect(predictionEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePredictionDirectoryName()));
    connect(plotsEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePlotsDirectoryName()));
    connect(profilesEdit, SIGNAL(textChanged(QString)), this, SLOT(updateProfilesDirectoryName()));
    connect(screenshotsEdit, SIGNAL(textChanged(QString)), this, SLOT(updateScreenshotsDirectoryName()));
    connect(screenshotsEdit2, SIGNAL(textChanged(QString)), this, SLOT(updateScreenshotsDirectoryName2()));
    connect(analyzeListEdit, SIGNAL(textChanged(QString)), this, SLOT(updateAnalyzeListFileName()));

    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessStdout()));
    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(readProcessError()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

    connect(createNewProfileButton, SIGNAL(clicked()), this, SLOT(createNewProfile()));
    connect(deleteProfileButton, SIGNAL(clicked()), this, SLOT(deleteProfile()));
    connect(editProfileNameButton, SIGNAL(clicked()), this, SLOT(changeProfileName()));

    connect(elleDataSetEdit, SIGNAL(textChanged(QString)), this, SLOT(updateElleDataSetFileName()));
    connect(elleExportEdit, SIGNAL(textChanged(QString)), this, SLOT(updateElleExportDirectoryName()));
    connect(elleExportButton, SIGNAL(clicked()), this, SLOT(elleExportImage()));


    //Load last settings
    std::fstream profiles_file;
    std::string pathSettings = (QDir::homePath()).toAscii().data();
    pathSettings.append("/.processdialog.profiles");
    profiles_file.open(pathSettings.c_str());

    std::vector<QString> temp_data;
    Profile::numberOfProfiles = 0;
    bool found_active_profile = false;
    char c = '\n';
    if(profiles_file != NULL)//profiles file exists
    {
        std::string line;
        std::string active_profile;        

        std::vector<int> line_pos;           //Beginning line of the profiles
        std::vector<std::string> strProfiles;//Names of the profiles
        std::string emptystring = "";

        //Load data from profiles file
        while(std::getline(profiles_file, line, c))
        {
            if(line.empty()) temp_data.push_back(QString::fromStdString(emptystring));
            else temp_data.push_back(QString::fromStdString(line));
        }

        //Scan for profiles
        for(unsigned int i = 0; i < temp_data.size(); i++)
        {
            std::string data_i = temp_data[i].toStdString();
            if(data_i.compare(0,1,"[") == 0)//active profile
            {
                if(found_active_profile)
                {
                    //more than one active profile found. Using the first found profile as active profile
                    std::string data_i_tmp = data_i;
                    data_i_tmp.resize(data_i_tmp.size()-1);
                    data_i_tmp = data_i_tmp.substr(1,data_i_tmp.size());
                    Profile tmpProfile(QString::fromStdString(data_i_tmp));
                    tmpProfile.index = Profile::numberOfProfiles;

                    profiles.push_back(tmpProfile);
                    line_pos.push_back(i);
                    Profile::numberOfProfiles++;
                }
                else
                {
                    active_profile = data_i;
                    active_profile.resize(active_profile.size()-1);
                    active_profile = active_profile.substr(1,active_profile.size());
                    Profile tmpProfile(QString::fromStdString(active_profile));
                    tmpProfile.index = Profile::numberOfProfiles;

                    profiles.push_back(tmpProfile);
                    line_pos.push_back(i);
                    found_active_profile = true;
                    Profile::numberOfProfiles++;
                }
            }
            else if(data_i.compare(0,1,"]") == 0)//inactive profile
            {
                std::string data_i_tmp = data_i;
                data_i_tmp.resize(data_i_tmp.size()-1);
                data_i_tmp = data_i_tmp.substr(1,data_i_tmp.size());
                Profile tmpProfile(QString::fromStdString(data_i_tmp));
                tmpProfile.index = Profile::numberOfProfiles;
                profiles.push_back(tmpProfile);

                line_pos.push_back(i);
                Profile::numberOfProfiles++;
            }
        }
        //if no active profile is found, no action is required as index is set to 0 by default

        //set up profile data structures
        for(unsigned int j = 0; j < profiles.size(); j++)
        {
            int line_next = 0;
            if(j+1 < profiles.size()) line_next = line_pos[j+1];
            else if(j+1 == profiles.size()) line_next = temp_data.size();
            std::string profileName = profiles[j].name.toStdString();
            std::vector<QString> profileData;

            if(profileName.compare(active_profile) == 0)
            {
                Profile::activeProfileIndex = j;
            }

            for(int k = line_pos[j]; k < line_next; k++)
            {                
                profileData.push_back(temp_data[k]);
            }
            profiles[j].data = profileData;
            profileData.clear();
        }        

        //resize profile data structure if necessary
        for(unsigned int m = 0; m < profiles.size(); m++)
        {
            if(profiles[m].data.size() < Profile::numberOfSettings)
            {
                for(int n = 0; n < Profile::numberOfSettings - profiles[m].data.size(); n++)
                {
                    profiles[m].data.push_back("");
                }
            }
        }
                
        //Load settings belonging to the active profile into the processdialog
        loadSettings();
    }
    else //If there exists no profile file
    {
        //create an empty "Default" profile
        Profile::activeProfileIndex = 0;
        Profile p("Default");
        std::vector<QString> empty;
        empty.push_back("[Default]");
        for(int i = 1; i < Profile::numberOfSettings; i++)
        {
            empty.push_back("");
        }
        p.setData(empty);
        p.index = 0;
        profiles.push_back(p);

        //Variables for the legacy settings message dialog
        int ret = 0;
        bool choice = false;

        FILE *processdialog_leg;
        std::string pathSettings = (QDir::homePath()).toAscii().data();
        pathSettings.append("/.processdialog");
        processdialog_leg = fopen(pathSettings.c_str(), "r");

        FILE *labeldialog_leg;
        pathSettings = (QDir::homePath()).toAscii().data();
        pathSettings.append("/.labeldialog");
        labeldialog_leg = fopen(pathSettings.c_str(), "r");

        FILE *plotsdialog_leg;
        pathSettings = (QDir::homePath()).toAscii().data();
        pathSettings.append("/.plotsdialog");
        plotsdialog_leg = fopen(pathSettings.c_str(), "r");

        if(processdialog_leg != NULL && labeldialog_leg != NULL && plotsdialog_leg != NULL)
        {
            QMessageBox loadLegacyMsgBox;
            loadLegacyMsgBox.setIcon(QMessageBox::Question);
            loadLegacyMsgBox.setWindowTitle("No profiles file found");
            loadLegacyMsgBox.setText("Could not find a profiles file. However, all three legacy settings files were found.");
            loadLegacyMsgBox.setInformativeText("Load program settings from these files? If you choose 'No', an empty profile will be created.\nNote: newer settings will be loaded with empty or default values if they do not exist in the legacy files.");
            loadLegacyMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            loadLegacyMsgBox.setDefaultButton(QMessageBox::Yes);

            ret = loadLegacyMsgBox.exec();
            switch(ret)
            {
                case QMessageBox::Yes:
                {
                    choice = true;
                    break;
                }
                case QMessageBox::No:
                {
                    choice = false;
                    break;
                }
            }

            loadLegacyMsgBox.close();

            if(choice)
            {
                //Load legacy settings
                loadSettingsLegacy();
                plotsDialog.loadSettingsLegacy();
                labelDialog.loadSettingsLegacy();

            }
            else
            {
                loadSettingsEmpty();
            }
        }
        else //No profiles file found and none or not all legacy settings files found
        {
            loadSettingsEmpty();
        }
    }

    //Load settings into member classes
    labelDialog.loadSettings();
    plotsDialog.loadSettings();

    QString item;
    for(unsigned int i = 0; i < profiles.size(); i++)
    {
        item = profiles[i].name;
        availableProfilesComboBox->addItem(item);
    }
    availableProfilesComboBox->setCurrentIndex(Profile::activeProfileIndex);

    connect(availableProfilesComboBox, SIGNAL(activated(int)/*currentIndexChanged(int)*/), this, SLOT(changeActiveProfile()));  
    updateProfileLabel();
}

/*******************************
Helper function to load settings
*******************************/

void ProcessDialog::loadSettings()
{
    std::string emptystring = "";
    mosaicDirectoryName=profiles[Profile::activeProfileIndex].data[2];
    mosaicEdit->setText(mosaicDirectoryName);
    relativeDirectoryName=profiles[Profile::activeProfileIndex].data[3];
    relativeEdit->setText(relativeDirectoryName);
    stitchedDirectoryName=profiles[Profile::activeProfileIndex].data[4];
    stitchedEdit->setText(stitchedDirectoryName);
    batchListFileName=profiles[Profile::activeProfileIndex].data[5];
    batchListEdit->setText(batchListFileName);
    imageDirectoryName=profiles[Profile::activeProfileIndex].data[6];
    originalEdit->setText(imageDirectoryName);
    preprocessedDirectoryName=profiles[Profile::activeProfileIndex].data[7];
    preprocessedEdit->setText(preprocessedDirectoryName);
    parametersFileName=profiles[Profile::activeProfileIndex].data[8];
    parametersEdit->setText(parametersFileName);
    pixelFeaturesDirectoryName=profiles[Profile::activeProfileIndex].data[9];
    pixelFeaturesEdit->setText(pixelFeaturesDirectoryName);
    probMapDirectoryName=profiles[Profile::activeProfileIndex].data[10];
    probMapEdit->setText(probMapDirectoryName);
    segmentationDirectoryName=profiles[Profile::activeProfileIndex].data[11];
    segmentationEdit->setText(segmentationDirectoryName);
    boundaryFeaturesDirectoryName=profiles[Profile::activeProfileIndex].data[12];
    boundaryFeaturesEdit->setText(boundaryFeaturesDirectoryName);
    if(profiles[Profile::activeProfileIndex].data[14].toStdString().compare(emptystring) != 0)
    {
        grainStepBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[14].toLocal8Bit().constData()));
    }
    else
    {
        grainStepBox->setValue(50);
    }
    if(profiles[Profile::activeProfileIndex].data[15].toStdString().compare(emptystring) != 0)
    {
        minBubbleDistanceBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[15].toLocal8Bit().constData()));
    }
    else
    {
        minBubbleDistanceBox->setValue(0);
    }
    if(profiles[Profile::activeProfileIndex].data[16].toStdString().compare(emptystring) != 0)
    {
        lowGrainSizeBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[16].toLocal8Bit().constData()));
    }
    else
    {
        lowGrainSizeBox->setValue(0);
    }
    if(profiles[Profile::activeProfileIndex].data[17].toStdString().compare(emptystring) != 0)
    {
        highGrainSizeBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[17].toLocal8Bit().constData()));
    }
    else
    {
        highGrainSizeBox->setValue(0);
    }
    if(profiles[Profile::activeProfileIndex].data[18].toStdString().compare(emptystring) != 0)
    {
        grainSizeStepBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[18].toLocal8Bit().constData()));
    }
    else
    {
        grainSizeStepBox->setValue(1000);
    }
    predictionDirectoryName=profiles[Profile::activeProfileIndex].data[19];
    predictionEdit->setText(predictionDirectoryName);
    plotsDirectoryName=profiles[Profile::activeProfileIndex].data[20];
    plotsEdit->setText(plotsDirectoryName);
    profilesDirectoryName=profiles[Profile::activeProfileIndex].data[21];
    profilesEdit->setText(profilesDirectoryName);
    if(profiles[Profile::activeProfileIndex].data[22].toStdString().compare(emptystring) != 0)
    {
        if (atoi(profiles[Profile::activeProfileIndex].data[22].toLocal8Bit().constData())==1) suffixCheckBox->setChecked(true);
        else
        {
            suffix="no";
            suffixCheckBox->setChecked(false);
        }
    }
    setSuffix();
    if(profiles[Profile::activeProfileIndex].data[23].toStdString().compare(emptystring) != 0) nrDepthsBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[23].toLocal8Bit().constData()));
    screenshotsDirectoryName=profiles[Profile::activeProfileIndex].data[24];
    screenshotsEdit->setText(screenshotsDirectoryName);
    QString startValue=profiles[Profile::activeProfileIndex].data[25];
    startValueEdit->setText(startValue);
    analyzeListFileName=profiles[Profile::activeProfileIndex].data[26];
    analyzeListEdit->setText(analyzeListFileName);
    screenshotsDirectoryName2=profiles[Profile::activeProfileIndex].data[27];
    screenshotsEdit2->setText(screenshotsDirectoryName2);
    if(profiles[Profile::activeProfileIndex].data[28].toStdString().compare(emptystring) != 0) paramComboBox1->setCurrentIndex(atoi(profiles[Profile::activeProfileIndex].data[28].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[29].toStdString().compare(emptystring) != 0) paramSpinBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[29].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[30].toStdString().compare(emptystring) != 0) paramComboBox2->setCurrentIndex(atoi(profiles[Profile::activeProfileIndex].data[30].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[31].toStdString().compare(emptystring) != 0) corrComboBox1->setCurrentIndex(atoi(profiles[Profile::activeProfileIndex].data[31].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[32].toStdString().compare(emptystring) != 0) corrSpinBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[32].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[33].toStdString().compare(emptystring) != 0) corrComboBox2->setCurrentIndex(atoi(profiles[Profile::activeProfileIndex].data[33].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[34].toStdString().compare(emptystring) != 0) depthBinWidthBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[34].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[35].toStdString().compare(emptystring) != 0)
    {
        if (atoi(profiles[Profile::activeProfileIndex].data[35].toLocal8Bit().constData())==1) thumbsCheckBox->setChecked(true);
        else thumbsCheckBox->setChecked(false);
    }
    thumbsDirectoryName=profiles[Profile::activeProfileIndex].data[36];
    thumbsEdit->setText(thumbsDirectoryName); 
    if(profiles[Profile::activeProfileIndex].data[75].toStdString().compare(emptystring) != 0)
    {
        if (atoi(profiles[Profile::activeProfileIndex].data[75].toLocal8Bit().constData())==0 ) defRfCheckBox->setChecked(false);
        else defRfCheckBox->setChecked(true);
        use_DefRf();
    }
    if(profiles[Profile::activeProfileIndex].data[76].toStdString().compare(emptystring) != 0)
    {
        if (atoi(profiles[Profile::activeProfileIndex].data[76].toLocal8Bit().constData())==0 ) defRfCheckBox0->setChecked(false);
        else defRfCheckBox0->setChecked(true);
        use_DefRf0();
    }

    checkPathesSet();
    saveSettings();
    Profile::unmodifiedProfile = profiles[Profile::activeProfileIndex];
}

void ProcessDialog::loadSettingsLegacy()
{
        std::string pathSettings = (QDir::homePath()).toAscii().data();
        pathSettings.append("/.processdialog");
        std::ifstream settings_file(pathSettings.c_str());
        std::ifstream temp_settings_file(pathSettings.c_str());
        std::string teststring;
        temp_settings_file>>teststring;

        if(settings_file && teststring.size()!=0)
        {
            std::string str;
            settings_file>>str;
            QString setting = QString::fromStdString(str);
            QStringList settings = setting.split(",");

            mosaicDirectoryName=settings.at(0).toLocal8Bit().constData();
            mosaicEdit->setText(mosaicDirectoryName);
            profiles[Profile::activeProfileIndex].data[2] = mosaicDirectoryName;
            relativeDirectoryName=settings.at(1).toLocal8Bit().constData();
            relativeEdit->setText(relativeDirectoryName);
            profiles[Profile::activeProfileIndex].data[3] = relativeDirectoryName;
            stitchedDirectoryName=settings.at(2).toLocal8Bit().constData();
            stitchedEdit->setText(stitchedDirectoryName);
            profiles[Profile::activeProfileIndex].data[4] = stitchedDirectoryName;
            batchListFileName=settings.at(3).toLocal8Bit().constData();
            batchListEdit->setText(batchListFileName);
            profiles[Profile::activeProfileIndex].data[5] = batchListFileName;
            imageDirectoryName=settings.at(4).toLocal8Bit().constData();
            originalEdit->setText(imageDirectoryName);
            profiles[Profile::activeProfileIndex].data[6] = imageDirectoryName;
            preprocessedDirectoryName=settings.at(5).toLocal8Bit().constData();
            preprocessedEdit->setText(preprocessedDirectoryName);
            profiles[Profile::activeProfileIndex].data[7] = preprocessedDirectoryName;
            parametersFileName=settings.at(6).toLocal8Bit().constData();
            parametersEdit->setText(parametersFileName);
            profiles[Profile::activeProfileIndex].data[8] = parametersFileName;
            pixelFeaturesDirectoryName=settings.at(7).toLocal8Bit().constData();
            pixelFeaturesEdit->setText(pixelFeaturesDirectoryName);
            probMapDirectoryName=settings.at(8).toLocal8Bit().constData();
            probMapEdit->setText(probMapDirectoryName);
            segmentationDirectoryName=settings.at(9).toLocal8Bit().constData();
            segmentationEdit->setText(segmentationDirectoryName);
            boundaryFeaturesDirectoryName=settings.at(10).toLocal8Bit().constData();
            boundaryFeaturesEdit->setText(boundaryFeaturesDirectoryName);
            grainStepBox->setValue(atoi(settings.at(12).toLocal8Bit().constData()));
            minBubbleDistanceBox->setValue(atoi(settings.at(13).toLocal8Bit().constData()));
            lowGrainSizeBox->setValue(atoi(settings.at(14).toLocal8Bit().constData()));
            highGrainSizeBox->setValue(atoi(settings.at(15).toLocal8Bit().constData()));
            grainSizeStepBox->setValue(atoi(settings.at(16).toLocal8Bit().constData()));
            predictionDirectoryName=settings.at(17).toLocal8Bit().constData();
            predictionEdit->setText(predictionDirectoryName);
            plotsDirectoryName=settings.at(18).toLocal8Bit().constData();
            plotsEdit->setText(plotsDirectoryName);
            profilesDirectoryName=settings.at(19).toLocal8Bit().constData();
            profilesEdit->setText(profilesDirectoryName);
            if (atoi(settings.at(20).toLocal8Bit().constData())==1) suffixCheckBox->setChecked(true);
            else suffix="no";
            setSuffix();
            nrDepthsBox->setValue(atoi(settings.at(21).toLocal8Bit().constData()));
            screenshotsDirectoryName=settings.at(22).toLocal8Bit().constData();
            screenshotsEdit->setText(screenshotsDirectoryName);
            QString startValue=settings.at(23).toLocal8Bit().constData();
            startValueEdit->setText(startValue);
            if (settings.size()>24)
            {
                analyzeListFileName=settings.at(24).toLocal8Bit().constData();
                analyzeListEdit->setText(analyzeListFileName);
            }
            if (settings.size()>25)
            {
                screenshotsDirectoryName2=settings.at(25).toLocal8Bit().constData();
                screenshotsEdit2->setText(screenshotsDirectoryName2);
            }
            if (settings.size()>26) paramComboBox1->setCurrentIndex(atoi(settings.at(26).toLocal8Bit().constData()));
            if (settings.size()>27) paramSpinBox->setValue(atoi(settings.at(27).toLocal8Bit().constData()));
            if (settings.size()>28) paramComboBox2->setCurrentIndex(atoi(settings.at(28).toLocal8Bit().constData()));
            if (settings.size()>29) corrComboBox1->setCurrentIndex(atoi(settings.at(29).toLocal8Bit().constData()));
            if (settings.size()>30) corrSpinBox->setValue(atoi(settings.at(30).toLocal8Bit().constData()));
            if (settings.size()>31) corrComboBox2->setCurrentIndex(atoi(settings.at(31).toLocal8Bit().constData()));
            if (settings.size()>32) depthBinWidthBox->setValue(atoi(settings.at(32).toLocal8Bit().constData()));
            if (settings.size()>33) if (atoi(settings.at(33).toLocal8Bit().constData())==1) thumbsCheckBox->setChecked(true);
            if (settings.size()>34)
            {
                thumbsDirectoryName=settings.at(34).toLocal8Bit().constData();
                thumbsEdit->setText(thumbsDirectoryName);
            }
            temp_settings_file.close();
            settings_file.close();

            //Set up data structures
            profiles[Profile::activeProfileIndex].data[0] = "[Default]";
            profiles[Profile::activeProfileIndex].data[2] = mosaicDirectoryName;
            profiles[Profile::activeProfileIndex].data[3] = relativeDirectoryName;
            profiles[Profile::activeProfileIndex].data[4] = stitchedDirectoryName;
            profiles[Profile::activeProfileIndex].data[5] = batchListFileName;
            profiles[Profile::activeProfileIndex].data[6] = imageDirectoryName;
            profiles[Profile::activeProfileIndex].data[7] = preprocessedDirectoryName;
            profiles[Profile::activeProfileIndex].data[8] = parametersFileName;
            profiles[Profile::activeProfileIndex].data[9] = pixelFeaturesDirectoryName;
            profiles[Profile::activeProfileIndex].data[10] = probMapDirectoryName;
            profiles[Profile::activeProfileIndex].data[11] = segmentationDirectoryName;
            profiles[Profile::activeProfileIndex].data[12] = boundaryFeaturesDirectoryName;
            profiles[Profile::activeProfileIndex].data[13] = "0";
            profiles[Profile::activeProfileIndex].data[14] = QString::fromStdString(Profile::convertInt(grainStepBox->value()));
            profiles[Profile::activeProfileIndex].data[15] = QString::fromStdString(Profile::convertInt(minBubbleDistanceBox->value()));
            profiles[Profile::activeProfileIndex].data[16] = QString::fromStdString(Profile::convertInt(lowGrainSizeBox->value()));
            profiles[Profile::activeProfileIndex].data[17] = QString::fromStdString(Profile::convertInt(highGrainSizeBox->value()));
            profiles[Profile::activeProfileIndex].data[18] = QString::fromStdString(Profile::convertInt(grainSizeStepBox->value()));
            profiles[Profile::activeProfileIndex].data[19] = predictionDirectoryName;
            profiles[Profile::activeProfileIndex].data[20] = plotsDirectoryName;
            profiles[Profile::activeProfileIndex].data[21] = profilesDirectoryName;
            if(suffixCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[22] = "1";
            else profiles[Profile::activeProfileIndex].data[22] = "0";
            profiles[Profile::activeProfileIndex].data[23] = QString::fromStdString(Profile::convertInt(nrDepthsBox->value()));
            profiles[Profile::activeProfileIndex].data[24] = screenshotsDirectoryName;
            profiles[Profile::activeProfileIndex].data[25] = startValue;
            profiles[Profile::activeProfileIndex].data[26] = analyzeListFileName;
            profiles[Profile::activeProfileIndex].data[27] = screenshotsDirectoryName2;
            profiles[Profile::activeProfileIndex].data[28] = QString::fromStdString(Profile::convertInt(paramComboBox1->currentIndex()));
            profiles[Profile::activeProfileIndex].data[29] = QString::fromStdString(Profile::convertInt(paramSpinBox->value()));
            profiles[Profile::activeProfileIndex].data[30] = QString::fromStdString(Profile::convertInt(paramComboBox2->currentIndex()));
            profiles[Profile::activeProfileIndex].data[31] = QString::fromStdString(Profile::convertInt(corrComboBox1->currentIndex()));
            profiles[Profile::activeProfileIndex].data[32] = QString::fromStdString(Profile::convertInt(corrSpinBox->value()));
            profiles[Profile::activeProfileIndex].data[33] = QString::fromStdString(Profile::convertInt(corrComboBox2->currentIndex()));
            profiles[Profile::activeProfileIndex].data[34] = QString::fromStdString(Profile::convertInt(depthBinWidthBox->value()));
            if(thumbsCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[35] = "1";
            else profiles[Profile::activeProfileIndex].data[35] = "0";
            profiles[Profile::activeProfileIndex].data[36] = thumbsDirectoryName;

            checkPathesSet();
            Profile::unmodifiedProfile = profiles[Profile::activeProfileIndex];
        }
}

void ProcessDialog::loadSettingsEmpty()
{
    //Create profile file with Default profile
    labelDialog.saveSettings();
    plotsDialog.saveSettings();
    saveSettings();

    //Load settings
    loadSettings();
}

/*********************
Quit and save settings
*********************/

void ProcessDialog::quit()
{
    saveSettings();
    bool changed;
    changed = isProfileChanged();
    int choice = 0;
    if(changed)
    {
        QMessageBox profileChangedMsgBox;
        profileChangedMsgBox.setIcon(QMessageBox::Question);
        profileChangedMsgBox.setWindowTitle("Current profile has been edited");
        profileChangedMsgBox.setText("The current profile has been edited. Do you want to save the changes before exiting?");
        profileChangedMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        profileChangedMsgBox.setDefaultButton(QMessageBox::Yes);

        int ret = profileChangedMsgBox.exec();
        switch(ret)
        {
            case QMessageBox::Yes:
            {
                choice = 1;
                break;
            }
            case QMessageBox::No:
            {
                choice = 2;
                break;
            }
            case QMessageBox::Cancel:
            {
                choice = 3;
                break;
            }
        }

        profileChangedMsgBox.close();

        if(choice == 1)
        {
            reject();
        }
        else if(choice == 2)
        {
            profiles[Profile::activeProfileIndex] = Profile::unmodifiedProfile;
            saveSettings();
            reject();
        }
    }
    else
    {
        reject();
    }
}

//write setttings to file
void ProcessDialog::saveSettings()
{
    //Load profiles file
    std::string pathSettings = (QDir::homePath()).toAscii().data();
    pathSettings.append("/.processdialog.profiles");
    std::ofstream profiles_file(pathSettings.c_str());    

    profiles[Profile::activeProfileIndex].data[13] = "0";
    profiles[Profile::activeProfileIndex].data[14] = QString::fromStdString(Profile::convertInt(grainStepBox->value()));
    profiles[Profile::activeProfileIndex].data[15] = QString::fromStdString(Profile::convertInt(minBubbleDistanceBox->value()));
    profiles[Profile::activeProfileIndex].data[16] = QString::fromStdString(Profile::convertInt(lowGrainSizeBox->value()));
    profiles[Profile::activeProfileIndex].data[17] = QString::fromStdString(Profile::convertInt(highGrainSizeBox->value()));
    profiles[Profile::activeProfileIndex].data[18] = QString::fromStdString(Profile::convertInt(grainSizeStepBox->value()));
    if (!suffixCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[22] = "0";
    else profiles[Profile::activeProfileIndex].data[22] = "1";
    profiles[Profile::activeProfileIndex].data[23] = QString::fromStdString(Profile::convertInt(nrDepthsBox->value()));
    profiles[Profile::activeProfileIndex].data[28] = QString::fromStdString(Profile::convertInt(paramComboBox1->currentIndex()));
    profiles[Profile::activeProfileIndex].data[29] = QString::fromStdString(Profile::convertInt(paramSpinBox->value()));
    profiles[Profile::activeProfileIndex].data[30] = QString::fromStdString(Profile::convertInt(paramComboBox2->currentIndex()));
    profiles[Profile::activeProfileIndex].data[31] = QString::fromStdString(Profile::convertInt(corrComboBox1->currentIndex()));
    profiles[Profile::activeProfileIndex].data[32] = QString::fromStdString(Profile::convertInt(corrSpinBox->value()));
    profiles[Profile::activeProfileIndex].data[33] = QString::fromStdString(Profile::convertInt(corrComboBox2->currentIndex()));
    profiles[Profile::activeProfileIndex].data[34] = QString::fromStdString(Profile::convertInt(depthBinWidthBox->value()));
    if (!thumbsCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[35] = "0";
    else profiles[Profile::activeProfileIndex].data[35] = "1";

    //Resize profile if necessary
    for(int a = 0; a < (int)profiles.size(); a++)
    {
        if(profiles[a].data.size() < Profile::numberOfSettings)
        {
            for(int n = 0; n < Profile::numberOfSettings - profiles[a].data.size(); n++)
            {
                profiles[a].data.push_back("");
            }
        }
    }

    for(int i = 0; i < (int)profiles.size(); i++)
    {
        std::string profileName_i = profiles[i].name.toStdString();
        if(i == Profile::activeProfileIndex)
        {
            profileName_i.append("]");
            profileName_i.insert(0,"[");
        }
        else
        {
            profileName_i.append("[");
            profileName_i.insert(0,"]");
        }
        profiles_file << profileName_i.c_str() << "\n";
        for(unsigned int j = 1; j < profiles[i].data.size(); j++)
        {            
            //Write tags
            if(j == 1)
            {
                profiles_file << "#processdialog#" << "\n";
                continue;

            }
            if(j == 37)
            {
                profiles_file << "#labeldialog#" << "\n";
                continue;
            }
            else if(j == 54)
            {
                profiles_file << "#plotsdialog#" << "\n";
                continue;
            }
            profiles_file << profiles[i].data[j].toAscii().data() << "\n";
        }
    }    

    profiles_file.close();    
}

/*********
Set pathes
*********/

void ProcessDialog::updateElleDataSetFileName()
{
    elleDataSetFileName = elleDataSetEdit->text();
    checkPathesSet();
}

void ProcessDialog::updateElleExportDirectoryName()
{
    elleExportDirectoryName = elleExportEdit->text();
    checkPathesSet();
}

void ProcessDialog::updateThumbsDirectoryName()
{
    thumbsDirectoryName = thumbsEdit->text();
    profiles[Profile::activeProfileIndex].data[36] = thumbsDirectoryName;
    checkPathesSet();
    if(!thumbsCheckBox->isChecked())
    {
        profiles[Profile::activeProfileIndex].data[35] = "0";
    }
    else
    {
        profiles[Profile::activeProfileIndex].data[35] = "1";
    }
}

void ProcessDialog::updateMosaicDirectoryName()
{
    mosaicDirectoryName = mosaicEdit->text();
    profiles[Profile::activeProfileIndex].data[2] = mosaicDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateRelativeDirectoryName()
{
    relativeDirectoryName = relativeEdit->text();
    profiles[Profile::activeProfileIndex].data[3] = relativeDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateStitchedDirectoryName()
{
    stitchedDirectoryName = stitchedEdit->text();
    profiles[Profile::activeProfileIndex].data[4] = stitchedDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateBatchListFileName()
{
    batchListFileName = batchListEdit->text();
    profiles[Profile::activeProfileIndex].data[5] = batchListFileName;
    checkPathesSet();
}

void ProcessDialog::updateImageDirectoryName()
{
    imageDirectoryName = originalEdit->text();
    profiles[Profile::activeProfileIndex].data[6] = imageDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updatePreprocessedDirectoryName()
{
    preprocessedDirectoryName = preprocessedEdit->text();
    profiles[Profile::activeProfileIndex].data[7] = preprocessedDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateParametersFileName()
{
    parametersFileName = parametersEdit->text();    
    profiles[Profile::activeProfileIndex].data[8] = parametersFileName;
    checkPathesSet();
}

void ProcessDialog::updatePixelFeaturesDirectoryName()
{
    pixelFeaturesDirectoryName = pixelFeaturesEdit->text();
    profiles[Profile::activeProfileIndex].data[9] = pixelFeaturesDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateProbMapDirectoryName()
{
    probMapDirectoryName = probMapEdit->text();
    profiles[Profile::activeProfileIndex].data[10] = probMapDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateSegmentationDirectoryName()
{
    segmentationDirectoryName = segmentationEdit->text();
    profiles[Profile::activeProfileIndex].data[11] = segmentationDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateBoundaryFeaturesDirectoryName()
{
    boundaryFeaturesDirectoryName = boundaryFeaturesEdit->text();
    profiles[Profile::activeProfileIndex].data[12] = boundaryFeaturesDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updatePredictionDirectoryName()
{
    predictionDirectoryName = predictionEdit->text();
    profiles[Profile::activeProfileIndex].data[19] = predictionDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updatePlotsDirectoryName()
{
    plotsDirectoryName = plotsEdit->text();
    profiles[Profile::activeProfileIndex].data[20] = plotsDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateProfilesDirectoryName()
{
    profilesDirectoryName = profilesEdit->text();
    profiles[Profile::activeProfileIndex].data[21] = profilesDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateScreenshotsDirectoryName()
{
    screenshotsDirectoryName = screenshotsEdit->text();
    profiles[Profile::activeProfileIndex].data[24] = screenshotsDirectoryName;
    checkPathesSet();
}

void ProcessDialog::updateAnalyzeListFileName()
{
    analyzeListFileName = analyzeListEdit->text();
    profiles[Profile::activeProfileIndex].data[26] = analyzeListFileName;
    checkPathesSet();
}

void ProcessDialog::updateScreenshotsDirectoryName2()
{
    screenshotsDirectoryName2 = screenshotsEdit2->text();
    profiles[Profile::activeProfileIndex].data[27] = screenshotsDirectoryName2;
    checkPathesSet();
}

void ProcessDialog::setSuffix()
{
    if (!suffixCheckBox->isChecked())
    {
        suffix="no";
        suffixLabel->setText("");
        suffixLabel_3->setText("");
        suffixLabel_4->setText("Suffix extraction: OFF");
    }
    else
    {
        QString text = "Current suffix:     ";
        QString getSuffix = originalEdit->text().toAscii().data();
        getSuffix.resize(getSuffix.size()-1);
        suffix=getFilename(getSuffix);
        suffix.append("/");
        text.append(suffix);
        suffixLabel->setText(text);
        suffixLabel_3->setText(text);
        suffixLabel_4->setText("Suffix extraction: ON");
    }
    checkPathesSet();
}

//General
void ProcessDialog::on_parametersBrowse_clicked()
{
    QString initialName = parametersEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    parametersFileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    parametersFileName = QDir::toNativeSeparators(parametersFileName);

    checkDirEmpty(initialName, parametersFileName, *parametersEdit);
}

void ProcessDialog::on_thumbsBrowse_clicked()
{
    QString initialName = thumbsEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    thumbsDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    thumbsDirectoryName = QDir::toNativeSeparators(thumbsDirectoryName);
    thumbsDirectoryName.append("/");

    checkDirEmpty(initialName, thumbsDirectoryName, *thumbsEdit);
}

//Elle
void ProcessDialog::on_elleDataSetBrowse_clicked()
{
    QString initialName = elleDataSetEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    elleDataSetFileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    elleDataSetFileName = QDir::toNativeSeparators(elleDataSetFileName);

    checkDirEmpty(initialName, elleDataSetFileName, *elleDataSetEdit);
}

void ProcessDialog::on_elleExportBrowse_clicked()
{
    QString initialName = elleExportEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    elleExportDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    elleExportDirectoryName = QDir::toNativeSeparators(elleExportDirectoryName);
    elleExportDirectoryName.append("/");

    checkDirEmpty(initialName, elleExportDirectoryName, *elleExportEdit);
}

//Stitching
void ProcessDialog::on_mosaicBrowse_clicked()
{
    QString initialName = mosaicEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    mosaicDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    mosaicDirectoryName = QDir::toNativeSeparators(mosaicDirectoryName);
    mosaicDirectoryName.append("/");

    checkDirEmpty(initialName, mosaicDirectoryName, *mosaicEdit);
}

void ProcessDialog::on_relativeBrowse_clicked()
{
    QString initialName = relativeEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    relativeDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    relativeDirectoryName = QDir::toNativeSeparators(relativeDirectoryName);
    relativeDirectoryName.append("/");

    checkDirEmpty(initialName, relativeDirectoryName, *relativeEdit);
}

void ProcessDialog::on_stitchedBrowse_clicked()
{
    QString initialName = stitchedEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    stitchedDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    stitchedDirectoryName = QDir::toNativeSeparators(stitchedDirectoryName);
    stitchedDirectoryName.append("/");

    checkDirEmpty(initialName, stitchedDirectoryName, *stitchedEdit);
}

void ProcessDialog::on_batchListBrowse_clicked()
{
    QString initialName = batchListEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    batchListFileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    batchListFileName = QDir::toNativeSeparators(batchListFileName);

    checkDirEmpty(initialName, batchListFileName, *batchListEdit);
}

//Preprocessing
void ProcessDialog::on_originalBrowse_clicked()
{
    QString initialName = originalEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    imageDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    imageDirectoryName = QDir::toNativeSeparators(imageDirectoryName);
    imageDirectoryName.append("/");

    checkDirEmpty(initialName, imageDirectoryName, *originalEdit);
    setSuffix();
}

void ProcessDialog::on_preprocessedBrowse_clicked()
{
    QString initialName = preprocessedEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    preprocessedDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    preprocessedDirectoryName = QDir::toNativeSeparators(preprocessedDirectoryName);
    preprocessedDirectoryName.append("/");

    checkDirEmpty(initialName, preprocessedDirectoryName, *preprocessedEdit);
}

void ProcessDialog::on_imageBrowse_clicked()
{
    QString initialName = imageDirectoryName;
    if (imageDirectoryName.isEmpty())
        imageDirectoryName = QDir::homePath();
    selectedImages = QFileDialog::getOpenFileNames(this, tr("Choose Files"), initialName);

    if(!selectedImages.isEmpty()) imageDirectoryName = getPath(selectedImages.at(0).toLocal8Bit().constData());
    originalEdit->setText(imageDirectoryName);

    QString text = QString("%1").arg(selectedImages.size());
    text.append(" image(s) selected");
    imagesSelectedLabel->setText(text);
    checkPathesSet();
    setSuffix();

    refreshSelectedImages(3);
}

//CIS
void ProcessDialog::on_pixelFeaturesBrowse_clicked()
{
    QString initialName = pixelFeaturesEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    pixelFeaturesDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    pixelFeaturesDirectoryName = QDir::toNativeSeparators(pixelFeaturesDirectoryName);
    pixelFeaturesDirectoryName.append("/");

    checkDirEmpty(initialName, pixelFeaturesDirectoryName, *pixelFeaturesEdit);
}

void ProcessDialog::on_probMapBrowse_clicked()
{
    QString initialName = probMapEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    probMapDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    probMapDirectoryName = QDir::toNativeSeparators(probMapDirectoryName);
    probMapDirectoryName.append("/");

    checkDirEmpty(initialName, probMapDirectoryName, *probMapEdit);
}

void ProcessDialog::on_segmentationBrowse_clicked()
{
    QString initialName = segmentationEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    segmentationDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    segmentationDirectoryName = QDir::toNativeSeparators(segmentationDirectoryName);
    segmentationDirectoryName.append("/");

    checkDirEmpty(initialName, segmentationDirectoryName, *segmentationEdit);
}

void ProcessDialog::on_boundaryFeaturesBrowse_clicked()
{
    QString initialName = boundaryFeaturesEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    boundaryFeaturesDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    boundaryFeaturesDirectoryName = QDir::toNativeSeparators(boundaryFeaturesDirectoryName);
    boundaryFeaturesDirectoryName.append("/");

    checkDirEmpty(initialName, boundaryFeaturesDirectoryName, *boundaryFeaturesEdit);
}

void ProcessDialog::on_imageBrowse1_clicked()
{
    QString initialName = imageDirectoryName;
    if (imageDirectoryName.isEmpty())
        imageDirectoryName = QDir::homePath();
    selectedImages1 = QFileDialog::getOpenFileNames(this, tr("Choose Files"), initialName);

    if(!selectedImages1.isEmpty()) imageDirectoryName = getPath(selectedImages1.at(0).toLocal8Bit().constData());
    originalEdit->setText(imageDirectoryName);

    QString text = QString("%1").arg(selectedImages1.size());
    text.append(" image(s) selected");
    imagesSelectedLabel1->setText(text);
    checkPathesSet();
    setSuffix();

    refreshSelectedImages(4);
}

//IceGrain
void ProcessDialog::on_predictionBrowse_clicked()
{
    QString initialName = predictionEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    predictionDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    predictionDirectoryName = QDir::toNativeSeparators(predictionDirectoryName);
    predictionDirectoryName.append("/");

    checkDirEmpty(initialName, predictionDirectoryName, *predictionEdit);
}

void ProcessDialog::on_plotsBrowse_clicked()
{
    QString initialName = plotsEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    plotsDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    plotsDirectoryName = QDir::toNativeSeparators(plotsDirectoryName);
    plotsDirectoryName.append("/");

    checkDirEmpty(initialName, plotsDirectoryName, *plotsEdit);
}

void ProcessDialog::on_profilesBrowse_clicked()
{
    QString initialName = profilesEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    profilesDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    profilesDirectoryName = QDir::toNativeSeparators(profilesDirectoryName);
    profilesDirectoryName.append("/");

    checkDirEmpty(initialName, profilesDirectoryName, *profilesEdit);
}

void ProcessDialog::on_imageBrowse2_clicked()
{
    QString initialName = imageDirectoryName;
    if (imageDirectoryName.isEmpty())
        imageDirectoryName = QDir::homePath();
    selectedImages2 = QFileDialog::getOpenFileNames(this, tr("Choose Files"), initialName);

    if(!selectedImages2.isEmpty()) imageDirectoryName = getPath(selectedImages2.at(0).toLocal8Bit().constData());
    originalEdit->setText(imageDirectoryName);

    QString text = QString("%1").arg(selectedImages2.size());
    text.append(" image(s) selected");
    imagesSelectedLabel2->setText(text);
    checkPathesSet();
    setSuffix();

    refreshSelectedImages(5);
}

//View
void ProcessDialog::on_screenshotsBrowse_clicked()
{
    QString initialName = screenshotsEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    screenshotsDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    screenshotsDirectoryName = QDir::toNativeSeparators(screenshotsDirectoryName);
    screenshotsDirectoryName.append("/");

    checkDirEmpty(initialName, screenshotsDirectoryName, *screenshotsEdit);
}

void ProcessDialog::on_imageBrowse3_clicked()
{
    QString initialName = imageDirectoryName;
    if (imageDirectoryName.isEmpty())
        imageDirectoryName = QDir::homePath();
    selectedImage3 = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);

    if(!selectedImage3.isEmpty()) imageDirectoryName = getPath(selectedImage3);
    originalEdit->setText(imageDirectoryName);

    QString text;
    if(!selectedImage3.isEmpty()) text = "1";
        else text ="0";
    text.append(" image(s) selected");
    imagesSelectedLabel3->setText(text);
    checkPathesSet();
    setSuffix();

    refreshSelectedImages(6);
}

//Analysis
void ProcessDialog::on_analyzeListBrowse_clicked()
{
    QString initialName = analyzeListEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    analyzeListFileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    analyzeListFileName = QDir::toNativeSeparators(analyzeListFileName);

    checkDirEmpty(initialName, analyzeListFileName, *analyzeListEdit);
}

void ProcessDialog::on_screenshotsBrowse2_clicked()
{
    QString initialName = screenshotsEdit2->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    screenshotsDirectoryName2 = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    screenshotsDirectoryName2 = QDir::toNativeSeparators(screenshotsDirectoryName2);
    screenshotsDirectoryName2.append("/");

    checkDirEmpty(initialName, screenshotsDirectoryName2, *screenshotsEdit2);
}

void ProcessDialog::checkPathesSet()
{
    disableAll();

    if (!mosaicDirectoryName.isEmpty() && !relativeDirectoryName.isEmpty() && !stitchedDirectoryName.isEmpty() &&
        !batchListFileName.isEmpty() && !parametersFileName.isEmpty() && !processRunning)
    {
        stitchStartButton->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages.isEmpty() && !preprocessedDirectoryName.isEmpty() && !processRunning)
    {
        preprocessStartButton->setEnabled(true);
        preprocessStartButton2->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages.isEmpty() && !thumbsDirectoryName.isEmpty() && !processRunning)
    {
        thumbsStartButton->setEnabled(true);
        thumbsStartButton2->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages1.isEmpty() && !preprocessedDirectoryName.isEmpty() && !processRunning &&
        !pixelFeaturesDirectoryName.isEmpty() && !probMapDirectoryName.isEmpty() && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !parametersFileName.isEmpty() && (!thumbsCheckBox->isChecked() ||
        !thumbsDirectoryName.isEmpty()))
    {
        cisStartButton->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages1.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !probMapDirectoryName.isEmpty() && !parametersFileName.isEmpty() &&
        (!thumbsCheckBox->isChecked() || !thumbsDirectoryName.isEmpty()))
    {
        loadProbButton->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages1.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !parametersFileName.isEmpty() &&
        (!thumbsCheckBox->isChecked() || !thumbsDirectoryName.isEmpty()))
    {
        loadSegmBotton->setEnabled(true);
    }
    if (selectedImages1.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !parametersFileName.isEmpty() && !suffixCheckBox->isChecked())
    {
        loadSegmHDF5Button->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages1.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !parametersFileName.isEmpty() && !probMapDirectoryName.isEmpty() &&
        (!thumbsCheckBox->isChecked() || !thumbsDirectoryName.isEmpty()))
    {
        corrSegmBotton->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages2.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !predictionDirectoryName.isEmpty() && !parametersFileName.isEmpty() &&
        !radioButton_FA->isChecked())
    {
        correctNetworkButton->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages2.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !predictionDirectoryName.isEmpty() && !parametersFileName.isEmpty() &&
        (!thumbsCheckBox->isChecked() || !thumbsDirectoryName.isEmpty()) && !radioButton_FA->isChecked())
    {
        extractNetworkButton->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages2.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !predictionDirectoryName.isEmpty() && !parametersFileName.isEmpty() &&
        !profilesDirectoryName.isEmpty() && radioButton_LASM->isChecked())
    {
        subgrainButton->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImages2.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !predictionDirectoryName.isEmpty() && !plotsDirectoryName.isEmpty() && !profilesDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !parametersFileName.isEmpty() && !radioButton_FA->isChecked())
    {
        getParametersButton->setEnabled(true);
    }
    if (selectedImages2.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !predictionDirectoryName.isEmpty() && !parametersFileName.isEmpty() &&
        radioButton_FA->isChecked() && !suffixCheckBox->isChecked())
    {
        correctNetworkButton->setEnabled(true);
    }
    if (selectedImages2.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !predictionDirectoryName.isEmpty() && !parametersFileName.isEmpty() &&
        (!thumbsCheckBox->isChecked() || !thumbsDirectoryName.isEmpty()) && radioButton_FA->isChecked() &&
        !suffixCheckBox->isChecked())
    {
        extractNetworkButton->setEnabled(true);
    }
    if (selectedImages2.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !predictionDirectoryName.isEmpty() && !plotsDirectoryName.isEmpty() && !profilesDirectoryName.isEmpty() &&
        !boundaryFeaturesDirectoryName.isEmpty() && !parametersFileName.isEmpty() && radioButton_FA->isChecked() &&
        !suffixCheckBox->isChecked())
    {
        getParametersButton->setEnabled(true);
    }
    if (!processRunning && !profilesDirectoryName.isEmpty() && !parametersFileName.isEmpty())
    {
        depthProfilesButton->setEnabled(true);
    }
    if (!parametersFileName.isEmpty())
    {
        labelButton->setEnabled(true);
    }
    if (!imageDirectoryName.isEmpty() && !selectedImage3.isEmpty() && !processRunning && !segmentationDirectoryName.isEmpty() &&
        !screenshotsDirectoryName.isEmpty() && !profilesDirectoryName.isEmpty() && !boundaryFeaturesDirectoryName.isEmpty() &&
        !parametersFileName.isEmpty())
    {
        viewGrainsButton->setEnabled(true);
        viewBoundariesButton->setEnabled(true);
        viewJunctionsButton->setEnabled(true);
        junctionAnalysisButton->setEnabled(true);
        grainOverviewButton->setEnabled(true);
        boundaryOverviewButton->setEnabled(true);
        localCurvatureButton->setEnabled(true);
    }
    if (!predictionDirectoryName.isEmpty() && !profilesDirectoryName.isEmpty() && !analyzeListFileName.isEmpty() && !processRunning &&
        !parametersFileName.isEmpty() && !screenshotsDirectoryName2.isEmpty() && !segmentationDirectoryName.isEmpty())
    {
        analyzeStartButton->setEnabled(true);
        paramPlotButton->setEnabled(true);
        paramListButton->setEnabled(true);
        corrPlotButton->setEnabled(true);
    }
    if (!predictionDirectoryName.isEmpty() && !profilesDirectoryName.isEmpty() && !analyzeListFileName.isEmpty() && !processRunning &&
        !parametersFileName.isEmpty())
    {
        analyzeStartButton_2->setEnabled(true);
    }
    if (true) plotsButton->setEnabled(true);
    if (!elleDataSetFileName.isEmpty() && !elleExportDirectoryName.isEmpty() && !processRunning)
    {
        elleExportButton->setEnabled(true);
    }
}

void ProcessDialog::errorPreprocessed()
{
    QString text = outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(preprocessedDirectoryName);
    text.append(suffix);
    text.append( " does not exist. Check suffix setting!\n");
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

void ProcessDialog::errorProbMap()
{
    QString text = outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(probMapDirectoryName);
    text.append(suffix);
    text.append( " does not exist. Check suffix setting!\n");
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

void ProcessDialog::errorSegmentation()
{
    QString text = outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(segmentationDirectoryName);
    text.append(suffix);
    text.append( " does not exist. Check suffix setting!\n");
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

void ProcessDialog::errorPrediction()
{
    QString text = outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(predictionDirectoryName);
    text.append(suffix);
    text.append( " does not exist. Check suffix setting!\n");
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

void ProcessDialog::errorProfiles()
{
    QString text = outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(profilesDirectoryName);
    text.append(suffix);
    text.append( " does not exist. Check suffix setting!\n");
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

bool ProcessDialog::get_FA_path()
{
    QString fileName = "";
    QString check1 = segmentationDirectoryName;
    QString check2 = segmentationDirectoryName;

    if (check1.size()>7)
    {
        check1.remove(0,check1.size()-7);
        check2.append("cAxes/");

        if(check1=="/cAxes/")
        {
            fileName = segmentationDirectoryName;
            fileName.remove(fileName.size()-7,7); //remove "/cAxes/"
            fileName = getFilename(fileName);
        }
        else if(directoryExists(check2.toAscii().data()))
        {
            fileName = segmentationDirectoryName;
            fileName.remove(fileName.size()-1,1); //remove "/"
            fileName = getFilename(fileName);
            segmentationDirectoryName.append("cAxes/");
            segmentationEdit->setText(segmentationDirectoryName);
        }
    }

    if (!fileName.isEmpty())
    {
        selectedImageFA = segmentationDirectoryName;
        selectedImageFA.append(fileName);
        selectedImageFA.append(".bmp");
        return false;
    }
    else return true;
}

void ProcessDialog::errorFA()
{
    QString text = outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(segmentationDirectoryName);
    text.append( " cannot be identified as cAxes path!\n");
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

/*******************
Initialize processes
*******************/

void ProcessDialog::disableAll()
{
    stitchStartButton->setEnabled(false);
    preprocessStartButton->setEnabled(false);
    preprocessStartButton2->setEnabled(false);
    thumbsStartButton->setEnabled(false);
    thumbsStartButton2->setEnabled(false);
    cisStartButton->setEnabled(false);
    loadProbButton->setEnabled(false);
    loadSegmBotton->setEnabled(false);
    correctNetworkButton->setEnabled(false);
    extractNetworkButton->setEnabled(false);
    getParametersButton->setEnabled(false);
    depthProfilesButton->setEnabled(false);
    labelButton->setEnabled(false);
    plotsButton->setEnabled(false);
    viewGrainsButton->setEnabled(false);
    viewBoundariesButton->setEnabled(false);
    viewJunctionsButton->setEnabled(false);
    junctionAnalysisButton->setEnabled(false);
    grainOverviewButton->setEnabled(false);
    boundaryOverviewButton->setEnabled(false);
    localCurvatureButton->setEnabled(false);
    analyzeStartButton->setEnabled(false);
    paramPlotButton->setEnabled(false);
    paramListButton->setEnabled(false);
    corrPlotButton->setEnabled(false);
    analyzeStartButton_2->setEnabled(false);
    subgrainButton->setEnabled(false);
    corrSegmBotton->setEnabled(false);
    loadSegmHDF5Button->setEnabled(false);
    elleExportButton->setEnabled(false);
}

//Start Labeling
void ProcessDialog::labeling()
{
    saveSettings();
    labelDialog.exec();
}

//Start Plots
void ProcessDialog::plots()
{
    saveSettings();
    plotsDialog.exec();
}

void ProcessDialog::stitchImages()
{
    disableAll();
    outputTextEdit->clear();

    QStringList args;
    args << batchListFileName << mosaicDirectoryName << relativeDirectoryName << parametersFileName;

    noNewLine=false;
    stitchingRunning=true;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("./../IceMatch/IceMatch", args);
}

void ProcessDialog::preprocessImage()
{
    disableAll();
    outputTextEdit->clear();

    QStringList args;
    if (suffix=="no") args << "-as" << selectedImages << preprocessedDirectoryName;
    else
    {
        //output folders, create if not existing
        QString newPath = preprocessedDirectoryName;
        newPath.append(suffix);
        args << "-as" << selectedImages << newPath;

        QDir dir(preprocessedDirectoryName);
        if (!directoryExists(newPath.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("/home/ollie/tbinder/scripts/preprocess.sh", args);
}

void ProcessDialog::preprocessImage2()
{
    disableAll();
    outputTextEdit->clear();

    QStringList args;
    if (suffix=="no") args << "-de" << selectedImages << preprocessedDirectoryName;
    else
    {
        //output folders, create if not existing
        QString newPath = preprocessedDirectoryName;
        newPath.append(suffix);
        args << "-de" << selectedImages << newPath;

        QDir dir(preprocessedDirectoryName);
        if (!directoryExists(newPath.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=true;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("/home/ollie/tbinder/scripts/preprocess.sh", args);
}

void ProcessDialog::createThumbs()
{
    disableAll();
    outputTextEdit->clear();

    QStringList args;
    if (suffix=="no") args << "-resize" << selectedImages << thumbsDirectoryName;
    else
    {
        //output folders, create if not existing
        QString newPath = thumbsDirectoryName;
        newPath.append(suffix);
        args << "-resize" << selectedImages << newPath;

        QDir dir(thumbsDirectoryName);
        if (!directoryExists(newPath.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("/home/ollie/tbinder/scripts/cis.sh", args);
}

void ProcessDialog::reduceThumbs()
{
    disableAll();
    outputTextEdit->clear();

    QStringList args;
    if (suffix=="no") args << "-reduce" << selectedImages << thumbsDirectoryName;
    else
    {
        //output folders, create if not existing
        QString newPath = thumbsDirectoryName;
        newPath.append(suffix);
        args << "-reduce" << selectedImages << newPath;

        QDir dir(thumbsDirectoryName);
        if (!directoryExists(newPath.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("/home/ollie/tbinder/scripts/preprocess.sh", args);
}

void ProcessDialog::segmentImage()
{
    outputTextEdit->clear();
    bool error_preprocessed=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = preprocessedDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_preprocessed=true;

        //output folders, create if not existing
        QDir dir1(pixelFeaturesDirectoryName);
        subFolder = pixelFeaturesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir1.mkdir(suffix.toAscii().data());

        QDir dir2(probMapDirectoryName);
        subFolder = probMapDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir(suffix.toAscii().data());

        QDir dir3(segmentationDirectoryName);
        subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir3.mkdir(suffix.toAscii().data());

        QDir dir4(boundaryFeaturesDirectoryName);
        subFolder = boundaryFeaturesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir4.mkdir(suffix.toAscii().data());
    }

    if (error_preprocessed) errorPreprocessed();

    if (!error_preprocessed)
    {
        disableAll();

        QString thumbs;
        if (!thumbsCheckBox->isChecked()) thumbs = "no";
        else thumbs = thumbsDirectoryName;        

        QString pixelFeaturesDir;
        if(useDefRf0)
        {
            pixelFeaturesDir = pixelFeaturesDirectoryName;
            pixelFeaturesDir.append("TRUE");
        }
        else
        {
            pixelFeaturesDir = labelDialog.pixelRfDirectoryName;
            pixelFeaturesDir.append(labelDialog.pixelRfName);
            pixelFeaturesDir.append(" ");
            pixelFeaturesDir.append(pixelFeaturesDirectoryName);
            pixelFeaturesDir.append("FALSE");
        }

        QStringList args;
        args << "-pixel-complete-gui" << preprocessedDirectoryName << pixelFeaturesDir << probMapDirectoryName <<
            segmentationDirectoryName << boundaryFeaturesDirectoryName << selectedImages1 << parametersFileName << suffix << thumbs;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("/home/ollie/tbinder/scripts/cis.sh", args);
    }
}

void ProcessDialog::loadProb()
{
    outputTextEdit->clear();
    bool error_prob=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = probMapDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prob=true;

        //output folders, create if not existing
        QDir dir1(segmentationDirectoryName);
        subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir1.mkdir(suffix.toAscii().data());

        QDir dir2(boundaryFeaturesDirectoryName);
        subFolder = boundaryFeaturesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir(suffix.toAscii().data());
    }

    if (error_prob) errorProbMap();

    if (!error_prob)
    {
        disableAll();

        QStringList args;
        QString correctedProbMap = probMapDirectoryName;
        if (suffix!="no") correctedProbMap.append(suffix);
        correctedProbMap.append(getFilename(selectedImages1.at(0)));
        QString suffixSegmentationPath = segmentationDirectoryName;
        if (suffix!="no") suffixSegmentationPath.append(suffix);
        args << "-ws-regions" << correctedProbMap << suffixSegmentationPath;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=true;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("/home/ollie/tbinder/scripts/cis.sh", args);
    }
}

void ProcessDialog::loadSegm()
{
    outputTextEdit->clear();
    bool error_segmentation=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        //output folders, create if not existing
        QDir dir(boundaryFeaturesDirectoryName);
        subFolder = boundaryFeaturesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();

    if (!error_segmentation)
    {
        disableAll();

        QStringList args;
        args << "-ws-manual" << selectedImages1 << segmentationDirectoryName << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=true;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("/home/ollie/tbinder/scripts/cis.sh", args);
    }
}

void ProcessDialog::corrSegm()
{
    outputTextEdit->clear();
    bool error_prob=false;
    bool error_segmentation=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = probMapDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prob=true;

        subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        //output folders, create if not existing
        QDir dir(boundaryFeaturesDirectoryName);
        subFolder = boundaryFeaturesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_prob) errorProbMap();
    if (error_segmentation) errorSegmentation();

    if (!error_prob && !error_segmentation)
    {
        disableAll();

        QStringList args;
        QString correctedProbMap = probMapDirectoryName;
        if (suffix!="no") correctedProbMap.append(suffix);
        QString correctedSegmentationImage = segmentationDirectoryName;
        if (suffix!="no") correctedSegmentationImage.append(suffix);
        correctedSegmentationImage.append(getFilename(selectedImages1.at(0)));
        args << "-correct_seg" << imageDirectoryName << correctedSegmentationImage << correctedProbMap << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=true;
        processRunning=true;

        process.start("./../SubGB/SubGB", args);
    }
}

void ProcessDialog::correctNetwork()
{
    outputTextEdit->clear();
    bool error_segmentation=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        //output folders, create if not existing
        QDir dir1(predictionDirectoryName);
        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir1.mkdir(suffix.toAscii().data());

        QDir dir2(subFolder);
        subFolder.append("bubbles");
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir("bubbles");

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        subFolder.append("grains");
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir("grains");

        QDir dir3(boundaryFeaturesDirectoryName);
        subFolder = boundaryFeaturesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir3.mkdir(suffix.toAscii().data());
    }
    else
    {
        //output folders, create if not existing
        QDir dir(predictionDirectoryName);

        QString subFolder = predictionDirectoryName;
        subFolder.append("bubbles");
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("bubbles");

        subFolder = predictionDirectoryName;
        subFolder.append("grains");
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("grains");
    }

    if (error_segmentation) errorSegmentation();

    bool error_cAxes=false;
    if (radioButton_FA->isChecked()) error_cAxes=get_FA_path();
    if (error_cAxes) errorFA();

    if (!error_segmentation && !error_cAxes)
    {
        disableAll();

        QStringList args;
        if(!radioButton_FA->isChecked())
        {
            args << "-corrections-gui" << selectedImages2 << segmentationDirectoryName << predictionDirectoryName <<
                boundaryFeaturesDirectoryName << parametersFileName << suffix;
        }
        else
        {
            args << "-corrections-gui" << selectedImageFA << segmentationDirectoryName << predictionDirectoryName <<
                boundaryFeaturesDirectoryName << parametersFileName << suffix;
        }

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::findSubgrains()
{
    outputTextEdit->clear();
    bool error_segmentation=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        //output folders, create if not existing
        QDir dir1(predictionDirectoryName);
        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir1.mkdir(suffix.toAscii().data());

        QDir dir2(subFolder);
        subFolder.append("bubbles");
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir("bubbles");

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        subFolder.append("grains");
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir("grains");

        QDir dir3(boundaryFeaturesDirectoryName);
        subFolder = boundaryFeaturesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir3.mkdir(suffix.toAscii().data());
    }
    else
    {
        //output folders, create if not existing
        QDir dir(predictionDirectoryName);

        QString subFolder = predictionDirectoryName;
        subFolder.append("bubbles");
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("bubbles");

        subFolder = predictionDirectoryName;
        subFolder.append("grains");
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("grains");
    }

    if (error_segmentation) errorSegmentation();

    if (!error_segmentation)
    {
        disableAll();

        QString thumbs;
        if (!thumbsCheckBox->isChecked()) thumbs = "no";
        else thumbs = thumbsDirectoryName;

        QStringList args;
        QString boundaryFeaturesDir = boundaryFeaturesDirectoryName;       

        QString imageName = getFilename(selectedImages2.at(0));
        QString predictionSubGBDirectoryName = predictionDirectoryName;
        predictionSubGBDirectoryName.remove(predictionDirectoryName.size()-1, 1);
        predictionSubGBDirectoryName.append("_subgrains/");

        QString selectedImages2Names;
        for(int i = 0; i < selectedImages2.size(); i++)
        {
            QString temp = getFilename(selectedImages2[i]);
            selectedImages2Names.push_back(temp);
        }

        args << "-subgrains-gui" << selectedImages2Names << boundaryFeaturesDir << suffix << parametersFileName << imageDirectoryName << segmentationDirectoryName << predictionDirectoryName
             << predictionSubGBDirectoryName << profilesDirectoryName;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::use_DefRf()
{
    if(defRfCheckBox->isChecked())
    {
        profiles[Profile::activeProfileIndex].data[75] = "1";
        useDefRf = true;
    }
    else
    {
        profiles[Profile::activeProfileIndex].data[75] = "0";
        useDefRf = false;
    }
}

void ProcessDialog::use_DefRf0()
{
    if(defRfCheckBox0->isChecked())
    {
        profiles[Profile::activeProfileIndex].data[76] = "1";
        useDefRf0 = true;
    }
    else
    {
        profiles[Profile::activeProfileIndex].data[76] = "0";
        useDefRf0 = false;
    }
}

void ProcessDialog::extractNetwork()
{
    outputTextEdit->clear();
    bool error_segmentation=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        //output folders, create if not existing
        QDir dir1(predictionDirectoryName);
        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir1.mkdir(suffix.toAscii().data());

        QDir dir2(subFolder);
        subFolder.append("bubbles");
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir("bubbles");

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        subFolder.append("grains");
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir("grains");

        QDir dir3(boundaryFeaturesDirectoryName);
        subFolder = boundaryFeaturesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir3.mkdir(suffix.toAscii().data());
    }
    else
    {
        //output folders, create if not existing
        QDir dir(predictionDirectoryName);

        QString subFolder = predictionDirectoryName;
        subFolder.append("bubbles");
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("bubbles");

        subFolder = predictionDirectoryName;
        subFolder.append("grains");
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("grains");
    }

    if (error_segmentation) errorSegmentation();

    bool error_cAxes=false;
    if (radioButton_FA->isChecked()) error_cAxes=get_FA_path();
    if (error_cAxes) errorFA();

    if (!error_segmentation && !error_cAxes)
    {
        disableAll();

        QString thumbs;
        if (!thumbsCheckBox->isChecked()) thumbs = "no";
        else thumbs = thumbsDirectoryName;

        QStringList args;
        QString boundaryFeaturesDir;
        if(useDefRf)
        {
            boundaryFeaturesDir = boundaryFeaturesDirectoryName;
            boundaryFeaturesDir.append("TRUE");           
        }
        else
        {
            boundaryFeaturesDir = labelDialog.labelRfDir;
            boundaryFeaturesDir.append(labelDialog.labelRfName);
            boundaryFeaturesDir.append(" ");
            boundaryFeaturesDir.append(boundaryFeaturesDirectoryName);
            boundaryFeaturesDir.append("FALSE");            
        }

        QString useOriginalImage;
        if(radioButton_LASM->isChecked())
        {
            useOriginalImage="TRUE";
        }
        else
        {
            useOriginalImage="FALSE";
        }

        if(!radioButton_FA->isChecked())
        {
            args << "-predict-gui" << selectedImages2 << segmentationDirectoryName << predictionDirectoryName <<
                boundaryFeaturesDir << parametersFileName << suffix << thumbs << useOriginalImage;
        }
        else
        {
            args << "-predict-gui" << selectedImageFA << segmentationDirectoryName << predictionDirectoryName <<
                boundaryFeaturesDir << parametersFileName << suffix << thumbs << useOriginalImage;
        }

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::getParameters()
{
    outputTextEdit->clear();
    bool error_segmentation=false;
    bool error_prediction=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        //output folders, create if not existing
        QDir dir1(plotsDirectoryName);
        subFolder = plotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir1.mkdir(suffix.toAscii().data());

        QDir dir2(profilesDirectoryName);
        subFolder = profilesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir2.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();
    if (error_prediction) errorPrediction();

    bool error_cAxes=false;
    if (radioButton_FA->isChecked()) error_cAxes=get_FA_path();
    if (error_cAxes) errorFA();

    if (!error_segmentation && !error_prediction && !error_cAxes)
    {
        disableAll();

        QStringList args;
        if(!radioButton_FA->isChecked())
        {
            args << "-statistics-gui" << selectedImages2 << lowGrainSizeBox->text() << highGrainSizeBox->text() <<
                grainSizeStepBox->text() << minBubbleDistanceBox->text() << grainStepBox->text() <<
                segmentationDirectoryName << predictionDirectoryName << plotsDirectoryName <<
                profilesDirectoryName << boundaryFeaturesDirectoryName << parametersFileName << suffix;
        }
        else
        {
            args << "-statistics-gui" << selectedImageFA << lowGrainSizeBox->text() << highGrainSizeBox->text() <<
                grainSizeStepBox->text() << minBubbleDistanceBox->text() << grainStepBox->text() <<
                segmentationDirectoryName << predictionDirectoryName << plotsDirectoryName <<
                profilesDirectoryName << boundaryFeaturesDirectoryName << parametersFileName << suffix;
        }

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::depthProfiles()
{
    disableAll();
    outputTextEdit->clear();

    QStringList args;
    if (suffixCheckBox->isChecked())
    {
        args << "-depth-profile-gui" << depthBinWidthBox->text() << nrDepthsBox->text() <<
            lowGrainSizeBox->text() << highGrainSizeBox->text() << grainSizeStepBox->text() <<
            minBubbleDistanceBox->text() << grainStepBox->text() << profilesDirectoryName <<
            parametersFileName;
    }
    else
    {
        args << "-single-depth-profile-gui" << depthBinWidthBox->text() << lowGrainSizeBox->text() <<
            highGrainSizeBox->text() << grainSizeStepBox->text() << minBubbleDistanceBox->text() <<
            grainStepBox->text() << profilesDirectoryName << parametersFileName;
    }
    //output folders, create if not existing
    QDir dir(profilesDirectoryName);

    QString subFolder = profilesDirectoryName;
    subFolder.append("bin");
    if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("bin");

    subFolder = profilesDirectoryName;
    subFolder.append("percent");
    if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("percent");

    subFolder = profilesDirectoryName;
    subFolder.append("quantile");
    if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("quantile");

    subFolder = profilesDirectoryName;
    subFolder.append("step");
    if (!directoryExists(subFolder.toAscii().data())) dir.mkdir("step");

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("./../IceGrain/IceGrain", args);
}

void ProcessDialog::viewGrains()
{
    outputTextEdit->clear();
    bool error_segmentation=false;
    bool error_prediction=false;
    bool error_profiles=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        subFolder = profilesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_profiles=true;

        //output folders, create if not existing
        QDir dir(screenshotsDirectoryName);
        subFolder = screenshotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();
    if (error_prediction) errorPrediction();
    if (error_profiles) errorProfiles();

    if (!error_segmentation && !error_prediction && !error_profiles)
    {
        disableAll();

        QString startValue = startValueEdit->text();
        if (startValue.isEmpty()) startValue = "-1";

        QStringList args;
        args << "-view-gui" << selectedImage3 << segmentationDirectoryName << predictionDirectoryName << screenshotsDirectoryName <<
            profilesDirectoryName << boundaryFeaturesDirectoryName << lowGrainSizeBox->text() << minBubbleDistanceBox->text() <<
            "0" << startValue << parametersFileName << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::viewBoundaries()
{
    outputTextEdit->clear();
    bool error_segmentation=false;
    bool error_prediction=false;
    bool error_profiles=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        subFolder = profilesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_profiles=true;

        //output folders, create if not existing
        QDir dir(screenshotsDirectoryName);
        subFolder = screenshotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();
    if (error_prediction) errorPrediction();
    if (error_profiles) errorProfiles();

    if (!error_segmentation && !error_prediction && !error_profiles)
    {
        disableAll();

        QString startValue = startValueEdit->text();
        if (startValue.isEmpty()) startValue = "-1";

        QStringList args;
        args << "-view-gui" << selectedImage3 << segmentationDirectoryName << predictionDirectoryName << screenshotsDirectoryName <<
            profilesDirectoryName << boundaryFeaturesDirectoryName << lowGrainSizeBox->text() << minBubbleDistanceBox->text() <<
            "1" << startValue << parametersFileName << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::viewJunctions()
{
    outputTextEdit->clear();
    bool error_segmentation=false;
    bool error_prediction=false;
    bool error_profiles=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        subFolder = profilesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_profiles=true;

        //output folders, create if not existing
        QDir dir(screenshotsDirectoryName);
        subFolder = screenshotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();
    if (error_prediction) errorPrediction();
    if (error_profiles) errorProfiles();

    if (!error_segmentation && !error_prediction && !error_profiles)
    {
        disableAll();

        QString startValue = startValueEdit->text();
        if (startValue.isEmpty()) startValue = "-1";

        QStringList args;
        args << "-view-gui" << selectedImage3 << segmentationDirectoryName << predictionDirectoryName << screenshotsDirectoryName <<
            profilesDirectoryName << boundaryFeaturesDirectoryName << lowGrainSizeBox->text() << minBubbleDistanceBox->text() <<
            "2" << startValue << parametersFileName << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::analyzeJunctions()
{
    outputTextEdit->clear();
    bool error_segmentation=false;
    bool error_prediction=false;
    bool error_profiles=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        subFolder = profilesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_profiles=true;

        //output folders, create if not existing
        QDir dir(screenshotsDirectoryName);
        subFolder = screenshotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();
    if (error_prediction) errorPrediction();
    if (error_profiles) errorProfiles();

    if (!error_segmentation && !error_prediction && !error_profiles)
    {
        disableAll();

        QString startValue = startValueEdit->text();
        if (startValue.isEmpty()) startValue = "-1";

        QStringList args;
        args << "-view-gui" << selectedImage3 << segmentationDirectoryName << predictionDirectoryName << screenshotsDirectoryName <<
            profilesDirectoryName << boundaryFeaturesDirectoryName << lowGrainSizeBox->text() << minBubbleDistanceBox->text() <<
            "3" << startValue << parametersFileName << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::grainOverview()
{
    outputTextEdit->clear();
    bool error_segmentation=false;
    bool error_prediction=false;
    bool error_profiles=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        subFolder = profilesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_profiles=true;

        //output folders, create if not existing
        QDir dir(screenshotsDirectoryName);
        subFolder = screenshotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();
    if (error_prediction) errorPrediction();
    if (error_profiles) errorProfiles();

    if (!error_segmentation && !error_prediction && !error_profiles)
    {
        disableAll();

        QString startValue = startValueEdit->text();
        if (startValue.isEmpty()) startValue = "-1";

        QStringList args;
        args << "-view-gui" << selectedImage3 << segmentationDirectoryName << predictionDirectoryName << screenshotsDirectoryName <<
            profilesDirectoryName << boundaryFeaturesDirectoryName << lowGrainSizeBox->text() << minBubbleDistanceBox->text() <<
            "4" << startValue << parametersFileName << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::boundaryOverview()
{
    outputTextEdit->clear();
    bool error_segmentation=false;
    bool error_prediction=false;
    bool error_profiles=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        subFolder = profilesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_profiles=true;

        //output folders, create if not existing
        QDir dir(screenshotsDirectoryName);
        subFolder = screenshotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();
    if (error_prediction) errorPrediction();
    if (error_profiles) errorProfiles();

    if (!error_segmentation && !error_prediction && !error_profiles)
    {
        disableAll();

        QString startValue = startValueEdit->text();
        if (startValue.isEmpty()) startValue = "-1";

        QStringList args;
        args << "-view-gui" << selectedImage3 << segmentationDirectoryName << predictionDirectoryName << screenshotsDirectoryName <<
            profilesDirectoryName << boundaryFeaturesDirectoryName << lowGrainSizeBox->text() << minBubbleDistanceBox->text() <<
            "9" << startValue << parametersFileName << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::localCurvature()
{
    outputTextEdit->clear();
    bool error_segmentation=false;
    bool error_prediction=false;
    bool error_profiles=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = segmentationDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;

        subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        subFolder = profilesDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_profiles=true;

        //output folders, create if not existing
        QDir dir(screenshotsDirectoryName);
        subFolder = screenshotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_segmentation) errorSegmentation();
    if (error_prediction) errorPrediction();
    if (error_profiles) errorProfiles();

    if (!error_segmentation && !error_prediction && !error_profiles)
    {
        disableAll();

        QString startValue = startValueEdit->text();
        if (startValue.isEmpty()) startValue = "-1";

        QStringList args;
        args << "-view-gui" << selectedImage3 << segmentationDirectoryName << predictionDirectoryName << screenshotsDirectoryName <<
            profilesDirectoryName << boundaryFeaturesDirectoryName << lowGrainSizeBox->text() << minBubbleDistanceBox->text() <<
            "6" << startValue << parametersFileName << suffix;

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("./../IceGrain/IceGrain", args);
    }
}

void ProcessDialog::analyze()
{
    outputTextEdit->clear();
    disableAll();

    QString suff;
    if (suffixCheckBox->isChecked()) suff="1";
    else suff="0";

    QStringList args;
    args << "-analyze" << analyzeListFileName << predictionDirectoryName << profilesDirectoryName<< lowGrainSizeBox->text() <<
            minBubbleDistanceBox->text() << parametersFileName << screenshotsDirectoryName2 << "0" << "0" << "0" << "0"<<
            segmentationDirectoryName << suff;

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("./../IceGrain/IceGrain", args);
}

void ProcessDialog::plotParam()
{
    outputTextEdit->clear();
    disableAll();

    char combo1[20];
    sprintf(combo1, "%i", paramComboBox1->currentIndex());
    char combo2[20];
    sprintf(combo2, "%i", paramComboBox2->currentIndex());

    QString suff;
    if (suffixCheckBox->isChecked()) suff="1";
    else suff="0";

    QStringList args;
    args << "-analyze" << analyzeListFileName << predictionDirectoryName << profilesDirectoryName<< lowGrainSizeBox->text() <<
        minBubbleDistanceBox->text() << parametersFileName << screenshotsDirectoryName2 << "1" << combo1 <<
        paramSpinBox->text() << combo2 << segmentationDirectoryName << suff;

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("./../IceGrain/IceGrain", args);
}

void ProcessDialog::listParam()
{
    outputTextEdit->clear();
    disableAll();

    char combo1[20];
    sprintf(combo1, "%i", paramComboBox1->currentIndex());
    char combo2[20];
    sprintf(combo2, "%i", paramComboBox2->currentIndex());

    QString suff;
    if (suffixCheckBox->isChecked()) suff="1";
    else suff="0";

    QStringList args;
    args << "-analyze" << analyzeListFileName << predictionDirectoryName << profilesDirectoryName<< lowGrainSizeBox->text() <<
        minBubbleDistanceBox->text() << parametersFileName << screenshotsDirectoryName2 << "3" << combo1 <<
        paramSpinBox->text() << combo2 << segmentationDirectoryName << suff;

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("./../IceGrain/IceGrain", args);
}

void ProcessDialog::plotCorrelation()
{
    outputTextEdit->clear();
    disableAll();

    char combo1[20];
    sprintf(combo1, "%i", corrComboBox1->currentIndex());
    char combo2[20];
    sprintf(combo2, "%i", corrComboBox2->currentIndex());

    QString suff;
    if (suffixCheckBox->isChecked()) suff="1";
    else suff="0";

    QStringList args;
    args << "-analyze" << analyzeListFileName << predictionDirectoryName << profilesDirectoryName<< lowGrainSizeBox->text() <<
        minBubbleDistanceBox->text() << parametersFileName << screenshotsDirectoryName2 << "2" << combo1 <<
        corrSpinBox->text() << combo2 << segmentationDirectoryName << suff;

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("./../IceGrain/IceGrain", args);
}

void ProcessDialog::newProfiles()
{
    outputTextEdit->clear();
    disableAll();

    QString suff;
    if (suffixCheckBox->isChecked()) suff="1";
    else suff="0";

    QStringList args;
    args << "-new-depth-profile" << analyzeListFileName << predictionDirectoryName << profilesDirectoryName<< lowGrainSizeBox->text() <<
            minBubbleDistanceBox->text() << parametersFileName << suff << highGrainSizeBox->text() << grainSizeStepBox->text() <<
            depthBinWidthBox->text();

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("./../IceGrain/IceGrain", args);
}

void ProcessDialog::loadSegmHDF5()
{
    outputTextEdit->clear();

    bool error_cAxes=get_FA_path();

    if (error_cAxes) errorFA();

    if (!error_cAxes)
    {
        disableAll();

        QStringList args;
        args << "-boundary-features-gui" << selectedImageFA << segmentationDirectoryName << segmentationDirectoryName <<
                "pixel-features/" << boundaryFeaturesDirectoryName << parametersFileName << "no" << "no";

        noNewLine=false;
        stitchingRunning=false;
        bubblePreprocessingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=true;

        process.start("/home/ollie/tbinder/scripts/cis.sh", args);
    }
}

void ProcessDialog::elleExportImage()
{
    outputTextEdit->clear();
    disableAll();

    QStringList args;
    args << "-elle-export" << elleDataSetFileName << elleExportDirectoryName << elleXBox->text() << elleYBox->text();

    noNewLine=false;
    stitchingRunning=false;
    bubblePreprocessingRunning=false;
    correctSegmentationRunning=false;
    correctSegmentationRunning2=false;
    processRunning=true;

    process.start("/home/ollie/tbinder/scripts/cis.sh", args);
}

/***************
Process handling
***************/

//Process standard output during runtime
void ProcessDialog::readProcessStdout()
{
    if (noNewLine)//if last output has not ended with new line, assume carriage return
    {
        outputTextEdit->moveCursor( QTextCursor::End, QTextCursor::MoveAnchor );
        outputTextEdit->moveCursor( QTextCursor::StartOfLine, QTextCursor::MoveAnchor );
        outputTextEdit->moveCursor( QTextCursor::End, QTextCursor::KeepAnchor );
        outputTextEdit->textCursor().removeSelectedText();
    }

    QByteArray newData = process.readAllStandardOutput();
    QString text;
    if (newData.contains("\r"))//if output contains carriage return, find last one
    {
        int oldPosition[2];
        int newPosition = newData.indexOf("\r");
        oldPosition[0] = newPosition;

        while (newPosition!=-1)
        {
            oldPosition[1] = oldPosition[0];
            oldPosition[0] = newPosition;
            newPosition = newData.indexOf("\r", oldPosition[0]+1);
        }

        if (newData.endsWith("\n"))//take second last
        {
            newData.remove(0, oldPosition[1]+1);//remove output before carriage return
            noNewLine=false;
        }
        else
        {
            newData.remove(0, oldPosition[0]+1);
            noNewLine=true;
        }
    }
    text = outputTextEdit->toPlainText() + QString::fromLocal8Bit(newData);
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

//Process standard error during runtime
void ProcessDialog::readProcessError()
{
    QByteArray newData = process.readAllStandardError();
    QString text = outputTextEdit->toPlainText() + QString::fromLocal8Bit(newData);
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

//Process finished
void ProcessDialog::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit)
    {
        stitchingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=false;

        outputTextEdit->append(tr("Process crashed or killed by user."));
        QTextCursor c =  outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        outputTextEdit->setTextCursor(c);
        outputTextEdit->ensureCursorVisible();
    }
    else if (exitCode != 0)
    {
        stitchingRunning=false;
        correctSegmentationRunning=false;
        correctSegmentationRunning2=false;
        processRunning=false;

        outputTextEdit->append(tr("Process failed."));
        QTextCursor c =  outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        outputTextEdit->setTextCursor(c);
        outputTextEdit->ensureCursorVisible();
    }
    else
    {
        //Start IceView if IceMatch succeeded.
        if(stitchingRunning)
        {
            QStringList args;
            args << batchListFileName << mosaicDirectoryName << relativeDirectoryName << stitchedDirectoryName;

            stitchingRunning=false;   
            process.start("./../IceView/IceView", args);
        }
        //Calculate cgp structure after interactive manipulation of ws region image
        else if(correctSegmentationRunning2)
        {
            QString segDirName = segmentationDirectoryName;
            if (!thumbsCheckBox->isChecked()) segDirName.append(thumbsDirectoryName);

            QString segFileName = segDirName;
            segFileName.append(getFilename(selectedImages1.at(0)));
            segFileName.append(".h5");

            QStringList args;
            args << "-cgp-structure" << segFileName << segDirName;

            correctSegmentationRunning2=false;
            correctSegmentationRunning=true;

            process.start("/home/ollie/tbinder/scripts/cis.sh", args);
        }
        //Calculate boundary features after reload of probmap/segmentation
        else if(correctSegmentationRunning)
        {
            QString thumbs;
            if (!thumbsCheckBox->isChecked()) thumbs = "no";
            else thumbs = thumbsDirectoryName;

            QString temp = probMapDirectoryName;

            QString arg = probMapDirectoryName + getFilename(selectedImages1.at(0));

            if(!fileExists(arg.toStdString().c_str()))
            {
                temp = segmentationDirectoryName;
            }

            QStringList args;
            args << "-boundary-features-gui" << selectedImages1 << segmentationDirectoryName << temp <<
                "pixel-features/" << boundaryFeaturesDirectoryName << parametersFileName << suffix << thumbs;

            correctSegmentationRunning=false;

            process.start("/home/ollie/tbinder/scripts/cis.sh", args);
        }
        //Create a bubble image
        else if (bubblePreprocessingRunning)
        {
            QStringList args;

            QString firn = "0";
            if (!firnBox->isChecked()) firn = "1";

            if (suffix=="no") args << "-bubbles-gui" << selectedImages << preprocessedDirectoryName << parametersFileName << firn;
            else
            {
                QString newPath = preprocessedDirectoryName;
                newPath.append(suffix);
                args << "-bubbles-gui" << selectedImages << newPath << parametersFileName << firn;
            }

            bubblePreprocessingRunning=false;

            process.start("/home/ollie/tbinder/scripts/cis.sh", args);
        }
        else
        {
            processRunning=false;
            outputTextEdit->append(tr("Process succeeded."));
            QTextCursor c =  outputTextEdit->textCursor();
            c.movePosition(QTextCursor::End);
            outputTextEdit->setTextCursor(c);
            outputTextEdit->ensureCursorVisible();
        }
    }
    checkPathesSet();
}

//Process failed to start
void ProcessDialog::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart)
    {
        stitchingRunning=false;
        correctSegmentationRunning=false;
        processRunning=false;
        outputTextEdit->append(tr("Process failed to start."));
        QTextCursor c =  outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        outputTextEdit->setTextCursor(c);
        outputTextEdit->ensureCursorVisible();
    }
    checkPathesSet();
}

//Kill running process
void ProcessDialog::killProcess()
{
    process.close();
}

/************
Path functions
*************/

QString ProcessDialog::getPath(QString pathAndFilename)
{
    int size=pathAndFilename.size();
    int position=0;
    bool found=false;

    while(size>=0 && found==false)
    {
      if (pathAndFilename[size]=='/')
         {
             position=size;
             found=true;
         }
         size--;
    }

    pathAndFilename.remove(position+1,pathAndFilename.size()-position );
    return pathAndFilename;

}

QString ProcessDialog::getFilename(QString pathAndFilename)
{
    int size=pathAndFilename.size();
    int position=0;
    bool found=false;

    while(size>=0&&found==false)
    {
      if (pathAndFilename[size]=='/')
         {
             position=size;
             found=true;
         }
         size--;
    }

    if(found) pathAndFilename.remove(0,position+1);
    return pathAndFilename;
}

bool ProcessDialog::fileExists(const char* filename)
{
    std::ifstream file(filename);
    std::string teststring;
    bool bExists = false;

    file>>teststring;

    if(file)
    {
        bExists = true;
        file.close();
    }
    return bExists;
}

bool ProcessDialog::directoryExists(const char* pzPath )
{
    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
    {
        bExists = true;
        closedir (pDir);
    }
    return bExists;
}

//if QString returned by 'getOpenFileName' is empty, use old QString
void ProcessDialog::checkDirEmpty(QString initialName, QString &newDir, QLineEdit &target)
{
    if(newDir == "/" || newDir.isEmpty())
    {
        if(initialName.compare(QDir::homePath()) == 0)
        {
            target.setText("");
        }
        else
        {
            target.setText(initialName);
            newDir = initialName;
        }
    }
    else
    {
        if(!initialName.isEmpty())
        {
            target.setText(newDir);
            checkPathesSet();
        }
        else
        {
            target.setText(initialName);
            checkPathesSet();
        }
    }
}

/*************
Handle profiles
**************/

bool ProcessDialog::isProfileChanged()
{
    //Check if the profile has been changed
    bool changed = false;   
    if(Profile::unmodifiedProfile.data.size() == profiles[Profile::activeProfileIndex].data.size())
    {
        for(int i = 2; i < profiles[Profile::activeProfileIndex].data.size(); i++)
        {
            QString orig = Profile::unmodifiedProfile.data[i];
            QString new_data = profiles[Profile::activeProfileIndex].data[i];
            //std::cout << "Index[" << i << "]: Comparing " << orig.toStdString() << " (profile: " << profileData[0].toStdString() << ") with " << new_data.toStdString() << " (profile: " << profiles[Profile::activeProfileIndex][0].toStdString() << ")" << std::endl;
            if(orig.compare(new_data) != 0)
            {
                changed = true;
                break;
            }
        }
    }
    return changed;
}

void ProcessDialog::changeActiveProfile()
{        
    //Save settings from all profiles
    saveSettings();

    int originalDataIndex = Profile::unmodifiedProfile.index;

    bool changed = false;
    changed = isProfileChanged();

    int choice = 0;
    if(changed && originalDataIndex != availableProfilesComboBox->currentIndex())
    {
        QMessageBox profileChangedMsgBox;
        profileChangedMsgBox.setIcon(QMessageBox::Question);
        profileChangedMsgBox.setWindowTitle("Current profile has been edited");
        profileChangedMsgBox.setText("The current profile has been edited. Do you want to save the changes?");
        profileChangedMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        profileChangedMsgBox.setDefaultButton(QMessageBox::Yes);

        int ret = profileChangedMsgBox.exec();
        switch(ret)
        {
            case QMessageBox::Yes:
            {
                choice = 1;
                break;
            }
            case QMessageBox::No:
            {
                choice = 2;
                break;
            }
            case QMessageBox::Cancel:
            {
                choice = 3;
                break;
            }
        }

        profileChangedMsgBox.close();

        if(choice == 1)
        {
            saveSettings();
            Profile::activeProfileIndex = availableProfilesComboBox->currentIndex();

            //Assign new active profile index, load settings from new active profile index and update profile label
            loadSettings();
            plotsDialog.loadSettings();
            labelDialog.loadSettings();
            updateProfileLabel();
        }
        else if(choice == 2)
        {                       
            profiles[Profile::unmodifiedProfile.index] = Profile::unmodifiedProfile;
            Profile::activeProfileIndex = availableProfilesComboBox->currentIndex();
            //Assign new active profile index, load settings from new active profile index and update profile label
            loadSettings();
            plotsDialog.loadSettings();
            labelDialog.loadSettings();
            updateProfileLabel();
        }
        else if(choice == 3)
        {                       
            availableProfilesComboBox->setCurrentIndex(originalDataIndex);
        }
    }
    else if(!changed)
    {
        saveSettings();
        Profile::activeProfileIndex = availableProfilesComboBox->currentIndex();

        //Assign new active profile index, load settings from new active profile index and update profile label        
        loadSettings();
        plotsDialog.loadSettings();
        labelDialog.loadSettings();
        updateProfileLabel();
    }

}

void ProcessDialog::updateProfileLabel()
{
    QString activeProfileLabelText = "Profile:     ";
    activeProfileLabelText.append(profiles[Profile::activeProfileIndex].getName());
    activeProfileLabel->setText(activeProfileLabelText);
}

//Creates a new profile using an input and message dialog
void ProcessDialog::createNewProfile()
{
    //List of profiles to choose from
    QStringList profilesItems;
    QString item;
    //Add empty profile as first element in the list. All available profiles come after.
    profilesItems << tr("An empty profile");
    for(unsigned int i = 0; i < profiles.size(); i++)
    {
        item = profiles[i].getName();
        profilesItems << item;
    }

    //'Ok' button status for both dialogs
    bool ok;
    bool okName = false;

    //Let the user choose wether to create a new empty profile or use a copy of an existing profile
    QString selectionItem = QInputDialog::getItem(this, tr("Create a new profile"), tr("You can create a new empty profile or use an existing profile. Create a new profile using: "), profilesItems, Profile::activeProfileIndex + 1, false, &ok);
    //The new profile name
    QString newProfileName;
    if(ok && !selectionItem.isEmpty())
    {
        //Let the user give the new profile a name
        profileNameGiver: newProfileName = QInputDialog::getText(this, tr("Enter a profile name"), tr("Specifiy a name for the new profile:"), QLineEdit::Normal, "New profile", &okName);
        if(okName && !newProfileName.isEmpty())
        {
            //Change active profile to the newly created profile and create a new empty profile if this option has been selected, else create a new profile using the specified profile's data
            changeActiveProfile();
            if(selectionItem.compare("An empty profile") == 0)
            {
                //Add profile with empty data
                std::vector<QString> empty;
                empty.push_back("[" + newProfileName + "]");
                for(int i = 0; i < Profile::numberOfSettings-1; i++)
                {
                    empty.push_back("");
                }
                Profile p(newProfileName);
                p.setData(empty);
                p.index = profiles.size();
                profiles.push_back(p);
                //Active profile is the newly created profile
                Profile::activeProfileIndex = profiles.size() - 1;
                //Update the list of available profiles
                updateAvailableProfiles();
            }
            else
            {
                //Add profile with the specified profile's data
                Profile p(newProfileName);
                p.setData(profiles[Profile::activeProfileIndex].getData());
                p.index = profiles.size();
                profiles.push_back(p);
                //Active profile is the newly created profile
                Profile::activeProfileIndex = profiles.size() - 1;
                //Update the list of available profiles
                updateAvailableProfiles();
            }
        }
        //Give an error message and jump back to the name dialog when the user specifies an empty string as name
        else if(okName && newProfileName.isEmpty())
        {
            QMessageBox::critical(this, tr("Specifiy a name"), tr("You must specifiy a name for the new profile."));
            goto profileNameGiver; //Using the evil goto statement to jump back to the profile name dialog, cannot substitute goto for anything else at the moment. Maybe I will think of a better solution in the future. *Ducks away in cover*
        }
    }
}

//Updates profiles in the profiles combo box when a profile has been added/removed and loads the information from the currently active profile into the GUI
void ProcessDialog::updateAvailableProfiles()
{
    //Clear current profile list
    availableProfilesComboBox->clear();

    //Rebuild profile list using the new data. Select the active profile in the combo box and update the profile label.
    QString item;
    for(unsigned int i = 0; i < profiles.size(); i++)
    {
        item = profiles[i].getName();
        availableProfilesComboBox->addItem(item);
    }
    availableProfilesComboBox->setCurrentIndex(Profile::activeProfileIndex);
    updateProfileLabel();

    //Load the active profile's settings into the GUI
    loadSettings();
    plotsDialog.loadSettings();
    labelDialog.loadSettings();    
}

//Deletes a profile using a message box dialog.
void ProcessDialog::deleteProfile()
{
    if(profiles.size() > 1)
    {
        int ret = QMessageBox::question(this, "Delete selected profile?", "Do you really want to delete the profile '" + profiles[Profile::activeProfileIndex].getName() + "'?", QMessageBox::Yes | QMessageBox::No);
        switch(ret)
        {
            case QMessageBox::Yes:
            {
                int index = Profile::activeProfileIndex;
                int newIndex = (index + 1) % (profiles.size() - 1);
                std::vector<Profile> newProfiles;
                for(int i = 0; i < profiles.size(); i++)
                {
                    if(i != Profile::activeProfileIndex)
                    {
                        profiles[i].index = i;
                        newProfiles.push_back(profiles[i]);
                    }
                }                
                profiles = newProfiles;
                Profile::activeProfileIndex = newIndex;
                updateAvailableProfiles();
                break;
            }
            case QMessageBox::No:
            {
                break;
            }
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Cannot delete profile"), tr("There has to be at least one profile remaining."));
    }
}

//Changes a profile's name using an input dialog
void ProcessDialog::changeProfileName()
{
    bool okName;
    QString newProfileName;
    profileEditGiver: newProfileName = QInputDialog::getText(this, tr("Enter a profile name"), tr("Specifiy a new name for the profile:"), QLineEdit::Normal, profiles[Profile::activeProfileIndex].getName(), &okName);
    if(okName && !newProfileName.isEmpty())
    {           
            profiles[Profile::activeProfileIndex].setName(newProfileName);
            updateAvailableProfiles();
    }
    else if(okName && newProfileName.isEmpty())
    {
        QMessageBox::critical(this, tr("Specifiy a name"), tr("You must specifiy a name for the new profile."));
        goto profileEditGiver; //Using the evil goto statement to jump back to the profile name dialog, cannot substitute goto for anything else at the moment. Maybe I will think of a better solution in the future. *Ducks away in cover*
    }
}

/**************
Selected images
**************/

void ProcessDialog::refreshSelectedImages(int index)
{
    QStringList selImages;
    if(index == 0) //Main tab
    {
        selectedImageslistWidget->clear();
    }
    if(index == 1) //Import Elle
    {
        selectedImageslistWidget->clear();
    }
    if(index == 2) //Stitching tab
    {
        selectedImageslistWidget->clear();
    }
    if(index == 3) //Preprocessing tab
    {
        selectedImageslistWidget->clear();
        if(hidePaths)
        {
            for(int i = 0; i < selectedImages.size(); i++)
            {
                QString tmp = getFilename(selectedImages[i]);
                selImages << tmp;
            }
            selectedImageslistWidget->addItems(selImages);
        }
        else
        {
            selectedImageslistWidget->addItems(selectedImages);
        }
    }
    if(index == 4) //Segmentation tab
    {
        selectedImageslistWidget->clear();
        if(hidePaths)
        {
            for(int i = 0; i < selectedImages1.size(); i++)
            {
                QString tmp = getFilename(selectedImages1[i]);
                selImages << tmp;
            }
            selectedImageslistWidget->addItems(selImages);
        }
        else
        {
            selectedImageslistWidget->addItems(selectedImages1);
        }
    }
    if(index == 5) //Network Extraction tab
    {
        selectedImageslistWidget->clear();
        if(hidePaths)
        {
            for(int i = 0; i < selectedImages2.size(); i++)
            {
                QString tmp = getFilename(selectedImages2[i]);
                selImages << tmp;
            }
            selectedImageslistWidget->addItems(selImages);
        }
        else
        {
            selectedImageslistWidget->addItems(selectedImages2);
        }
    }
    if(index == 6) //View tab
    {
        selectedImageslistWidget->clear();
        if(hidePaths)
        {
            QString tmp = getFilename(selectedImage3);
            selectedImageslistWidget->addItem(tmp);
        }
        else
        {
            selectedImageslistWidget->addItem(selectedImage3);
        }
    }
    if(index == 7) //Analysis tab
    {
        selectedImageslistWidget->clear();
    }
}

void ProcessDialog::hide_Paths()
{
    if(hideSelectedImagesPathsCheckBox->isChecked())
    {
        hidePaths = true;
        refreshSelectedImages(tabWidget->currentIndex());
    }
    else
    {
        hidePaths = false;
        refreshSelectedImages(tabWidget->currentIndex());
    }
}

/****************
Check installation
*****************/

void ProcessDialog::write(QByteArray newText, bool clear)
{
    if(clear) outputTextEdit->setPlainText("");
    QString text = outputTextEdit->toPlainText() + QString::fromLocal8Bit(newText);
    outputTextEdit->setPlainText(text);
    QTextCursor c =  outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    outputTextEdit->setTextCursor(c);
    outputTextEdit->ensureCursorVisible();
}

//Check executables
void ProcessDialog::checkInstallation()
{
    stitchingRunning = false;
    bubblePreprocessingRunning = false;
    correctSegmentationRunning = false;
    correctSegmentationRunning2 = false;

    write("",true);
    write("Not implemented\n",false);
}
