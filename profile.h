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
#ifndef PROFILE_H
#define PROFILE_H

#include <QtGui>
#include <sstream>

/*Structure of data (as of 08/10/2012)
  Naturally, new settings will be loaded with empty or default values if settings are loaded from legacy settings files.
  Index Variable
  0     [profile_name]    //Profile Name [profile_name] = active profile, ]profile_name[ = non-active profile
  1     #processdialog#   //IGNORE***********************************
  2     mosaicEdit
  3     relativeEdit
  4     stitchedEdit
  5     batchListEdit
  6     originalEdit
  7     preprocessedEdit
  8     parametersEdit
  9     pixelFeaturesEdit
  10    probMapEdit
  11    segmentationEdit
  12    boundaryFeaturesEdit
  13    0                    //Empty, so I suppose it is free to be used
  14    grainStepBox
  15    minBubbleDistance
  16    lowGrainSizeBox
  17    highGrainSizeBox
  18    grainSizeStepBox
  19    predictionEdit
  20    plotsEdit
  21    profilesEdit
  22    suffixCheckbox
  23    nrDepthsBox
  24    screenshotsEdit
  25    startValueEdit
  26    analyzeListEdit
  27    screenshotsEdit2
  28    paramComboBox1
  29    paramSpinBox
  30    paramBomboBox2
  31    corrComboBox1
  32    corrSpinBox
  33    corrComboBox2
  34    depthBinWidthBox
  35    thumbsCheckBox
  36    thumbsEdit
  37    #labeldialog#     //IGNORE***********************************
  38    originalEdit
  39    preprocessedEdit
  40    pixelLabelsEdit
  41    pixelClassificationEdit
  42    pixelRfEdit
  43    pixelOverwriteCheckBox
  44    segmentationEdit
  45    boundaryLabelsEdit
  46    boundaryFeaturesEdit
  47    boundaryClassificationEdit
  48    boundaryRfEdit
  49    boundaryOverwriteCheckBox
  50    pixelRfNameEdit
  51    boundaryRfNameEdit
  52    pixelProbEdit
  53    suffixCheckBox
  54    #plotsdialog#     //IGNORE***********************************
  55    image100Edit
  56    image200Edit
  57    image300Edit
  58    image400Edit
  59    image500Edit
  60    image600Edit
  61    image101Edit
  62    image201Edit
  63    image301Edit
  64    image401Edit
  65    image501Edit
  66    image601Edit
  67    predictionEdit
  68    cloudyPlotsEdit
  69    profilesEdit
  70    histogramPlotsEdit
  71    lowGrainSizeBox
  72    minBubbleDistanceBox
  73    comboBox
  74    comboBox_2
  75    defRfCheckBox              //NEW
  76    defRfCheckBox0             //NEW
*/

class Profile
{
public:
    //Static attributes
    static int numberOfProfiles;        //Number of available profiles
    static int numberOfSettings;        //Total number of settings that are administrated by the profiles
    static int activeProfileIndex;      //The index of the currently used profile in the profiles vector (see globals.h)
    static Profile unmodifiedProfile;   //The unmodified version of the currently used profile. Used to check if the profile has been changed.

    //Non-static attributes
    QString name;                       //Profile name
    std::vector<QString> data;          //The settings that are administrated by the profile (data.size() = numberOfSettings)
    int index;                          //This profile's index in the profiles vector (see globals.h)

    //Constructors
    Profile();                          //Standard constructor
    Profile(QString name);              //Constructor with profile name

    //Methods
    QString getName() { return this->name; }                //Gets the profile name
    void setName(QString newName) { this->name = newName; } //Sets the profile name

    std::vector<QString> getData() { return this->data; }                   //Gets the settings
    void setData(std::vector<QString> newData) { this->data = newData; }    //Sets the settings

    //Satic methods
    static std::string convertInt(int number);  //Converts an integer into a string using stringstreams

};

#endif // PROFILE_H
