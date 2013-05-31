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
#include "labeldialog.h"
#include <dirent.h>

#include "globals.h"
#include "profile.h"

LabelDialog::LabelDialog(QWidget *parent): QDialog(parent), ui(new Ui::LabelDialog)
{
    ui->setupUi(this);

    disableAll();
    ui->pixelOverwriteCheckBox->setChecked(false);
    ui->boundaryOverwriteCheckBox->setChecked(false);
    ui->suffixCheckBox->setChecked(false);
    processRunning=false;
    lhidePaths = false;

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(refreshSelectedImages(int)));
    connect(ui->lhidePathsCheckBox, SIGNAL(clicked()), this, SLOT(lhide_Paths()));

    connect(ui->killProcessButton, SIGNAL(clicked()), this, SLOT(killProcess()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(saveSettings()));

    connect(ui->pixelRfNameEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePixelRfName()));
    connect(ui->boundaryRfNameEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBoundaryRfName()));
    connect(ui->originalEdit, SIGNAL(textChanged(QString)), this, SLOT(updateImageDirectoryName()));
    connect(ui->preprocessedEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePreprocessedDirectoryName()));
    connect(ui->pixelLabelsEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePixelLabelsDirectoryName()));
    connect(ui->pixelClassificationEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePixelClassificationDirectoryName()));
    connect(ui->pixelRfEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePixelRfDirectoryName()));
    connect(ui->pixelProbEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePixelProbDirectoryName()));
    connect(ui->segmentationEdit, SIGNAL(textChanged(QString)), this, SLOT(updateSegmentationDirectoryName()));
    connect(ui->boundaryLabelsEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBoundaryLabelsDirectoryName()));
    connect(ui->boundaryFeaturesEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBoundaryFeaturesDirectoryName()));
    connect(ui->boundaryClassificationEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBoundaryClassificationDirectoryName()));
    connect(ui->boundaryRfEdit, SIGNAL(textChanged(QString)), this, SLOT(updateBoundaryRfDirectoryName()));

    connect(ui->suffixCheckBox, SIGNAL(clicked()), this, SLOT(setSuffix()));
    connect(ui->pixelLabelStartButton, SIGNAL(clicked()), this, SLOT(pixelLabel()));
    connect(ui->pixelRfStartButton, SIGNAL(clicked()), this, SLOT(pixelRf()));
    connect(ui->boundaryLabelStartButton, SIGNAL(clicked()), this, SLOT(boundaryLabel()));
    connect(ui->boundaryRfStartButton, SIGNAL(clicked()), this, SLOT(boundaryRf()));

    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessStdout()));
    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(readProcessError()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));   
}

