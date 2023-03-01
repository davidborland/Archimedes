/*=========================================================================

  Name:        axmRectangle.cpp

  Author:      David Borland

  Description: Draws a rectangular shape.

=========================================================================*/


#include "axmRectangle.h"

#include <windows.h>
#include <GL/gl.h>


axmRectangle::axmRectangle() {
}

axmRectangle::~axmRectangle() {
}


void axmRectangle::Draw() {
    glPushMatrix();

    glTranslated(position[0], position[1], 0.0);

    glColor4d(color[0], color[1], color[2], opacity);

    double w = width / 2.0;
    double h = height / 2.0;

    glBegin(GL_QUADS);

    glVertex2d(-w, -h);
    glVertex2d(w, -h);
    glVertex2d(w, h);
    glVertex2d(-w, h);

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