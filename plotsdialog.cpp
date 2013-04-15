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
#include "plotsdialog.h"
#include <dirent.h>

#include "globals.h"
#include "profile.h"

PlotsDialog::PlotsDialog(QWidget *parent): QDialog(parent), ui(new Ui::PlotsDialog)
{   
    ui->setupUi(this);

    disableAll();
    processRunning=false;

    connect(ui->killProcessButton, SIGNAL(clicked()), this, SLOT(killProcess()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(saveSettings()));

    connect(ui->image100Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage100FileName()));
    connect(ui->image200Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage200FileName()));
    connect(ui->image300Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage300FileName()));
    connect(ui->image400Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage400FileName()));
    connect(ui->image500Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage500FileName()));
    connect(ui->image600Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage600FileName()));
    connect(ui->image101Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage101FileName()));
    connect(ui->image201Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage201FileName()));
    connect(ui->image301Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage301FileName()));
    connect(ui->image401Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage401FileName()));
    connect(ui->image501Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage501FileName()));
    connect(ui->image601Edit, SIGNAL(textChanged(QString)), this, SLOT(updateImage601FileName()));
    connect(ui->predictionEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePredictionDirectoryName()));
    connect(ui->cloudyPlotsEdit, SIGNAL(textChanged(QString)), this, SLOT(updateCloudyPlotsDirectoryName()));
    connect(ui->profilesEdit, SIGNAL(textChanged(QString)), this, SLOT(updateProfilesDirectoryName()));
    connect(ui->histogramPlotsEdit, SIGNAL(textChanged(QString)), this, SLOT(updateHistogramPlotsDirectoryName()));

    connect(ui->cloudyBandsStartButton, SIGNAL(clicked()), this, SLOT(cloudyBands()));
    connect(ui->doubleHistogramStartButton, SIGNAL(clicked()), this, SLOT(doubleHistogram()));

    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessStdout()));
    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(readProcessError()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));       


    /*std::string pathSettings = (QDir::homePath()).toAscii().data();
    pathSettings.append("/.plotsdialog");
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

        image100FileName=settings.at(0).toLocal8Bit().constData();
        ui->image100Edit->setText(image100FileName);
        image200FileName=settings.at(1).toLocal8Bit().constData();
        ui->image200Edit->setText(image200FileName);
        image300FileName=settings.at(2).toLocal8Bit().constData();
        ui->image300Edit->setText(image300FileName);
        image400FileName=settings.at(3).toLocal8Bit().constData();
        ui->image400Edit->setText(image400FileName);
        image500FileName=settings.at(4).toLocal8Bit().constData();
        ui->image500Edit->setText(image500FileName);
        image600FileName=settings.at(5).toLocal8Bit().constData();
        ui->image600Edit->setText(image600FileName);
        image101FileName=settings.at(6).toLocal8Bit().constData();
        ui->image101Edit->setText(image101FileName);
        image201FileName=settings.at(7).toLocal8Bit().constData();
        ui->image201Edit->setText(image201FileName);
        image301FileName=settings.at(8).toLocal8Bit().constData();
        ui->image301Edit->setText(image301FileName);
        image401FileName=settings.at(9).toLocal8Bit().constData();
        ui->image401Edit->setText(image401FileName);
        image501FileName=settings.at(10).toLocal8Bit().constData();
        ui->image501Edit->setText(image501FileName);
        image601FileName=settings.at(11).toLocal8Bit().constData();
        ui->image601Edit->setText(image601FileName);
        predictionDirectoryName=settings.at(12).toLocal8Bit().constData();
        ui->predictionEdit->setText(predictionDirectoryName);
        cloudyPlotsDirectoryName=settings.at(13).toLocal8Bit().constData();
        ui->cloudyPlotsEdit->setText(cloudyPlotsDirectoryName);
        if (settings.size()>14)
        {
            profilesDirectoryName=settings.at(14).toLocal8Bit().constData();
            ui->profilesEdit->setText(profilesDirectoryName);
            histogramPlotsDirectoryName=settings.at(15).toLocal8Bit().constData();
            ui->histogramPlotsEdit->setText(histogramPlotsDirectoryName);
            ui->lowGrainSizeBox->setValue(atoi(settings.at(16).toLocal8Bit().constData()));
            ui->minBubbleDistanceBox->setValue(atoi(settings.at(17).toLocal8Bit().constData()));
            ui->comboBox->setCurrentIndex(atoi(settings.at(18).toLocal8Bit().constData()));
            ui->comboBox_2->setCurrentIndex(atoi(settings.at(19).toLocal8Bit().constData()));
            temp_settings_file.close();
            settings_file.close();
        }
        checkPathesSet();
    }

    //Load parameter file name and check suffix setting
    pathSettings = (QDir::homePath()).toAscii().data();
    pathSettings.append("/.processdialog");
    std::ifstream settings_file2(pathSettings.c_str());
    std::ifstream temp_settings_file2(pathSettings.c_str());
    std::string teststring2;
    temp_settings_file>>teststring2;

    if(settings_file && teststring.size()!=0)
    {
        std::string str;
        settings_file2>>str;
        QString setting = QString::fromStdString(str);
        QStringList settings = setting.split(",");
        parametersFileName=settings.at(6).toLocal8Bit().constData();
        if (atoi(settings.at(20).toLocal8Bit().constData())==0) suffix="no";
        setSuffix();
        temp_settings_file2.close();
        settings_file2.close();
    }*/
}