/********
Load settings from global data structures
********/
void LabelDialog::loadSettings()
{
    //Load last settings
    std::string emptystring = "";
    imageDirectoryName=profiles[Profile::activeProfileIndex].data[38];
    ui->originalEdit->setText(imageDirectoryName);
    preprocessedDirectoryName=profiles[Profile::activeProfileIndex].data[39];
    ui->preprocessedEdit->setText(preprocessedDirectoryName);
    pixelLabelsDirectoryName=profiles[Profile::activeProfileIndex].data[40];
    ui->pixelLabelsEdit->setText(pixelLabelsDirectoryName);
    pixelClassificationDirectoryName=profiles[Profile::activeProfileIndex].data[41];
    ui->pixelClassificationEdit->setText(pixelClassificationDirectoryName);
    pixelRfDirectoryName=profiles[Profile::activeProfileIndex].data[42];
    ui->pixelRfEdit->setText(pixelRfDirectoryName);
    if(profiles[Profile::activeProfileIndex].data[43].toStdString().compare(emptystring) != 0)
    {
        if (atoi(profiles[Profile::activeProfileIndex].data[43].toLocal8Bit().constData())==1) ui->pixelOverwriteCheckBox->setChecked(true);
        else ui->pixelOverwriteCheckBox->setChecked(false);
    }
    segmentationDirectoryName=profiles[Profile::activeProfileIndex].data[44];
    ui->segmentationEdit->setText(segmentationDirectoryName);
    boundaryLabelsDirectoryName=profiles[Profile::activeProfileIndex].data[45];
    ui->boundaryLabelsEdit->setText(boundaryLabelsDirectoryName);
    boundaryFeaturesDirectoryName=profiles[Profile::activeProfileIndex].data[46];
    ui->boundaryFeaturesEdit->setText(boundaryFeaturesDirectoryName);
    boundaryClassificationDirectoryName=profiles[Profile::activeProfileIndex].data[47];
    ui->boundaryClassificationEdit->setText(boundaryClassificationDirectoryName);
    boundaryRfDirectoryName=profiles[Profile::activeProfileIndex].data[48];
    labelRfDir = boundaryRfDirectoryName;
    ui->boundaryRfEdit->setText(boundaryRfDirectoryName);
    if(profiles[Profile::activeProfileIndex].data[49].toStdString().compare(emptystring) != 0)
    {
        if (atoi(profiles[Profile::activeProfileIndex].data[49].toLocal8Bit().constData())==1) ui->boundaryOverwriteCheckBox->setChecked(true);
        else ui->boundaryOverwriteCheckBox->setChecked(false);
    }
    pixelRfName=profiles[Profile::activeProfileIndex].data[50];
    ui->pixelRfNameEdit->setText(pixelRfName);
    boundaryRfName=profiles[Profile::activeProfileIndex].data[51];
    labelRfName = boundaryRfName;
    ui->boundaryRfNameEdit->setText(boundaryRfName);
    pixelProbDirectoryName=profiles[Profile::activeProfileIndex].data[52];
    ui->pixelProbEdit->setText(pixelProbDirectoryName);
    if(profiles[Profile::activeProfileIndex].data[53].toStdString().compare(emptystring) != 0)
    {
        if (atoi(profiles[Profile::activeProfileIndex].data[53].toLocal8Bit().constData())==1) ui->suffixCheckBox->setChecked(true);
        else
        {
            suffix="no";
            ui->suffixCheckBox->setChecked(false);
        }
        setSuffix();
    }

    checkPathesSet();    

    parametersFileName=profiles[Profile::activeProfileIndex].data[8];

}

/*********
Load settings from legacy file
*********/

