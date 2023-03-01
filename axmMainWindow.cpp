/*=========================================================================

  Name:        axmMainWindow.cpp

  Author:      David Borland

  Description: The main window for the Archimedes program.

=========================================================================*/


#include "axmMainWindow.h"

#include "axmGLWidget.h"
#include "axmQSliderHelper.h"

#include <qapplication.h>

#include <iostream>
//#include <math.h>


// Constructor
axmMainWindow::axmMainWindow(QWidget* parent) : QMainWindow(parent) {
    // Create the GUI from the Qt Designer file
    setupUi(this);

    // Set up the OpenGL widget
    glWidget = new axmGLWidget(this);
    glWidgetLayout->addWidget(glWidget);
   
    // Set up values for sliders
    double minProbeDensity = glWidget->GetMinProbeDensity();
    double maxProbeDensity = glWidget->GetMaxProbeDensity();

    double minProbeWidth = glWidget->GetMinProbeWidth();
    double maxProbeWidth = glWidget->GetMaxProbeWidth();

    double minLiquidDensity = glWidget->GetMinLiquidDensity();
    double maxLiquidDensity = glWidget->GetMaxLiquidDensity();

    double minForceCorrection = 0.0;
    double maxForceCorrection = 5.0;

    double minConstrainStiffness = 0.0;
    double maxConstrainStiffness = 10000.0;

    // Helpers for sliders
    probeDensitySliderHelper = new axmQSliderHelper(probeDensitySlider, probeDensityLineEdit,
                                                    minProbeDensity, maxProbeDensity, 0.1);
    
    probeWidthSliderHelper = new axmQSliderHelper(probeWidthSlider, probeWidthLineEdit,
                                                  minProbeWidth, maxProbeWidth, 0.25);

    liquidDensitySliderHelper = new axmQSliderHelper(liquidDensitySlider, liquidDensityLineEdit,
                                                     minLiquidDensity, maxLiquidDensity, 0.1);

    forceCorrectionSliderHelper = new axmQSliderHelper(forceCorrectionSlider, forceCorrectionLineEdit,
                                                       minForceCorrection, maxForceCorrection, 0.1);

    constrainStiffnessSliderHelper = new axmQSliderHelper(constrainStiffnessSlider, constrainStiffnessLineEdit,
                                                          minConstrainStiffness, maxConstrainStiffness, 100.0);

    // Set the GUI from the graphics
    RefreshGUI();
}

// Destructor
axmMainWindow::~axmMainWindow() {
    delete probeDensitySliderHelper;
    delete probeWidthSliderHelper;
    delete liquidDensitySliderHelper;
    delete forceCorrectionSliderHelper;
}


void axmMainWindow::on_probeDensitySlider_valueChanged(int value) {
    // Set the density
    double density = probeDensitySliderHelper->FromSlider();

    glWidget->SetProbeDensity(density);

    // Update the mass
    UpdateMassLabel();
}

void axmMainWindow::on_probeDensityLineEdit_editingFinished() {
    double density = probeDensitySliderHelper->ToSlider();

    glWidget->SetProbeDensity(density);
}

void axmMainWindow::on_probeWidthSlider_valueChanged(int value) {
    // Set the width
    double width = probeWidthSliderHelper->FromSlider();

    glWidget->SetProbeWidth(width);

    // Update the labels
    UpdateMassLabel();
    UpdateVolumeLabel();
}

void axmMainWindow::on_probeWidthLineEdit_editingFinished() {
    double width = probeWidthSliderHelper->ToSlider();

    glWidget->SetProbeWidth(width);
}

void axmMainWindow::on_liquidDensitySlider_valueChanged(int value) {
    double density = liquidDensitySliderHelper->FromSlider();

    glWidget->SetLiquidDensity(density);
}

void axmMainWindow::on_liquidDensityLineEdit_editingFinished() {
    double density = liquidDensitySliderHelper->ToSlider();

    glWidget->SetLiquidDensity(density);
}

void axmMainWindow::on_forceCorrectionSlider_valueChanged(int value) {
    double forceCorrection = forceCorrectionSliderHelper->FromSlider();

    glWidget->SetForceCorrection(forceCorrection);
}

void axmMainWindow::on_forceCorrectionLineEdit_editingFinished() {
    double forceCorrection = forceCorrectionSliderHelper->ToSlider();

    glWidget->SetForceCorrection(forceCorrection);
}

void axmMainWindow::on_constrainStiffnessSlider_valueChanged(int value) {
    double constrainZStiffness = constrainStiffnessSliderHelper->FromSlider();

    glWidget->SetConstrainStiffness(constrainZStiffness);
}

