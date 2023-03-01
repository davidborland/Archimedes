/*=========================================================================

  Name:        axmGLWidget.cpp

  Author:      David Borland

  Description: Handles drawing to an OpenGL buffer.

=========================================================================*/


#include "axmGLWidget.h"

#include "axmArrow.h"
#include "axmBeaker.h"
#include "axmLiquid.h"
#include "axmProbe.h"

#include <QtGui>

#include <iostream>
#include <fstream>


axmGLWidget::axmGLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    // Graphics and haptics objects
    graphics = new axmGraphics();
    haptics = new axmHaptics();

    // Initalize some stuff
    haptics->SetProbe(graphics->GetProbe());
    haptics->SetLiquid(graphics->GetLiquid());

    // Load the advanced configuration file
    LoadConfigurationFile();

    // Create a timer for updating
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
    timer->start(10);

    // Need to do this to render text with QPainter
    setAutoFillBackground(false);
    setAutoBufferSwap(false);
}

axmGLWidget::~axmGLWidget() {
    // Delete haptics before graphics, as haptics contains
    // references to objects in graphics
    delete haptics;
    delete graphics;
}


double axmGLWidget::GetProbeDensity() {
    return graphics->GetProbe()->GetDensity();
}

void axmGLWidget::SetProbeDensity(double density) {
    graphics->GetProbe()->SetDensity(density);
}


double axmGLWidget::GetMinProbeDensity() {
    return graphics->GetProbe()->GetMinDensity();
}

double axmGLWidget::GetMaxProbeDensity() {
    return graphics->GetProbe()->GetMaxDensity();
}


double axmGLWidget::GetProbeWidth() {
    return graphics->GetProbe()->GetWidth();
}

void axmGLWidget::SetProbeWidth(double width) {
    graphics->GetProbe()->SetWidth(width);
}


double axmGLWidget::GetMinProbeWidth() {
    return graphics->GetProbe()->GetMinWidth();
}

double axmGLWidget::GetMaxProbeWidth() {
    return graphics->GetProbe()->GetMaxWidth();
}


double axmGLWidget::GetLiquidDensity() {
    return graphics->GetLiquid()->GetDensity();
}

void axmGLWidget::SetLiquidDensity(double density) {
    graphics->GetLiquid()->SetDensity(density);
    graphics->GetProbe()->GetBouyantForceArrow()->SetOpacity(graphics->GetLiquid()->GetOpacity());
}


double axmGLWidget::GetMinLiquidDensity() {
    return graphics->GetLiquid()->GetMinDensity();
}

double axmGLWidget::GetMaxLiquidDensity() {
    return graphics->GetLiquid()->GetMaxDensity();
}


double axmGLWidget::GetForceCorrection() {
    return haptics->GetForceCorrection();
}

void axmGLWidget::SetForceCorrection(double force) {
    haptics->SetForceCorrection(force);
}


bool axmGLWidget::GetConstrainX() {
    return haptics->GetConstrainX();
}

void axmGLWidget::ConstrainX(bool constrain) {
    haptics->ConstrainX(constrain);
}


bool axmGLWidget::GetConstrainZ() {
    return haptics->GetConstrainZ();
}

void axmGLWidget::ConstrainZ(bool constrain) {
    haptics->ConstrainZ(constrain);
}


double axmGLWidget::GetConstrainStiffness() {
    return haptics->GetConstrainStiffness();
}

void axmGLWidget::SetConstrainStiffness(double stiffness) {
    haptics->SetConstrainStiffness(stiffness);
}


bool axmGLWidget::GetEnableHaptics() {
    return haptics->ForceEnabled();
}

void axmGLWidget::EnableHaptics(bool enable) {
    haptics->EnableForce(enable);
}

bool axmGLWidget::GetEnableForceVisualization() {
    return graphics->ForceVisualizationEnabled();
}

void axmGLWidget::EnableForceVisualization(bool enable) {
    graphics->EnableForceVisualization(enable);
}


void axmGLWidget::Update() {
    // Update the haptics
    haptics->Update();

    // Update the animation, with the time delta in seconds
    graphics->UpdateAnimation(haptics->ButtonDown(), (double)timer->interval() / 1000.0);

    // Render
    graphics->Update();

    // Tell the widget to update
    updateGL();
}


void axmGLWidget::initializeGL() {
    if (!haptics->Initialize(graphics->GetHapticWorkspace())) {
        std::cout << "Haptics initialization failed" << std::endl;
    }

    graphics->Initialize();
}

void axmGLWidget::paintGL() { 
    // Draw the OpenGL scene
    graphics->Initialize();
    graphics->Update();    
    
    // Draw text labels
    if (graphics->ForceVisualizationEnabled()) {
        DrawLabels();
    }

    swapBuffers();
}

void axmGLWidget::resizeGL(int width, int height) {
    graphics->Resize(width, height);
}


void axmGLWidget::SaveConfigurationFile() {
    std::ofstream file;
    file.open("Archimedes.cfg");

    if (!file.good()) {
        std::cout << "Failed to save configuration file." << std::endl;

        return;
    }

    // Just one line with the values in order
    file << haptics->ForceEnabled() << " " <<
            graphics->ForceVisualizationEnabled() << " " <<
            haptics->GetForceCorrection() << " " <<
            haptics->GetConstrainX() << " " <<
            haptics->GetConstrainZ() << " " <<
            haptics->GetConstrainStiffness();


    file.close();
}

