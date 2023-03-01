/*=========================================================================

  Name:        axmLiquidContainer.cpp

  Author:      David Borland

  Description: Draws a rectangular shape.

=========================================================================*/


#include "axmLiquidContainer.h"

#include <windows.h>
#include <GL/gl.h>


axmLiquidContainer::axmLiquidContainer() {
}

axmLiquidContainer::~axmLiquidContainer() {
}


void axmLiquidContainer::Draw() {
    glPushMatrix();

    glTranslated(position[0], position[1], 0.0);

    glColor4d(color[0], color[1], color[2], opacity);

    double w = width / 2.0;
    double h = height / 2.0;
    double bw = borderWidth;

    glBegin(GL_QUADS);

    // XXX: FIX THIS FOR POSITION AT CENTER

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

    glEnd();

    glPopMatrix();
}