void LabelDialog::loadSettingsLegacy()
{
    std::string pathSettings = (QDir::homePath()).toAscii().data();
    pathSettings.append("/.labeldialog");
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

        imageDirectoryName=settings.at(0).toLocal8Bit().constData();
        ui->originalEdit->setText(imageDirectoryName);
        preprocessedDirectoryName=settings.at(1).toLocal8Bit().constData();
        ui->preprocessedEdit->setText(preprocessedDirectoryName);
        pixelLabelsDirectoryName=settings.at(2).toLocal8Bit().constData();
        ui->pixelLabelsEdit->setText(pixelLabelsDirectoryName);
        pixelClassificationDirectoryName=settings.at(3).toLocal8Bit().constData();
        ui->pixelClassificationEdit->setText(pixelClassificationDirectoryName);
        pixelRfDirectoryName=settings.at(4).toLocal8Bit().constData();
        ui->pixelRfEdit->setText(pixelRfDirectoryName);
        if (atoi(settings.at(5).toLocal8Bit().constData())==1) ui->pixelOverwriteCheckBox->setChecked(true);
        segmentationDirectoryName=settings.at(6).toLocal8Bit().constData();
        ui->segmentationEdit->setText(segmentationDirectoryName);
        boundaryLabelsDirectoryName=settings.at(7).toLocal8Bit().constData();
        ui->boundaryLabelsEdit->setText(boundaryLabelsDirectoryName);
        boundaryFeaturesDirectoryName=settings.at(8).toLocal8Bit().constData();
        ui->boundaryFeaturesEdit->setText(boundaryFeaturesDirectoryName);
        boundaryClassificationDirectoryName=settings.at(9).toLocal8Bit().constData();
        ui->boundaryClassificationEdit->setText(boundaryClassificationDirectoryName);
        boundaryRfDirectoryName=settings.at(10).toLocal8Bit().constData();
        labelRfDir = boundaryRfDirectoryName;
        ui->boundaryRfEdit->setText(boundaryRfDirectoryName);
        if (atoi(settings.at(11).toLocal8Bit().constData())==1) ui->boundaryOverwriteCheckBox->setChecked(true);
        pixelRfName=settings.at(12).toLocal8Bit().constData();
        ui->pixelRfNameEdit->setText(pixelRfName);
        boundaryRfName=settings.at(13).toLocal8Bit().constData();
        labelRfName = boundaryRfName;
        ui->boundaryRfNameEdit->setText(boundaryRfName);
        if (settings.size()>14)
        {
            pixelProbDirectoryName=settings.at(14).toLocal8Bit().constData();
            ui->pixelProbEdit->setText(pixelProbDirectoryName);
        }
        if (settings.size()>15)
        {
            if (atoi(settings.at(15).toLocal8Bit().constData())==1) ui->suffixCheckBox->setChecked(true);
            else suffix="no";
            setSuffix();
        }
        temp_settings_file.close();
        settings_file.close();
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
        temp_settings_file2.close();
        settings_file2.close();
    }

    profiles[Profile::activeProfileIndex].data[38] = imageDirectoryName;
    profiles[Profile::activeProfileIndex].data[39] = preprocessedDirectoryName;
    profiles[Profile::activeProfileIndex].data[40] = pixelLabelsDirectoryName;
    profiles[Profile::activeProfileIndex].data[41] = pixelClassificationDirectoryName;
    profiles[Profile::activeProfileIndex].data[42] = pixelRfDirectoryName;
    if(ui->pixelOverwriteCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[43] = "1";
    else profiles[Profile::activeProfileIndex].data[43] = "0";
    profiles[Profile::activeProfileIndex].data[44] = segmentationDirectoryName;
    profiles[Profile::activeProfileIndex].data[45] = boundaryLabelsDirectoryName;
    profiles[Profile::activeProfileIndex].data[46] = boundaryFeaturesDirectoryName;
    profiles[Profile::activeProfileIndex].data[47] = boundaryClassificationDirectoryName;
    profiles[Profile::activeProfileIndex].data[48] = boundaryRfDirectoryName;
    if(ui->boundaryOverwriteCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[49] = "1";
    else profiles[Profile::activeProfileIndex].data[49] = "0";
    profiles[Profile::activeProfileIndex].data[50] = pixelRfName;
    profiles[Profile::activeProfileIndex].data[51] = boundaryRfName;
    profiles[Profile::activeProfileIndex].data[52] = pixelProbDirectoryName;
    if(ui->suffixCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[53] = "1";
    else profiles[Profile::activeProfileIndex].data[53] = "0";
}

/*********
Set pathes
*********/

void LabelDialog::saveSettings()
{
    //Update ChecBox values
    if(!ui->pixelOverwriteCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[43] = "0";
    else profiles[Profile::activeProfileIndex].data[43] = "1";
    if(!ui->boundaryOverwriteCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[49] = "0";
    else profiles[Profile::activeProfileIndex].data[49] = "1";
    if(!ui->suffixCheckBox->isChecked()) profiles[Profile::activeProfileIndex].data[53] = "0";
    else profiles[Profile::activeProfileIndex].data[53] = "1";

    reject();
}

/**************
checkDirEmpty: Checks if the QString returned by the 'getOpenFileName' function is empty. If that is the case use the old
QString as the new directory.
Variables:
- initialName: the original directory QString
- newDir:      the QString returned by 'getOpenFileName'
- target:      QLineEdit widget that will be overwritten
***************/
void LabelDialog::checkDirEmpty(QString initialName, QString &newDir, QLineEdit &target)
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

void LabelDialog::updatePixelRfName()
{
    pixelRfName = ui->pixelRfNameEdit->text();
    profiles[Profile::activeProfileIndex].data[50] = pixelRfName;
    checkPathesSet();
}

void LabelDialog::updateBoundaryRfName()
{
    boundaryRfName = ui->boundaryRfNameEdit->text();
    labelRfName = boundaryRfName;    
    profiles[Profile::activeProfileIndex].data[51] = labelRfName;
    checkPathesSet();
}

void LabelDialog::updateImageDirectoryName()
{
    imageDirectoryName = ui->originalEdit->text();
    profiles[Profile::activeProfileIndex].data[38] = imageDirectoryName;
    checkPathesSet();
}

void LabelDialog::updatePreprocessedDirectoryName()
{
    preprocessedDirectoryName = ui->preprocessedEdit->text();
    profiles[Profile::activeProfileIndex].data[39] = preprocessedDirectoryName;
    checkPathesSet();
}

void LabelDialog::updatePixelLabelsDirectoryName()
{
    pixelLabelsDirectoryName = ui->pixelLabelsEdit->text();
    profiles[Profile::activeProfileIndex].data[40] = pixelLabelsDirectoryName;
    checkPathesSet();
}

void LabelDialog::updatePixelClassificationDirectoryName()
{
    pixelClassificationDirectoryName = ui->pixelClassificationEdit->text();
    profiles[Profile::activeProfileIndex].data[41] = pixelClassificationDirectoryName;
    checkPathesSet();
}

void LabelDialog::updatePixelRfDirectoryName()
{
    pixelRfDirectoryName = ui->pixelRfEdit->text();
    profiles[Profile::activeProfileIndex].data[42] = pixelRfDirectoryName;
    checkPathesSet();
}

void LabelDialog::updatePixelProbDirectoryName()
{
    pixelProbDirectoryName = ui->pixelProbEdit->text();
    profiles[Profile::activeProfileIndex].data[52] = pixelProbDirectoryName;
    checkPathesSet();
}

void LabelDialog::updateSegmentationDirectoryName()
{
    segmentationDirectoryName = ui->segmentationEdit->text();
    profiles[Profile::activeProfileIndex].data[44] = segmentationDirectoryName;
    checkPathesSet();
}

void LabelDialog::updateBoundaryLabelsDirectoryName()
{
    boundaryLabelsDirectoryName = ui->boundaryLabelsEdit->text();
    profiles[Profile::activeProfileIndex].data[45] = boundaryLabelsDirectoryName;
    checkPathesSet();
}

void LabelDialog::updateBoundaryFeaturesDirectoryName()
{
    boundaryFeaturesDirectoryName = ui->boundaryFeaturesEdit->text();
    profiles[Profile::activeProfileIndex].data[46] = boundaryFeaturesDirectoryName;
    checkPathesSet();
}

void LabelDialog::updateBoundaryClassificationDirectoryName()
{
    boundaryClassificationDirectoryName = ui->boundaryClassificationEdit->text();
    profiles[Profile::activeProfileIndex].data[47] = boundaryClassificationDirectoryName;
    checkPathesSet();
}

void LabelDialog::updateBoundaryRfDirectoryName()
{
    boundaryRfDirectoryName = ui->boundaryRfEdit->text();
    labelRfDir = boundaryRfDirectoryName;
    profiles[Profile::activeProfileIndex].data[48] = boundaryRfDirectoryName;
    checkPathesSet();
}

void LabelDialog::setSuffix()
{
    if (!ui->suffixCheckBox->isChecked())
    {
        suffix="no";
        ui->suffixLabel->setText("");
        profiles[Profile::activeProfileIndex].data[53] = "0";
    }
    else
    {
        QString getSuffix = imageDirectoryName;
        getSuffix.resize(getSuffix.size()-1);
        suffix=getFilename(getSuffix);
        suffix.append("/");
        QString text = suffix;
        ui->suffixLabel->setText(text);
        profiles[Profile::activeProfileIndex].data[53] = "1";
    }
}

//Pixel Labeling
void LabelDialog::on_originalBrowse_clicked()
{
    QString initialName = ui->originalEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    imageDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    imageDirectoryName = QDir::toNativeSeparators(imageDirectoryName);
    imageDirectoryName.append("/");

    checkDirEmpty(initialName, imageDirectoryName, *ui->originalEdit);
    setSuffix();
}

void LabelDialog::on_preprocessedBrowse_clicked()
{
    QString initialName = ui->preprocessedEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    preprocessedDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    preprocessedDirectoryName = QDir::toNativeSeparators(preprocessedDirectoryName);
    preprocessedDirectoryName.append("/");

    checkDirEmpty(initialName, preprocessedDirectoryName, *ui->preprocessedEdit);
}

void LabelDialog::on_pixelLabelsBrowse_clicked()
{
    QString initialName = ui->pixelLabelsEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    pixelLabelsDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    pixelLabelsDirectoryName = QDir::toNativeSeparators(pixelLabelsDirectoryName);
    pixelLabelsDirectoryName.append("/");

    checkDirEmpty(initialName, pixelLabelsDirectoryName, *ui->pixelLabelsEdit);
}

void LabelDialog::on_pixelClassificationBrowse_clicked()
{
    QString initialName = ui->pixelClassificationEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    pixelClassificationDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    pixelClassificationDirectoryName = QDir::toNativeSeparators(pixelClassificationDirectoryName);
    pixelClassificationDirectoryName.append("/");

    checkDirEmpty(initialName, pixelClassificationDirectoryName, *ui->pixelClassificationEdit);
}

void LabelDialog::on_pixelRfBrowse_clicked()
{
    QString initialName = ui->pixelRfEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    pixelRfDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    pixelRfDirectoryName = QDir::toNativeSeparators(pixelRfDirectoryName);
    pixelRfDirectoryName.append("/");

    checkDirEmpty(initialName, pixelRfDirectoryName, *ui->pixelRfEdit);
}

void LabelDialog::on_pixelProbBrowse_clicked()
{
    QString initialName = ui->pixelProbEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    pixelProbDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    pixelProbDirectoryName = QDir::toNativeSeparators(pixelProbDirectoryName);
    pixelProbDirectoryName.append("/");

    checkDirEmpty(initialName, pixelProbDirectoryName, *ui->pixelProbEdit);
}

void LabelDialog::on_imageBrowse_clicked()
{
    QString initialName = imageDirectoryName;
    if (imageDirectoryName.isEmpty())
        imageDirectoryName = QDir::homePath();
    selectedImages = QFileDialog::getOpenFileNames(this, tr("Choose Files"), initialName);

    if(!selectedImages.isEmpty()) imageDirectoryName = getPath(selectedImages.at(0).toLocal8Bit().constData());
    ui->originalEdit->setText(imageDirectoryName);

    QString text = QString("%1").arg(selectedImages.size());
    text.append(" image(s) selected");
    ui->imagesSelectedLabel->setText(text);
    checkPathesSet();
    setSuffix();

    refreshSelectedImages(0);
}

//Boundary Labeling
void LabelDialog::on_segmentationBrowse_clicked()
{
    QString initialName = ui->segmentationEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    segmentationDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    segmentationDirectoryName = QDir::toNativeSeparators(segmentationDirectoryName);
    segmentationDirectoryName.append("/");

    checkDirEmpty(initialName, segmentationDirectoryName, *ui->segmentationEdit);
}

void LabelDialog::on_boundaryLabelsBrowse_clicked()
{
    QString initialName = ui->boundaryLabelsEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    boundaryLabelsDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    boundaryLabelsDirectoryName = QDir::toNativeSeparators(boundaryLabelsDirectoryName);
    boundaryLabelsDirectoryName.append("/");

    checkDirEmpty(initialName, boundaryLabelsDirectoryName, *ui->boundaryLabelsEdit);
}

void LabelDialog::on_boundaryFeaturesBrowse_clicked()
{
    QString initialName = ui->boundaryFeaturesEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    boundaryFeaturesDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    boundaryFeaturesDirectoryName = QDir::toNativeSeparators(boundaryFeaturesDirectoryName);
    boundaryFeaturesDirectoryName.append("/");

    checkDirEmpty(initialName, boundaryFeaturesDirectoryName, *ui->boundaryFeaturesEdit);
}

void LabelDialog::on_boundaryClassificationBrowse_clicked()
{
    QString initialName = ui->boundaryClassificationEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    boundaryClassificationDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    boundaryClassificationDirectoryName = QDir::toNativeSeparators(boundaryClassificationDirectoryName);
    boundaryClassificationDirectoryName.append("/");

    checkDirEmpty(initialName, boundaryClassificationDirectoryName, *ui->boundaryClassificationEdit);
}

void LabelDialog::on_boundaryRfBrowse_clicked()
{
    QString initialName = ui->boundaryRfEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    boundaryRfDirectoryName = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), initialName);
    boundaryRfDirectoryName = QDir::toNativeSeparators(boundaryRfDirectoryName);
    boundaryRfDirectoryName.append("/");

    checkDirEmpty(initialName, boundaryRfDirectoryName, *ui->boundaryRfEdit);
}

void LabelDialog::on_imageBrowse2_clicked()
{
    QString initialName = imageDirectoryName;
    if (imageDirectoryName.isEmpty())
        imageDirectoryName = QDir::homePath();
    selectedImages2 = QFileDialog::getOpenFileNames(this, tr("Choose Files"), initialName);

    if(!selectedImages2.isEmpty()) imageDirectoryName = getPath(selectedImages2.at(0).toLocal8Bit().constData());
    ui->originalEdit->setText(imageDirectoryName);

    QString text = QString("%1").arg(selectedImages2.size());
    text.append(" image(s) selected");
    ui->imagesSelectedLabel2->setText(text);
    checkPathesSet();

    refreshSelectedImages(1);
}

void LabelDialog::checkPathesSet()
{
    if (!selectedImages.isEmpty() && !preprocessedDirectoryName.isEmpty() && !pixelLabelsDirectoryName.isEmpty() &&
        !processRunning)
    {
        ui->pixelLabelStartButton->setEnabled(true);
    }
    if (!selectedImages.isEmpty() && !preprocessedDirectoryName.isEmpty() && !pixelLabelsDirectoryName.isEmpty() &&
        !pixelClassificationDirectoryName.isEmpty() && !pixelRfDirectoryName.isEmpty() && !pixelRfName.isEmpty() &&
        !processRunning)
    {
        ui->pixelRfStartButton->setEnabled(true);
    }
    if (!selectedImages2.isEmpty() && !segmentationDirectoryName.isEmpty() && !boundaryLabelsDirectoryName.isEmpty() &&
        !processRunning)
    {
        ui->boundaryLabelStartButton->setEnabled(true);
    }
    if (!selectedImages2.isEmpty() && !boundaryLabelsDirectoryName.isEmpty() && !boundaryFeaturesDirectoryName.isEmpty() &&
        !boundaryClassificationDirectoryName.isEmpty() && !boundaryRfDirectoryName.isEmpty() && !boundaryRfName.isEmpty() &&
        !processRunning)
    {
        ui->boundaryRfStartButton->setEnabled(true);
    }
}

void LabelDialog::errorPreprocessed()
{
    QString text = ui->outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(preprocessedDirectoryName);
    text.append(suffix);
    text.append( "does not exist. Check suffix setting!\n");
    ui->outputTextEdit->setPlainText(text);
    QTextCursor c =  ui->outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->outputTextEdit->setTextCursor(c);
    ui->outputTextEdit->ensureCursorVisible();
}

void LabelDialog::errorPixelProbability()
{
    QString text = ui->outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(pixelProbDirectoryName);
    text.append(suffix);
    text.append( "does not exist. Check suffix setting!\n");
    ui->outputTextEdit->setPlainText(text);
    QTextCursor c =  ui->outputTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->outputTextEdit->setTextCursor(c);
    ui->outputTextEdit->ensureCursorVisible();
}

void LabelDialog::errorSegmentation()
{
    QString text = ui->outputTextEdit->toPlainText();
    text.append("Process can not be started. The path ");
    text.append(segmentationDirectoryName);
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

void LabelDialog::disableAll()
{
    ui->pixelLabelStartButton->setEnabled(false);
    ui->pixelRfStartButton->setEnabled(false);
    ui->boundaryLabelStartButton->setEnabled(false);
    ui->boundaryRfStartButton->setEnabled(false);
}

void LabelDialog::pixelLabel()
{
    ui->outputTextEdit->clear();
    bool error_prob=false;
    bool error_segmentation=false;
    bool error_preprocessed=false;

    if (suffix!="no" && !pixelProbDirectoryName.isEmpty())
    {
        //input folders, error if not existing
        QString subFolder = pixelProbDirectoryName;
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_prob=true;

        QString subFolder2 = preprocessedDirectoryName;
        subFolder2.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_preprocessed=true;

        if(!segmentationDirectoryName.isEmpty())
        {
            //input folders, error if not existing
            QString subFolder = segmentationDirectoryName;
            subFolder.append(suffix.toAscii().data());
            if (!directoryExists(subFolder.toAscii().data())) error_segmentation=true;
        }
    }

    if (error_prob) errorPixelProbability();
    if (error_segmentation) errorSegmentation();
    if (error_preprocessed) errorPreprocessed();

    if (!error_prob && !error_segmentation && !error_preprocessed)
    {
        disableAll();

        QStringList args;
        if (pixelProbDirectoryName.isEmpty() || segmentationDirectoryName.isEmpty()) args << "-pixel-labels-gui" <<
            selectedImages << preprocessedDirectoryName << pixelLabelsDirectoryName << "no" << "no" << suffix << parametersFileName;
        else args << "-pixel-labels-gui" << selectedImages << preprocessedDirectoryName << pixelLabelsDirectoryName <<
            pixelProbDirectoryName << segmentationDirectoryName << suffix << parametersFileName;

        noNewLine=false;
        pixelClassificationRunning=false;
        boundaryClassificationRunning=false;
        processRunning=true;

        if (fileExists("cis"))
            process.start("./cis", args);
        else if (directoryExists("processdialog.app"))
            process.start("./processdialog.app/Contents/MacOS/cis", args);
        else process.start("./../CIS/cis", args);
    }
}
void LabelDialog::pixelRf()
{
    ui->outputTextEdit->clear();
    bool error_preprocessed=false;

    QString subFolder = preprocessedDirectoryName;

    if (suffix!="no")
    {
        subFolder.append(suffix.toAscii().data());
        if (!directoryExists(subFolder.toAscii().data())) error_preprocessed=true;
    }

    if (error_preprocessed) errorPreprocessed();

    if (!error_preprocessed)
    {
        disableAll();

        QString pixelOverwrite;
        if (ui->pixelOverwriteCheckBox->isChecked()) pixelOverwrite = "1";
        else pixelOverwrite = "0";

        QStringList args;
        args << "-pixel-classification-gui" << selectedImages << subFolder << pixelLabelsDirectoryName <<
            pixelClassificationDirectoryName << pixelRfName << parametersFileName << pixelOverwrite;

        noNewLine=false;
        pixelClassificationRunning=true;
        boundaryClassificationRunning=false;
        processRunning=true;

        if (fileExists("cis"))
            process.start("./cis", args);
        else if (directoryExists("processdialog.app"))
            process.start("./processdialog.app/Contents/MacOS/cis", args);
        else process.start("./../CIS/cis", args);
    }
}

void LabelDialog::boundaryLabel()
{
    disableAll();
    ui->outputTextEdit->clear();

    QStringList args;
    args << "-boundary-labels-gui" << selectedImages2 << segmentationDirectoryName << boundaryLabelsDirectoryName <<
        parametersFileName;

    noNewLine=false;
    pixelClassificationRunning=false;
    boundaryClassificationRunning=false;
    processRunning=true;

    if (fileExists("cis"))
        process.start("./cis", args);
    else if (directoryExists("processdialog.app"))
        process.start("./processdialog.app/Contents/MacOS/cis", args);
    else process.start("./../CIS/cis", args);
}

void LabelDialog::boundaryRf()
{
    disableAll();
    ui->outputTextEdit->clear();

    QString boundaryOverwrite;
    if (ui->boundaryOverwriteCheckBox->isChecked()) boundaryOverwrite = "1";
    else boundaryOverwrite = "0";

    QStringList args;
    args << "-boundary-classification-gui" << selectedImages2 << boundaryLabelsDirectoryName << boundaryFeaturesDirectoryName <<
        boundaryClassificationDirectoryName << boundaryRfName << boundaryOverwrite;

    noNewLine=false;
    pixelClassificationRunning=false;
    boundaryClassificationRunning=true;
    processRunning=true;

    if (fileExists("cis"))
        process.start("./cis", args);
    else if (directoryExists("processdialog.app"))
        process.start("./processdialog.app/Contents/MacOS/cis", args);
    else process.start("./../CIS/cis", args);
}

/***************
Process handling
***************/

//Process standard output during runtime
void LabelDialog::readProcessStdout()
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
void LabelDialog::readProcessError()
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
void LabelDialog::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit)
    {
        pixelClassificationRunning=false;
        boundaryClassificationRunning=false;
        processRunning=false;
        ui->outputTextEdit->append(tr("Process crashed or killed by user."));
        QTextCursor c =  ui->outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        ui->outputTextEdit->setTextCursor(c);
        ui->outputTextEdit->ensureCursorVisible();
    }
    else if (exitCode != 0)
    {
        pixelClassificationRunning=false;
        boundaryClassificationRunning=false;
        processRunning=false;
        ui->outputTextEdit->append(tr("Process failed."));
        QTextCursor c =  ui->outputTextEdit->textCursor();
        c.movePosition(QTextCursor::End);
        ui->outputTextEdit->setTextCursor(c);
        ui->outputTextEdit->ensureCursorVisible();
    }
    else
    {
        //Train pixel Rf after computation of classification set
        if(pixelClassificationRunning)
        {
            QStringList args;
            args << "-pixel-rf-train-gui" << pixelClassificationDirectoryName << pixelRfDirectoryName << pixelRfName << parametersFileName;

            pixelClassificationRunning=false;
            if (fileExists("cis"))
                process.start("./cis", args);
            else if (directoryExists("processdialog.app"))
                process.start("./processdialog.app/Contents/MacOS/cis", args);
            else process.start("./../CIS/cis", args);
        }
        //Train boundary Rf after computation of classification set
        else if(boundaryClassificationRunning)
        {
            QStringList args;
            args << "-boundary-rf-train-gui" << boundaryClassificationDirectoryName << boundaryRfDirectoryName << boundaryRfName << parametersFileName;

            boundaryClassificationRunning=false;
            if (fileExists("cis"))
                process.start("./cis", args);
            else if (directoryExists("processdialog.app"))
                process.start("./processdialog.app/Contents/MacOS/cis", args);
            else process.start("./../CIS/cis", args);
        }
        else
        {
            pixelClassificationRunning=false;
            boundaryClassificationRunning=false;
            processRunning=false;
            ui->outputTextEdit->append(tr("Process succeeded."));
            QTextCursor c =  ui->outputTextEdit->textCursor();
            c.movePosition(QTextCursor::End);
            ui->outputTextEdit->setTextCursor(c);
            ui->outputTextEdit->ensureCursorVisible();
        }
    }
    checkPathesSet();
}

//Process failed to start
void LabelDialog::processError(QProcess::ProcessError error)
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
void LabelDialog::killProcess()
{
    process.close();
}

/************
Path functions
*************/

QString LabelDialog::getPath(QString pathAndFilename)
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

QString LabelDialog::getFilename(QString pathAndFilename)
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

bool LabelDialog::fileExists(const char* filename)
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

bool LabelDialog::directoryExists(const char* pzPath )
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

void LabelDialog::refreshSelectedImages(int index)
{
    QStringList selImages;
    if(index == 0) //Pixel labels tab
    {
        ui->selectedImageslistWidget->clear();
        if(lhidePaths)
        {
            for(int i = 0; i < selectedImages.size(); i++)
            {
                QString tmp = getFilename(selectedImages[i]);
                selImages << tmp;
            }
            ui->selectedImageslistWidget->addItems(selImages);
        }
        else
        {
            ui->selectedImageslistWidget->addItems(selectedImages);
        }
    }
    if(index == 1) //Boundary labels tab
    {
        ui->selectedImageslistWidget->clear();
        if(lhidePaths)
        {
            for(int i = 0; i < selectedImages2.size(); i++)
            {
                QString tmp = getFilename(selectedImages2[i]);
                selImages << tmp;
            }
            ui->selectedImageslistWidget->addItems(selImages);
        }
        else
        {
            ui->selectedImageslistWidget->addItems(selectedImages2);
        }
    }    
}

void LabelDialog::lhide_Paths()
{
    if(ui->lhidePathsCheckBox->isChecked())
    {
        lhidePaths = true;
        refreshSelectedImages(ui->tabWidget->currentIndex());
    }
    else
    {
        lhidePaths = false;
        refreshSelectedImages(ui->tabWidget->currentIndex());
    }
}
