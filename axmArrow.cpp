/*=========================================================================

  Name:        axmArrow.cpp

  Author:      David Borland

  Description: Draws an arrow shape.

=========================================================================*/


#include "axmArrow.h"

#include <windows.h>
#include <GL/gl.h>


axmArrow::axmArrow() {
}

axmArrow::~axmArrow() {
}


void axmArrow::Draw() {
    glPushMatrix();

    glTranslated(position[0], position[1], 0.0);
    glRotated(rotation, 0.0, 0.0, 1.0);

    glColor4d(color[0], color[1], color[2], opacity);

    double w = width / 2.0;
    double h = height / 2.0;


    glBegin(GL_QUADS);

    glVertex2d(-w / 2.0, -h);
    glVertex2d(w / 2.0, -h);
    glVertex2d(w / 2.0, 0.0);
    glVertex2d(-w / 2.0, 0.0);

    glEnd();


    glBegin(GL_TRIANGLES);

    glVertex2d(-w, 0.0);
    glVertex2d(w, 0.0);
    glVertex2d(0.0, h);    

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