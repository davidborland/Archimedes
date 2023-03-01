/*=========================================================================

  Name:        axmBeaker.cpp

  Author:      David Borland

  Description: Draws a beaker filled with liquid on top of a scale.  The 
               size of the scale is set with the standard axmRenderObject
               interface.  Additional members are used to control the 
               beaker.

=========================================================================*/


#include "axmBeaker.h"

#include "axmLiquid.h"
#include "axmPhysics.h"
#include "axmProbe.h"

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>


axmBeaker::axmBeaker() {
    liquid = NULL;
    probe = NULL;

    liquidHeight = 0.0;
}

axmBeaker::~axmBeaker() {
}


void axmBeaker::Draw() {
    glPushMatrix();

    glTranslated(position[0], position[1], 0.0);

    glColor4d(color[0], color[1], color[2], opacity);

    // Scale
    double w = width / 2.0;
    double h = height / 2.0;
    double bw = borderWidth;

    glBegin(GL_QUADS);


    // Scale

    // Left wall
    glVertex2d(-w - bw, -h - bw);
    glVertex2d(-w, -h - bw);
    glVertex2d(-w, h + bw);
    glVertex2d(-w - bw, h + bw);

    // Bottom wall
    glVertex2d(-w, -h - bw);
    glVertex2d(w, -h - bw);
    glVertex2d(w, -h);
    glVertex2d(-w, -h);

    // Right wall
    glVertex2d(w, -h - bw);
    glVertex2d(w + bw, -h - bw);
    glVertex2d(w + bw, h + bw);
    glVertex2d(w, h + bw);

    // Top wall
    glVertex2d(-w, h);
    glVertex2d(w, h);
    glVertex2d(w, h + bw);
    glVertex2d(-w, h + bw);

    // Support
    glVertex2d(-bw * 0.5, h + bw);
    glVertex2d(bw * 0.5, h + bw);
    glVertex2d(bw * 0.5, h + bw + h * 0.5);
    glVertex2d(-bw * 0.5, h + bw + h * 0.5);

    // Top surface
    glVertex2d(-w, h + bw + h * 0.5);
    glVertex2d(w, h + bw + h * 0.5);
    glVertex2d(w, h + bw + h * 0.5 + bw);
    glVertex2d(-w, h + bw + h * 0.5 + bw);


    // Beaker

    double maxWidth = probe->GetMaxWidth();
    double maxVolume = maxWidth * maxWidth * maxWidth;

    glColor3d(0.0, 0.0, 0.0);

    double r = w * 0.75;
    double y = h + bw + h * 0.5 + bw;
    double a = axmPhysics::Pi() * r * r;

    h = maxVolume / a + 2 * bw;

    // Left wall
    glVertex2d(-r - bw, y + bw);
    glVertex2d(-r, y + bw);
    glVertex2d(-r, y + bw + h);
    glVertex2d(-r - bw, y + bw + h);

    // Bottom wall
    glVertex2d(-r - bw, y);
    glVertex2d(r + bw, y);
    glVertex2d(r + bw, y + bw);
    glVertex2d(-r - bw, y + bw); 

    // Right wall
    glVertex2d(r, y + bw);
    glVertex2d(r + bw, y + bw);
    glVertex2d(r + bw, y + bw + h);
    glVertex2d(r, y + bw + h);


    // Liquid

    // Draw twice to get correct blended color

    liquidHeight = liquid->GetDisplacedVolume () / a;
    double lh = liquidHeight;

    glColor4d(liquid->GetColor()[0] * 0.25, 
              liquid->GetColor()[1] * 0.25, 
              liquid->GetColor()[2] * 0.25, 
              liquid->GetOpacity());

    glVertex2d(-r, y + bw);
    glVertex2d(r, y + bw);
    glVertex2d(r, y + bw + lh);
    glVertex2d(-r, y + bw + lh);

    glColor4d(liquid->GetColor()[0], 
              liquid->GetColor()[1], 
              liquid->GetColor()[2], 
              liquid->GetOpacity());

    glVertex2d(-r, y + bw);
    glVertex2d(r, y + bw);
    glVertex2d(r, y + bw + lh);
    glVertex2d(-r, y + bw + lh);


    glEnd();

    glPopMatrix();
}


void axmBeaker::SetLiquid(axmLiquid* liquidObject) {
    liquid = liquidObject;
}

void axmBeaker::SetProbe(axmProbe* probeObject) {
    probe = probeObject;
}


double axmBeaker::GetLiquidHeight() {
    return liquidHeight;
}