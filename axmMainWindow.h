/*=========================================================================

  Name:        axmMainWindow.h

  Author:      David Borland

  Description: The main window for the Archimedes program.

=========================================================================*/


#ifndef AXMMAINWINDOW_H
#define AXMMAINWINDOW_H


#include <qmainwindow.h>

#include "ui_axmMainWindow.h"

class axmGLWidget;
class axmQSliderHelper;


class axmMainWindow : public QMainWindow, private Ui_MainWindow {
    Q_OBJECT

public:
    // Constructor/destructor
    axmMainWindow(QWidget* parent = NULL);
    virtual ~axmMainWindow();

public slots:
    // Use Qt's auto-connect magic to tie GUI widgets to slots,
    // removing the need to call connect() explicitly.
    // Names of the methods must follow the naming convention
    // on_<widget name>_<signal name>(<signal parameters>).

    // Sliders
    virtual void on_probeDensitySlider_valueChanged(int value);
    virtual void on_probeDensityLineEdit_editingFinished();

    virtual void on_probeWidthSlider_valueChanged(int value);
    virtual void on_probeWidthLineEdit_editingFinished();

    virtual void on_liquidDensitySlider_valueChanged(int value);
    virtual void on_liquidDensityLineEdit_editingFinished();

    virtual void on_forceCorrectionSlider_valueChanged(int value);
    virtual void on_forceCorrectionLineEdit_editingFinished(); 
 
    virtual void on_constrainStiffnessSlider_valueChanged(int value);
    virtual void on_constrainStiffnessLineEdit_editingFinished();

    // Radio buttons
    virtual void on_customProbeDensityRadioButton_toggled(bool checked);
    virtual void on_brickProbeDensityRadioButton_toggled(bool checked);
    virtual void on_iceProbeDensityRadioButton_toggled(bool checked);
    virtual void on_corkProbeDensityRadioButton_toggled(bool checked);

    virtual void on_customLiquidDensityRadioButton_toggled(bool checked);
    virtual void on_citricAcidLiquidDensityRadioButton_toggled(bool checked);
    virtual void on_waterLiquidDensityRadioButton_toggled(bool checked);
    virtual void on_gasolineLiquidDensityRadioButton_toggled(bool checked);

    // Check boxes
    virtual void on_hapticsCheckBox_toggled(bool checked);
    virtual void on_forceVisualizationCheckBox_toggled(bool checked);
    virtual void on_constrainXCheckBox_toggled(bool checked);
    virtual void on_constrainZCheckBox_toggled(bool checked);

    // Push buttons
    virtual void on_saveConfigurationButton_clicked();

protected:
    axmGLWidget* glWidget;

    // Helpers for using QSliders for continuous values
    axmQSliderHelper* probeDensitySliderHelper;
    axmQSliderHelper* probeWidthSliderHelper;
    axmQSliderHelper* liquidDensitySliderHelper;
    axmQSliderHelper* forceCorrectionSliderHelper;
    axmQSliderHelper* constrainStiffnessSliderHelper;

    void RefreshGUI();

    void UpdateMassLabel();
    void UpdateVolumeLabel();
};


#endif