/*Load settings from global data structures*/
void PlotsDialog::loadSettings()
{
    //Load last settings
    std::string emptystring = "";
    image100FileName=profiles[Profile::activeProfileIndex].data[55];
    ui->image100Edit->setText(image100FileName);
    image200FileName=profiles[Profile::activeProfileIndex].data[56];
    ui->image200Edit->setText(image200FileName);
    image300FileName=profiles[Profile::activeProfileIndex].data[57];
    ui->image300Edit->setText(image300FileName);
    image400FileName=profiles[Profile::activeProfileIndex].data[58];
    ui->image400Edit->setText(image400FileName);
    image500FileName=profiles[Profile::activeProfileIndex].data[59];
    ui->image500Edit->setText(image500FileName);
    image600FileName=profiles[Profile::activeProfileIndex].data[60];
    ui->image600Edit->setText(image600FileName);
    image101FileName=profiles[Profile::activeProfileIndex].data[61];
    ui->image101Edit->setText(image101FileName);
    image201FileName=profiles[Profile::activeProfileIndex].data[62];
    ui->image201Edit->setText(image201FileName);
    image301FileName=profiles[Profile::activeProfileIndex].data[63];
    ui->image301Edit->setText(image301FileName);
    image401FileName=profiles[Profile::activeProfileIndex].data[64];
    ui->image401Edit->setText(image401FileName);
    image501FileName=profiles[Profile::activeProfileIndex].data[65];
    ui->image501Edit->setText(image501FileName);
    image601FileName=profiles[Profile::activeProfileIndex].data[66];
    ui->image601Edit->setText(image601FileName);
    predictionDirectoryName=profiles[Profile::activeProfileIndex].data[67];
    ui->predictionEdit->setText(predictionDirectoryName);
    cloudyPlotsDirectoryName=profiles[Profile::activeProfileIndex].data[68];
    ui->cloudyPlotsEdit->setText(cloudyPlotsDirectoryName);
    profilesDirectoryName=profiles[Profile::activeProfileIndex].data[69];
    ui->profilesEdit->setText(profilesDirectoryName);
    histogramPlotsDirectoryName=profiles[Profile::activeProfileIndex].data[70];
    ui->histogramPlotsEdit->setText(histogramPlotsDirectoryName);
    if(profiles[Profile::activeProfileIndex].data[71].toStdString().compare(emptystring) != 0) ui->lowGrainSizeBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[71].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[72].toStdString().compare(emptystring) != 0) ui->minBubbleDistanceBox->setValue(atoi(profiles[Profile::activeProfileIndex].data[72].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[73].toStdString().compare(emptystring) != 0) ui->comboBox->setCurrentIndex(atoi(profiles[Profile::activeProfileIndex].data[73].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[74].toStdString().compare(emptystring) != 0) ui->comboBox_2->setCurrentIndex(atoi(profiles[Profile::activeProfileIndex].data[74].toLocal8Bit().constData()));
    if(profiles[Profile::activeProfileIndex].data[22].toStdString().compare(emptystring) != 0) if(atoi(profiles[Profile::activeProfileIndex].data[22].toLocal8Bit().constData())==0) suffix="no";
    setSuffix();

    parametersFileName=profiles[Profile::activeProfileIndex].data[8];


    checkPathesSet();
}

/********
Load settings from legacy file
********/
void PlotsDialog::loadSettingsLegacy()
{
        std::string pathSettings = (QDir::homePath()).toAscii().data();
        pathSettings.append("/.plotsdialog");
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

            image100FileName=settings.at(0).toLocal8Bit().constData();
            ui->image100Edit->setText(image100FileName);
            image200FileName=settings.at(1).toLocal8Bit().constData();
            ui->image200Edit->setText(image200FileName);
            image300FileName=settings.at(2).toLocal8Bit().constData();
            ui->image300Edit->setText(image300FileName);
            image400FileName=settings.at(3).toLocal8Bit().constData();
            ui->image400Edit->setText(image400FileName);
            image500FileName=settings.at(4).toLocal8Bit().constData();
            ui->image500Edit->setText(image500FileName);
            image600FileName=settings.at(5).toLocal8Bit().constData();
            ui->image600Edit->setText(image600FileName);
            image101FileName=settings.at(6).toLocal8Bit().constData();
            ui->image101Edit->setText(image101FileName);
            image201FileName=settings.at(7).toLocal8Bit().constData();
            ui->image201Edit->setText(image201FileName);
            image301FileName=settings.at(8).toLocal8Bit().constData();
            ui->image301Edit->setText(image301FileName);
            image401FileName=settings.at(9).toLocal8Bit().constData();
            ui->image401Edit->setText(image401FileName);
            image501FileName=settings.at(10).toLocal8Bit().constData();
            ui->image501Edit->setText(image501FileName);
            image601FileName=settings.at(11).toLocal8Bit().constData();
            ui->image601Edit->setText(image601FileName);
            predictionDirectoryName=settings.at(12).toLocal8Bit().constData();
            ui->predictionEdit->setText(predictionDirectoryName);
            cloudyPlotsDirectoryName=settings.at(13).toLocal8Bit().constData();
            ui->cloudyPlotsEdit->setText(cloudyPlotsDirectoryName);
            if (settings.size()>14)
            {
                profilesDirectoryName=settings.at(14).toLocal8Bit().constData();
                ui->profilesEdit->setText(profilesDirectoryName);
                histogramPlotsDirectoryName=settings.at(15).toLocal8Bit().constData();
                ui->histogramPlotsEdit->setText(histogramPlotsDirectoryName);
                ui->lowGrainSizeBox->setValue(atoi(settings.at(16).toLocal8Bit().constData()));
                ui->minBubbleDistanceBox->setValue(atoi(settings.at(17).toLocal8Bit().constData()));
                ui->comboBox->setCurrentIndex(atoi(settings.at(18).toLocal8Bit().constData()));
                ui->comboBox_2->setCurrentIndex(atoi(settings.at(19).toLocal8Bit().constData()));
                temp_settings_file.close();
                settings_file.close();
            }
            checkPathesSet();
        }

        //Load parameter file name and check suffix setting
        pathSettings = (QDir::homePath()).toAscii().data();
        pathSettings.append("/.processdialog");
        std::ifstream settings_file2(pathSettings.c_str());
        std::ifstream temp_settings_file2(pathSettings.c_str());
        std::string teststring2;
        temp_settings_file>>teststring2;

        if(settings_file && teststring.size()!=0)
        {
            std::string str;
            settings_file2>>str;
            QString setting = QString::fromStdString(str);
            QStringList settings = setting.split(",");
            parametersFileName=settings.at(6).toLocal8Bit().constData();
            if (atoi(settings.at(20).toLocal8Bit().constData())==0) suffix="no";
            setSuffix();
            temp_settings_file2.close();
            settings_file2.close();
        }

        //Set up data structures
        profiles[Profile::activeProfileIndex].data[55] = image100FileName;
        profiles[Profile::activeProfileIndex].data[56] = image200FileName;
        profiles[Profile::activeProfileIndex].data[57] = image300FileName;
        profiles[Profile::activeProfileIndex].data[58] = image400FileName;
        profiles[Profile::activeProfileIndex].data[59] = image500FileName;
        profiles[Profile::activeProfileIndex].data[60] = image600FileName;
        profiles[Profile::activeProfileIndex].data[61] = image101FileName;
        profiles[Profile::activeProfileIndex].data[62] = image201FileName;
        profiles[Profile::activeProfileIndex].data[63] = image301FileName;
        profiles[Profile::activeProfileIndex].data[64] = image401FileName;
        profiles[Profile::activeProfileIndex].data[65] = image501FileName;
        profiles[Profile::activeProfileIndex].data[66] = image601FileName;
        profiles[Profile::activeProfileIndex].data[67] = predictionDirectoryName;
        profiles[Profile::activeProfileIndex].data[68] = cloudyPlotsDirectoryName;
        profiles[Profile::activeProfileIndex].data[69] = profilesDirectoryName;
        profiles[Profile::activeProfileIndex].data[70] = histogramPlotsDirectoryName;
        profiles[Profile::activeProfileIndex].data[71] = QString::fromStdString(Profile::convertInt(ui->lowGrainSizeBox->value()));
        profiles[Profile::activeProfileIndex].data[72] = QString::fromStdString(Profile::convertInt(ui->minBubbleDistanceBox->value()));
        profiles[Profile::activeProfileIndex].data[73] = QString::fromStdString(Profile::convertInt(ui->comboBox->currentIndex()));
        profiles[Profile::activeProfileIndex].data[74] = QString::fromStdString(Profile::convertInt(ui->comboBox_2->currentIndex()));
}

