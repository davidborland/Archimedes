/*=========================================================================

  Name:        axmArrowTail.cpp

  Author:      David Borland

  Description: Draws an arrow with a bifurcated tail.

=========================================================================*/


#include "axmArrowTail.h"

#include "axmPhysics.h"

#include <windows.h>
#include <GL/gl.h>


#include <stdio.h>

axmArrowTail::axmArrowTail() {
    liquidLeft = -1.0;
    liquidRight = 1.0;
    liquidY = 0.0;
}

axmArrowTail::~axmArrowTail() {
}


void axmArrowTail::Draw() {
    if (position[1] > 0.0) return;

    // Control points for the tail, an extra at each endpoint
    double b0[2];
    double b1[2];
    double b2[2];
    double b3[2];

    double t0[2];
    double t1[2];
    double t2[2];
    double t3[2];

    const double bottomOffset = 1.25;

    glPushMatrix();
    glColor4d(color[0], color[1], color[2], opacity);

    double w = width / 2.0;
    double h = height / 2.0;

    b3[0] = b3[1] = t3[0] = t3[1] = 0.0;


    // Left side


    // Bottom
    b0[0] = liquidLeft * 0.9;
    b0[1] = liquidY;

    b2[0] = position[0];
    b2[1] = position[1] - h;

    b1[0] = b0[0] + (b2[0] - b0[0]) * 0.5;
    b1[1] = position[1] * bottomOffset;
        
    // Top
    t0[0] = liquidLeft * 0.7;
    t0[1] = b0[1];

    t2[0] = position[0] - w / 2.0;
    t2[1] = b2[1];

    t1[0] = b1[0];
    t1[1] = position[1] * bottomOffset * 0.8;

    glBegin(GL_QUAD_STRIP);

    DrawTailSegment(b0, b0, b1, b2, t0, t0, t1, t2);
    DrawTailSegment(b0, b1, b2, b3, t0, t1, t2, t3);
    
    glEnd();


    // Right side


    // Bottom
    b0[0] = liquidRight * 0.9;

    b1[0] = (b0[0] + b2[0]) * 0.5;

    // Top
    t0[0] = liquidRight * 0.7;

    t2[0] = position[0] + w / 2.0;

    t1[0] = (t0[0] + t2[0]) * 0.5;

    glBegin(GL_QUAD_STRIP);

    DrawTailSegment(b0, b0, b1, b2, t0, t0, t1, t2);
    DrawTailSegment(b0, b1, b2, b3, t0, t1, t2, t3);
    
    glEnd();


    glPopMatrix();

    // Draw the arrow
    axmArrow::Draw();
}


void axmArrowTail::SetLiquidParameters(double left, double right, double y) {
    liquidLeft = left;
    liquidRight = right;
    liquidY = y;
}



void axmArrowTail::DrawTailSegment(double b0[2], double b1[2], double b2[2], double b3[2],
                                   double t0[2], double t1[2], double t2[2], double t3[2]) {
    const int numSteps = 10;

    for (int i = 0; i < numSteps; i++) {
        double mu = (double)i / (double)(numSteps - 1);

        double bx = b1[0] + (b2[0] - b1[0]) * mu;
        double by = axmPhysics::CubicInterpolate(b0[1], b1[1], b2[1], b3[1], mu);
                
        double tx = t1[0] + (t2[0] - t1[0]) * mu;
        double ty = axmPhysics::CubicInterpolate(t0[1], t1[1], t2[1], t3[1], mu);

        glVertex2d(bx, by);
        glVertex2d(tx, ty);
    }
}