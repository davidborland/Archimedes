/*=========================================================================

  Name:        axmGraphics.cpp

  Author:      David Borland

  Description: Handles drawing graphics for the Archimedes program.

=========================================================================*/


#include "axmGraphics.h"

#include "axmArrow.h"
#include "axmBeaker.h"
#include "axmLiquid.h"
#include "axmLiquidContainer.h"
#include "axmProbe.h"
#include "axmRectangle.h"

#include <windows.h>
#include <GL/gl.h>
#include <iostream>


axmGraphics::axmGraphics() {
    // Set the graphics workspace
    double width = 30.0;
    double height = width;

    workspace[0] = -width * 0.5;
    workspace[1] = -height * 0.5;
    workspace[2] = -1.0;
    workspace[3] = width * 0.5;
    workspace[4] = height * 0.5;
    workspace[5] = 1.0;

    // Create the probe
    double probeSize = 1.0;

    probe = new axmProbe();

    probe->SetColor(0.5, 0.5, 0.5);
    probe->BorderOn();

    probe->SetDensity(1.0);
    probe->SetMinDensity(0.2);
    probe->SetMaxDensity(2.0);

    probe->SetMinWidth(6.0);
    probe->SetMaxWidth(8.0);
    probe->SetWidth((probe->GetMaxWidth() + probe->GetMinWidth()) / 2.0 );


    // Create the liquid
    double liquidWidth = width * 0.5;
    double liquidHeight = height * 0.4;
    double liquidX = 5.0;
    double liquidY = -liquidHeight / 2.0;

    probe->SetExtent(liquidX - liquidWidth / 2.0,
                     liquidX + liquidWidth / 2.0,
                     liquidY - liquidHeight / 2.0,
                     height / 2.0);

    liquid = new axmLiquid();

    liquid->SetColor(0.0, 1.0, 1.0);
    liquid->SetOpacity(0.5);
    liquid->SetWidth(liquidWidth);
    liquid->SetHeight(liquidHeight);
    liquid->SetPosition(liquidX, liquidY);

    liquid->SetDensity(1.0);
    liquid->SetMinDensity(0.2);
    liquid->SetMaxDensity(2.0);


    // Create the container
    double containerWidth = liquidWidth;
    double containerHeight = liquidHeight * 1.5;
    double containerX = liquidX;
    double containerY = -liquidHeight + containerHeight / 2.0;

    container = new axmLiquidContainer();

    container->SetColor(0.0, 0.0, 0.0);
    container->SetWidth(containerWidth);
    container->SetHeight(containerHeight);
    container->SetBorderWidth(containerWidth * 0.05);
    container->SetPosition(containerX, containerY);


    // Create the beaker
    beaker = new axmBeaker();
    beaker->SetProbe(probe);
    beaker->SetLiquid(liquid);

    double beakerWidth = containerWidth * 0.5;
    double beakerHeight = beakerWidth * 0.25;
    double beakerX = containerX - containerWidth * 0.5 - beakerWidth;
    double beakerY = containerY - containerHeight * 0.5 + beakerHeight * 0.5 - 
                     containerWidth * 0.05 + beakerWidth * 0.05;

    beaker->SetColor(0.25, 0.25, 0.25);
    beaker->SetWidth(beakerWidth);
    beaker->SetHeight(beakerHeight);
    beaker->SetBorderWidth(beakerWidth * 0.05);
    beaker->SetPosition(beakerX, beakerY);


    forceVisualizationEnabled = true;

    hapticWorkspace[0] = container->GetPosition()[0] - container->GetWidth() * 1.1;
    hapticWorkspace[1] = workspace[1];
    hapticWorkspace[2] = workspace[2];
    hapticWorkspace[3] = container->GetPosition()[0] + container->GetWidth() * 1.1;
    hapticWorkspace[4] = workspace[4];
    hapticWorkspace[5] = workspace[5];

    velocity = 0.0;
}

axmGraphics::~axmGraphics() {
    // Clean up
    delete probe;
    delete liquid;
    delete container;
    delete beaker;
}


void axmGraphics::Initialize() {
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void axmGraphics::UpdateAnimation(bool buttonDown, double timeDelta) {
    if (buttonDown) {
        velocity = 0.0;
        return;
    }

    // Get the force in N
    double force = -probe->GetWeight();
    double bouyantForce = liquid->GetBouyantForce(probe);
    force += bouyantForce;

    // Get the width in cm
    double w = probe->GetWidth();

    // Get the mass in g
    double m = w * w * w * probe->GetDensity();

    // Get the acceleration in m/s^2
    double a = force / (m / 1000.0);

    if (!(probe->OnBottom() && a <= 0.0)) {    
        // Compute the velocity in cm/s, with some viscosity for water
        double vWater = 0.15;
        bool under = liquid->GetDisplacedVolume() > 0.0;
        double v = under * vWater;

        double newVelocity = a * 100.0 * timeDelta;

        velocity = velocity + newVelocity - velocity * v - newVelocity * v;

        double y = probe->GetPosition()[1] + velocity * timeDelta;

        probe->SetPosition(probe->GetPosition()[0], y);
    }
    else {
        velocity = 0.0;
    }
}

void axmGraphics::Update() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    probe->Draw();

    container->Draw();

    beaker->Draw();

    if (forceVisualizationEnabled) {
        probe->GetBouyantForceArrow()->Draw();
    }

    liquid->Draw();

    if (forceVisualizationEnabled) {
        probe->GetWeightArrow()->Draw();
        probe->GetForceArrow()->Draw();
    }
}

void axmGraphics::Resize(int width, int height) {
    double aspect = (double)width / (double)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(workspace[0] * aspect, workspace[3] * aspect, 
            workspace[1],          workspace[4],
            workspace[2],          workspace[5]);

    glMatrixMode(GL_MODELVIEW);
}


axmProbe* axmGraphics::GetProbe() {
    return probe;
}

axmLiquid* axmGraphics::GetLiquid() {
    return liquid;
}

axmBeaker* axmGraphics::GetBeaker() {
    return beaker;
}


double* axmGraphics::GetWorkspace() {
    return workspace;
}

double* axmGraphics::GetHapticWorkspace() {
    return hapticWorkspace;
}


void axmGraphics::EnableForceVisualization(bool enable) {
    forceVisualizationEnabled = enable;
}

bool axmGraphics::ForceVisualizationEnabled() {
    return forceVisualizationEnabled;
}