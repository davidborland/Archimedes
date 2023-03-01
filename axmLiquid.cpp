/*=========================================================================

  Name:        axmLiquid.cpp

  Author:      David Borland

  Description: Represents the probe object in Archimedes

=========================================================================*/


#include "axmLiquid.h"

#include "axmPhysics.h"
#include "axmProbe.h"

#include <windows.h>
#include <GL/gl.h>

#include <stdio.h>


axmLiquid::axmLiquid() {
    density = 1.0;
    densityRange[0] = 0.1;
    densityRange[1] = 2.0;

    displacedVolume = 0.0;
    surfaceOffset = 0.0;
}

axmLiquid::~axmLiquid() {
}


void axmLiquid::SetDensity(double d) {
    density = d;

    double o = (density - densityRange[0]) / (densityRange[1] - densityRange[0]);

    double minOpacity = 0.2;
    double maxOpacity = 0.8;
    o = o * (maxOpacity - minOpacity) + minOpacity;
    opacity = o;
}

double axmLiquid::GetDensity() {
    return density;
}


void axmLiquid::SetMinDensity(double d) {
    densityRange[0] = d;
}

double axmLiquid::GetMinDensity() {
    return densityRange[0];
}


void axmLiquid::SetMaxDensity(double d) {
    densityRange[1] = d;
}

double axmLiquid::GetMaxDensity() {
    return densityRange[1];
}


double axmLiquid::GetBouyantForce(axmProbe* probe) {    
    // Calculate amount underwater
    double probeHeight = probe->GetHeight();
    double probeWidth = probe->GetWidth();

    double surface = position[1] + height * 0.5 + surfaceOffset;
    double probeSurface = probe->GetPosition()[1] - probeHeight * 0.5;

    double under = surface - probeSurface;
    under = under > probeHeight ? probeHeight : under;

    double force = 0.0;
    if (under > 0.0) {
        displacedVolume = under * probeWidth * probeWidth;
        force = axmPhysics::Weight(density, displacedVolume);

        // Compute the height of the displaced water
        double r = width / 2.0;
        double area = r * r * axmPhysics::Pi();
        surfaceOffset = displacedVolume / area;
    }
    else {
        displacedVolume = 0.0;
        surfaceOffset = 0.0;
    }

    return force;
}


void axmLiquid::Draw() {
    // Draw the displaced liquid
    glPushMatrix();

    glTranslated(position[0], position[1], 0.0);

    glColor4d(color[0] * 0.25, color[1] * 0.25, color[2] * 0.25, opacity);

    double w = width / 2.0;
    double h = height / 2.0;

    glBegin(GL_QUADS);

    glVertex2d(-w, h);
    glVertex2d(w, h);
    glVertex2d(w, h + surfaceOffset);
    glVertex2d(-w, h + surfaceOffset);

    glEnd();


    // Normal drawing
    glColor4d(color[0], color[1], color[2], opacity);

    glBegin(GL_QUADS);

    glVertex2d(-w, -h);
    glVertex2d(w, -h);
    glVertex2d(w, h + surfaceOffset);
    glVertex2d(-w, h + surfaceOffset);

    glEnd();

    if (drawBorder) {
        glColor3d(0.0, 0.0, 0.0);

        glLineWidth((float)borderWidth);

        glBegin(GL_LINES);

        glVertex2d(-w, -h);
        glVertex2d(w, -h);

        glVertex2d(w, -h);
        glVertex2d(w, h);

        glVertex2d(w, h);
        glVertex2d(-w, h);

        glVertex2d(-w, h);
        glVertex2d(-w, -h);

        glEnd();
    }

    glPopMatrix();
}


double axmLiquid::GetDisplacedVolume() {
    return displacedVolume;
}

double axmLiquid::GetSurfaceOffset() {
    return surfaceOffset;
}