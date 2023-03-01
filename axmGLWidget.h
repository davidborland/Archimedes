/*=========================================================================

  Name:        axmGLWidget.h

  Author:      David Borland

  Description: Handles drawing to an OpenGL buffer.

=========================================================================*/


#ifndef AXMGLWIDGET_H
#define AXMGLWIDGET_H


#include <QGLWidget>

#include "axmGraphics.h"
#include "axmHaptics.h"


class axmGLWidget : public QGLWidget {
    Q_OBJECT

public:
    axmGLWidget(QWidget* parent);
    virtual ~axmGLWidget();

    // Get/set probe density
    double GetProbeDensity();
    void SetProbeDensity(double density);

    double GetMinProbeDensity();
    double GetMaxProbeDensity();

    // Get/set probe width
    double GetProbeWidth();
    void SetProbeWidth(double width);

    double GetMinProbeWidth();
    double GetMaxProbeWidth();

    // Get/set liquid density
    double GetLiquidDensity();
    void SetLiquidDensity(double density);

    double GetMinLiquidDensity();
    double GetMaxLiquidDensity();

    // Get/set force correction to account for weight of the haptic tool
    double GetForceCorrection();
    void SetForceCorrection(double force);

    // Constrain to x and z planes
    bool GetConstrainX();
    void ConstrainX(bool constrain);

    bool GetConstrainZ();
    void ConstrainZ(bool constrain);

    double GetConstrainStiffness();
    void SetConstrainStiffness(double stiffness);

    // Turn on/off haptics and force visualization
    bool GetEnableHaptics();
    void EnableHaptics(bool enable);

    bool GetEnableForceVisualization();
    void EnableForceVisualization(bool enable);

    // Save and load advanced configuration file
    void SaveConfigurationFile();
    void LoadConfigurationFile();

public slots:
    void Update();

protected:
    // Reimplement parent members
    virtual void initializeGL();
    virtual void paintGL();     
    virtual void resizeGL(int width, int height);

    // Draw text labels
    virtual void DrawLabels();

    // Timer
    QTimer* timer;

    // Graphics object that performs OpenGL rendering
    axmGraphics* graphics;

    // Haptics object that communicates with Novint Falcon
    axmHaptics* haptics;
};


#endif