void axmGLWidget::LoadConfigurationFile() {
    std::ifstream file;
    file.open("Archimedes.cfg");

    if (!file.good()) {
        std::cout << "Failed to open configuration file.  Using default advanced options." << std::endl;

        return;
    }

    // Just one line with the values in order
    bool forceEnabled;
    bool visualizationEnabled;
    double forceCorrection;
    bool constrainX;
    bool constrainZ;
    double constrainStiffness;

    file >> forceEnabled >> visualizationEnabled >> forceCorrection >>
            constrainX >> constrainZ >> constrainStiffness;

    file.close();

    haptics->EnableForce(forceEnabled);
    graphics->EnableForceVisualization(visualizationEnabled);
    haptics->SetForceCorrection(forceCorrection);
    haptics->ConstrainX(constrainX);
    haptics->ConstrainZ(constrainZ);
    haptics->SetConstrainStiffness(constrainStiffness);
}


void axmGLWidget::DrawLabels() {
    // Draw text labels
    QPainter painter(this);

    double* w = graphics->GetWorkspace();
    double a = (double)width() / (double)height();

    axmProbe* probe = graphics->GetProbe();
    axmLiquid* liquid = graphics->GetLiquid();
    axmBeaker* beaker = graphics->GetBeaker();

    axmArrow* weightArrow = probe->GetWeightArrow();
    axmArrow* bouyantForceArrow = probe->GetBouyantForceArrow();
    axmArrow* forceArrow = probe->GetForceArrow();

    QString text;
    QFontMetrics metrics(font());
    QRect rect;
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);

    double x, y;

    int border = 5;

    // Weight
    double weight = probe->GetWeight();
    if (weight > 0.0) {        
        text = QString().sprintf("%0.2f N", weight);
        rect = metrics.boundingRect(text);

        x = (weightArrow->GetPosition()[0] - w[0] * a) / 
            (w[3] * a - w[0] * a) * width() - rect.width() * 0.75;
        y = height() - 
            (weightArrow->GetPosition()[1] + weightArrow->GetHeight() / 2.0 - w[1]) / 
            (w[4] - w[1]) * height() - border;

//        painter.fillRect(x, y, rect.width(), rect.height(), QColor(200, 200, 200));
        painter.drawText(x, y, text);
    }

    // Bouyant force    
    double bouyantForce = liquid->GetBouyantForce(probe);
    if (bouyantForce > 0.0) {        
        text = QString().sprintf("%0.2f N", bouyantForce);
        rect = metrics.boundingRect(text);

        x = (bouyantForceArrow->GetPosition()[0] - w[0] * a) / 
            (w[3] * a - w[0] * a) * width() - rect.width() * 0.75;
        y = height() - 
            (bouyantForceArrow->GetPosition()[1] - bouyantForceArrow->GetHeight() / 2.0 - w[1]) / 
            (w[4] - w[1]) * height() + rect.height();
        
        painter.drawText(x, y, text);
    }

    // Combined force    
    double force = abs(bouyantForce - weight);
    if (force > 0.0099) {        
        text = QString().sprintf("%0.2f N", force);
        rect = metrics.boundingRect(text);

        x = (forceArrow->GetPosition()[0] + forceArrow->GetWidth() / 2.0 - w[0] * a) / 
            (w[3] * a - w[0] * a) * width() + border;
        y = height() - 
            (forceArrow->GetPosition()[1] - w[1]) / 
            (w[4] - w[1]) * height() + rect.height() / 2.0;
        
        painter.drawText(x, y, text);
    }

    // Displaced liquid mass
    double mass = liquid->GetDisplacedVolume() * liquid->GetDensity();
    text = QString().sprintf("%0.2f g", mass);
    rect = metrics.boundingRect(text);

    x = (beaker->GetPosition()[0] - w[0] * a) /
        (w[3] * a - w[0] * a) * width() - rect.width() * 0.75;
    y = height() -
        (beaker->GetPosition()[1] - w[1]) / 
        (w[4] - w[1]) * height() + rect.height() / 2.0;

    
    painter.drawText(x, y, text);

    // Displaced liquid volume
    double volume = liquid->GetDisplacedVolume();
    text = QString().sprintf("%0.2f cm^3", volume);
    rect = metrics.boundingRect(text);

//    x = (beaker->GetPosition()[0] + beaker->GetWidth() * 0.425 - w[0] * a) /
//        (w[3] * a - w[0] * a) * width() + border;
    x = (beaker->GetPosition()[0] - w[0] * a) /
        (w[3] * a - w[0] * a) * width() - rect.width() * 0.75;

    y = height() -
        (beaker->GetPosition()[1] + beaker->GetHeight() * 1.4 + beaker->GetLiquidHeight() - w[1]) / 
        (w[4] - w[1]) * height() + rect.height() * 1.5;

    
    painter.drawText(x, y, text);


    // Clean up
    painter.end();
}