void axmMainWindow::on_constrainStiffnessLineEdit_editingFinished() {
    double constrainZStiffness = constrainStiffnessSliderHelper->ToSlider();
    
    glWidget->SetConstrainStiffness(constrainZStiffness);
}


void axmMainWindow::on_customProbeDensityRadioButton_toggled(bool checked) {
    if (checked) {
        probeDensitySlider->setEnabled(true);
        probeDensityLineEdit->setEnabled(true);
    }
}

void axmMainWindow::on_brickProbeDensityRadioButton_toggled(bool checked) {
    if (checked) {
        probeDensitySliderHelper->SetValue(1.922);

        probeDensitySlider->setEnabled(false);
        probeDensityLineEdit->setEnabled(false);
    }
}

void axmMainWindow::on_iceProbeDensityRadioButton_toggled(bool checked) {
    if (checked) {
        probeDensitySliderHelper->SetValue(0.919);

        probeDensitySlider->setEnabled(false);
        probeDensityLineEdit->setEnabled(false);
    }
}

void axmMainWindow::on_corkProbeDensityRadioButton_toggled(bool checked) {
    if (checked) {
        probeDensitySliderHelper->SetValue(0.24);

        probeDensitySlider->setEnabled(false);
        probeDensityLineEdit->setEnabled(false);
    }
}


void axmMainWindow::on_customLiquidDensityRadioButton_toggled(bool checked) {
    if (checked) {
        liquidDensitySlider->setEnabled(true);
        liquidDensityLineEdit->setEnabled(true);
    }
}

void axmMainWindow::on_citricAcidLiquidDensityRadioButton_toggled(bool checked) {
    if (checked) {
        liquidDensitySliderHelper->SetValue(1.65951);

        liquidDensitySlider->setEnabled(false);
        liquidDensityLineEdit->setEnabled(false);
    }
}

void axmMainWindow::on_waterLiquidDensityRadioButton_toggled(bool checked) {
    if (checked) {
        liquidDensitySliderHelper->SetValue(1.0);

        liquidDensitySlider->setEnabled(false);
        liquidDensityLineEdit->setEnabled(false);
    }
}

void axmMainWindow::on_gasolineLiquidDensityRadioButton_toggled(bool checked) {
    if (checked) {
        liquidDensitySliderHelper->SetValue(0.73722);

        liquidDensitySlider->setEnabled(false);
        liquidDensityLineEdit->setEnabled(false);
    }
}


void axmMainWindow::on_hapticsCheckBox_toggled(bool checked) {
    glWidget->EnableHaptics(checked);
}

void axmMainWindow::on_forceVisualizationCheckBox_toggled(bool checked) {
    glWidget->EnableForceVisualization(checked);
}

void axmMainWindow::on_constrainXCheckBox_toggled(bool checked) {
    glWidget->ConstrainX(checked);
}
void axmMainWindow::on_constrainZCheckBox_toggled(bool checked) {
    glWidget->ConstrainZ(checked);
}


void axmMainWindow::on_saveConfigurationButton_clicked() {
    glWidget->SaveConfigurationFile();
}


void axmMainWindow::RefreshGUI() {
    // Probe density
    probeDensitySliderHelper->SetValue(glWidget->GetProbeDensity());

    // Probe width
    probeWidthSliderHelper->SetValue(glWidget->GetProbeWidth());

    // Liquid density
    liquidDensitySliderHelper->SetValue(glWidget->GetLiquidDensity());

    // Advanced
    hapticsCheckBox->setChecked(glWidget->GetEnableHaptics());
    forceVisualizationCheckBox->setChecked(glWidget->GetEnableForceVisualization());
    forceCorrectionSliderHelper->SetValue(glWidget->GetForceCorrection());
    constrainXCheckBox->setChecked(glWidget->GetConstrainX());
    constrainZCheckBox->setChecked(glWidget->GetConstrainZ());
    constrainStiffnessSliderHelper->SetValue(glWidget->GetConstrainStiffness());
}


void axmMainWindow::UpdateMassLabel() {
    double density = probeDensitySliderHelper->FromSlider();
    double width = probeWidthSliderHelper->FromSlider();
    double volume = width * width * width;

    QString text = QString().sprintf("Mass: %0.2f g", density * volume);
    massLabel->setText(text);
}

void axmMainWindow::UpdateVolumeLabel() {
    double width = probeWidthSliderHelper->FromSlider();
    double volume = width * width * width;

    QString text = QString().sprintf("Volume: %0.2f cm^3", volume);
    volumeLabel->setText(text);
}