/*********
Set pathes
*********/
void PlotsDialog::saveSettings()
{
    //Update value Boxes and stuff
    profiles[Profile::activeProfileIndex].data[71] = QString::fromStdString(Profile::convertInt(ui->lowGrainSizeBox->value()));
    profiles[Profile::activeProfileIndex].data[72] = QString::fromStdString(Profile::convertInt(ui->minBubbleDistanceBox->value()));
    profiles[Profile::activeProfileIndex].data[73] = QString::fromStdString(Profile::convertInt(ui->comboBox->currentIndex()));
    profiles[Profile::activeProfileIndex].data[74] = QString::fromStdString(Profile::convertInt(ui->comboBox_2->currentIndex()));

    reject();
}

void PlotsDialog::updateImage100FileName()
{
    image100FileName = ui->image100Edit->text();
    profiles[Profile::activeProfileIndex].data[55] = image100FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage200FileName()
{
    image200FileName = ui->image200Edit->text();
    profiles[Profile::activeProfileIndex].data[56] = image200FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage300FileName()
{
    image300FileName = ui->image300Edit->text();
    profiles[Profile::activeProfileIndex].data[57] = image300FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage400FileName()
{
    image400FileName = ui->image400Edit->text();
    profiles[Profile::activeProfileIndex].data[58] = image400FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage500FileName()
{
    image500FileName = ui->image500Edit->text();
    profiles[Profile::activeProfileIndex].data[59] = image500FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage600FileName()
{
    image600FileName = ui->image600Edit->text();
    profiles[Profile::activeProfileIndex].data[60] = image600FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage101FileName()
{
    image101FileName = ui->image101Edit->text();
    profiles[Profile::activeProfileIndex].data[61] = image101FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage201FileName()
{
    image201FileName = ui->image201Edit->text();
    profiles[Profile::activeProfileIndex].data[62] = image201FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage301FileName()
{
    image301FileName = ui->image301Edit->text();
    profiles[Profile::activeProfileIndex].data[63] = image301FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage401FileName()
{
    image401FileName = ui->image401Edit->text();
    profiles[Profile::activeProfileIndex].data[64] = image401FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage501FileName()
{
    image501FileName = ui->image501Edit->text();
    profiles[Profile::activeProfileIndex].data[65] = image501FileName;
    checkPathesSet();
}

void PlotsDialog::updateImage601FileName()
{
    image601FileName = ui->image601Edit->text();
    profiles[Profile::activeProfileIndex].data[66] = image601FileName;
    checkPathesSet();
}

void PlotsDialog::updatePredictionDirectoryName()
{
    predictionDirectoryName = ui->predictionEdit->text();
    profiles[Profile::activeProfileIndex].data[67] = predictionDirectoryName;
    checkPathesSet();
}

void PlotsDialog::updateCloudyPlotsDirectoryName()
{
    cloudyPlotsDirectoryName = ui->cloudyPlotsEdit->text();
    profiles[Profile::activeProfileIndex].data[68] = cloudyPlotsDirectoryName;
    checkPathesSet();
}

void PlotsDialog::updateProfilesDirectoryName()
{
    profilesDirectoryName = ui->profilesEdit->text();
    profiles[Profile::activeProfileIndex].data[69] = profilesDirectoryName;
    checkPathesSet();
}

void PlotsDialog::updateHistogramPlotsDirectoryName()
{
    histogramPlotsDirectoryName = ui->histogramPlotsEdit->text();
    profiles[Profile::activeProfileIndex].data[70] = histogramPlotsDirectoryName;
    checkPathesSet();
}

void PlotsDialog::setSuffix()
{
    if (suffix=="no")
    {
        ui->suffixLabel->setText("");
    }
    else
    {
        QString text = "Current suffix:     ";
        QString getSuffix = getPath(ui->image100Edit->text().toAscii().data());
        getSuffix.resize(getSuffix.size()-1);
        suffix=getFilename(getSuffix);
        suffix.append("/");
        text.append(suffix);
        ui->suffixLabel->setText(text);
    }
}

//Cloudy bands plot
void PlotsDialog::on_image100Browse_clicked()
{
    QString initialName = ui->image100Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image100FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image100FileName = QDir::toNativeSeparators(image100FileName);

    ui->image100Edit->setText(image100FileName);
    checkPathesSet();
}

void PlotsDialog::on_image200Browse_clicked()
{
    QString initialName = ui->image200Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image200FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image200FileName = QDir::toNativeSeparators(image200FileName);

    ui->image200Edit->setText(image200FileName);
    checkPathesSet();
}

void PlotsDialog::on_image300Browse_clicked()
{
    QString initialName = ui->image300Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image300FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image300FileName = QDir::toNativeSeparators(image300FileName);

    ui->image300Edit->setText(image300FileName);
    checkPathesSet();
}

void PlotsDialog::on_image400Browse_clicked()
{
    QString initialName = ui->image400Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image400FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image400FileName = QDir::toNativeSeparators(image400FileName);

    ui->image400Edit->setText(image400FileName);
    checkPathesSet();
}

void PlotsDialog::on_image500Browse_clicked()
{
    QString initialName = ui->image500Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image500FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image500FileName = QDir::toNativeSeparators(image500FileName);

    ui->image500Edit->setText(image500FileName);
    checkPathesSet();
}

void PlotsDialog::on_image600Browse_clicked()
{
    QString initialName = ui->image600Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image600FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image600FileName = QDir::toNativeSeparators(image600FileName);

    ui->image600Edit->setText(image600FileName);
    checkPathesSet();
}

void PlotsDialog::on_image101Browse_clicked()
{
    QString initialName = ui->image101Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image101FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image101FileName = QDir::toNativeSeparators(image101FileName);

    ui->image101Edit->setText(image101FileName);
    checkPathesSet();
}

void PlotsDialog::on_image201Browse_clicked()
{
    QString initialName = ui->image201Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image201FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image201FileName = QDir::toNativeSeparators(image201FileName);

    ui->image201Edit->setText(image201FileName);
    checkPathesSet();
}

void PlotsDialog::on_image301Browse_clicked()
{
    QString initialName = ui->image301Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image301FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image301FileName = QDir::toNativeSeparators(image301FileName);

    ui->image301Edit->setText(image301FileName);
    checkPathesSet();
}

void PlotsDialog::on_image401Browse_clicked()
{
    QString initialName = ui->image401Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image401FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image401FileName = QDir::toNativeSeparators(image401FileName);

    ui->image401Edit->setText(image401FileName);
    checkPathesSet();
}

void PlotsDialog::on_image501Browse_clicked()
{
    QString initialName = ui->image501Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image501FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image501FileName = QDir::toNativeSeparators(image501FileName);

    ui->image501Edit->setText(image501FileName);
    checkPathesSet();
}

void PlotsDialog::on_image601Browse_clicked()
{
    QString initialName = ui->image601Edit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    image601FileName = QFileDialog::getOpenFileName(this, tr("Choose File"), initialName);
    image601FileName = QDir::toNativeSeparators(image601FileName);

    ui->image601Edit->setText(image601FileName);
    checkPathesSet();
}

void PlotsDialog::on_predictionBrowse_clicked()
{
    QString initialName = ui->predictionEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    predictionDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    predictionDirectoryName = QDir::toNativeSeparators(predictionDirectoryName);
    predictionDirectoryName.append("/");

    ui->predictionEdit->setText(predictionDirectoryName);
    checkPathesSet();
}

void PlotsDialog::on_cloudyPlotsBrowse_clicked()
{
    QString initialName = ui->cloudyPlotsEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    cloudyPlotsDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    cloudyPlotsDirectoryName = QDir::toNativeSeparators(cloudyPlotsDirectoryName);
    cloudyPlotsDirectoryName.append("/");

    ui->cloudyPlotsEdit->setText(cloudyPlotsDirectoryName);
    checkPathesSet();
}

//Double Histogram
void PlotsDialog::on_profilesBrowse_clicked()
{
    QString initialName = ui->profilesEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    profilesDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    profilesDirectoryName = QDir::toNativeSeparators(profilesDirectoryName);
    profilesDirectoryName.append("/");

    ui->profilesEdit->setText(profilesDirectoryName);
    checkPathesSet();
}

void PlotsDialog::on_histogramPlotsBrowse_clicked()
{
    QString initialName = ui->histogramPlotsEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    histogramPlotsDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    histogramPlotsDirectoryName = QDir::toNativeSeparators(histogramPlotsDirectoryName);
    histogramPlotsDirectoryName.append("/");

    ui->histogramPlotsEdit->setText(histogramPlotsDirectoryName);
    checkPathesSet();
}

void PlotsDialog::checkPathesSet()
{  
    if (!image100FileName.isEmpty() && !image200FileName.isEmpty() && !image300FileName.isEmpty() && !image400FileName.isEmpty() &&
        !image500FileName.isEmpty() && !image600FileName.isEmpty() && !image101FileName.isEmpty() && !image201FileName.isEmpty() &&
        !image301FileName.isEmpty() && !image401FileName.isEmpty() && !image501FileName.isEmpty() && !image601FileName.isEmpty() &&
        !predictionDirectoryName.isEmpty() && !cloudyPlotsDirectoryName.isEmpty() && !processRunning)
    {
        ui->cloudyBandsStartButton->setEnabled(true);
    }
    if (!profilesDirectoryName.isEmpty() && !histogramPlotsDirectoryName.isEmpty() && !processRunning)
    {
        ui->doubleHistogramStartButton->setEnabled(true);
    }
}

void PlotsDialog::errorPrediction()
{
    QString text = ui->outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(predictionDirectoryName);
    text.append(suffix);
    text.append( "does not exist. Check suffix setting!\n");
    ui->outputTextEdit->setPlainText(text);
    QTextCursor c =  ui->outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->outputTextEdit->setTextCursor(c);
    ui->outputTextEdit->ensureCursorVisible();
}

/*******************
Initialize processes
*******************/

void PlotsDialog::disableAll()
{
    ui->cloudyBandsStartButton->setEnabled(false);
    ui->doubleHistogramStartButton->setEnabled(false);
}

void PlotsDialog::cloudyBands()
{
    ui->outputTextEdit->clear();
    bool error_prediction=false;

    if (suffix!="no")
    {
        //input folders, error if not existing
        QString subFolder = predictionDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prediction=true;

        //output folders, create if not existing
        QDir dir(cloudyPlotsDirectoryName);
        subFolder = cloudyPlotsDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) dir.mkdir(suffix.toAscii().data());
    }

    if (error_prediction) errorPrediction();

    if (!error_prediction)
    {
        disableAll();

        QStringList args;
        args << "-cloudy-bands" << getFilename(image100FileName) << getFilename(image200FileName) << getFilename(image300FileName) <<
            getFilename(image400FileName) << getFilename(image500FileName) << getFilename(image600FileName) << getFilename(image101FileName) <<
            getFilename(image201FileName) << getFilename(image301FileName) << getFilename(image401FileName) << getFilename(image501FileName) <<
            getFilename(image601FileName) << predictionDirectoryName << cloudyPlotsDirectoryName << suffix;

        noNewLine=false;
        processRunning=true;

        if (fileExists("IceGrain"))
            process.start("./IceGrain", args);
        else if (directoryExists("processdialog.app"))
            process.start("./processdialog.app/Contents/MacOS/IceGrain", args);
        else process.start("./../IceGrain/IceGrain", args);
    }
}

void PlotsDialog::doubleHistogram()
{
    ui->outputTextEdit->clear();
    disableAll();

    QStringList args;
    args << "-double-grain-size" << ui->lowGrainSizeBox->text() << ui->minBubbleDistanceBox->text() << ui->comboBox->currentText() <<
        ui->comboBox_2->currentText() << profilesDirectoryName << histogramPlotsDirectoryName;

    noNewLine=false;
    processRunning=true;

    if (fileExists("IceGrain"))
        process.start("./IceGrain", args);
    else if (directoryExists("processdialog.app"))
        process.start("./processdialog.app/Contents/MacOS/IceGrain", args);
    else process.start("./../IceGrain/IceGrain", args);
}

/***************
Process handling
***************/

//Process standard output during runtime
void PlotsDialog::readProcessStdout()
{
    if (noNewLine)//if last output has not ended with new line, assume carriage return
    {
        ui->outputTextEdit->moveCursor( QTextCursor::End, QTextCursor::MoveAnchor );
        ui->outputTextEdit->moveCursor( QTextCursor::StartOfLine, QTextCursor::MoveAnchor );
        ui->outputTextEdit->moveCursor( QTextCursor::End, QTextCursor::KeepAnchor );
        ui->outputTextEdit->textCursor().removeSelectedText();
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
    text = ui->outputTextEdit->toPlainText() + QString::fromLocal8Bit(newData);
    ui->outputTextEdit->setPlainText(text);
    QTextCursor c =  ui->outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->outputTextEdit->setTextCursor(c);
    ui->outputTextEdit->ensureCursorVisible();
}

//Process standard error during runtime
void PlotsDialog::readProcessError()
{
    QByteArray newData = process.readAllStandardError();
    QString text = ui->outputTextEdit->toPlainText() + QString::fromLocal8Bit(newData);
    ui->outputTextEdit->setPlainText(text);
    QTextCursor c =  ui->outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->outputTextEdit->setTextCursor(c);
    ui->outputTextEdit->ensureCursorVisible();
}

//Process finished
void PlotsDialog::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit)
    {
        processRunning=false;
        ui->outputTextEdit->append(tr("Process crashed or killed by user."));
        QTextCursor c =  ui->outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        ui->outputTextEdit->setTextCursor(c);
        ui->outputTextEdit->ensureCursorVisible();
    }
    else if (exitCode != 0)
    {
        processRunning=false;
        ui->outputTextEdit->append(tr("Process failed."));
        QTextCursor c =  ui->outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        ui->outputTextEdit->setTextCursor(c);
        ui->outputTextEdit->ensureCursorVisible();
    }
    else
    {
        processRunning=false;
        ui->outputTextEdit->append(tr("Process succeeded."));
        QTextCursor c =  ui->outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        ui->outputTextEdit->setTextCursor(c);
        ui->outputTextEdit->ensureCursorVisible();
    }
    checkPathesSet();
}

//Process failed to start
void PlotsDialog::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart)
    {
        processRunning=false;
        ui->outputTextEdit->append(tr("Process failed to start."));
        QTextCursor c =  ui->outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        ui->outputTextEdit->setTextCursor(c);
        ui->outputTextEdit->ensureCursorVisible();
    }
    checkPathesSet();
}

//Kill running process
void PlotsDialog::killProcess()
{
    process.close();
}

/************
Path functions
*************/

QString PlotsDialog::getPath(QString pathAndFilename)
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

QString PlotsDialog::getFilename(QString pathAndFilename)
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

bool PlotsDialog::fileExists(const char* filename)
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

bool PlotsDialog::directoryExists(const char* pzPath )
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
