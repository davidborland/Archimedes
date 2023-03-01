/*=========================================================================

  Name:        axmRenderObject.cpp

  Author:      David Borland

  Description: Abstract base class for drawing objects in Archimedes.

=========================================================================*/


#include "axmRenderObject.h"

#include <windows.h>
#include <GL/gl.h>


axmRenderObject::axmRenderObject() {
    // Set defaults    
    color[0] = 0.5;
    color[1] = 0.5;
    color[2] = 0.5;

    opacity = 1.0;

    position[0] = 0.0;
    position[1] = 0.0;

    width = height = 1.0;

    rotation = 0.0;

    drawBorder = false;
    borderWidth = 2.0;
}

axmRenderObject::~axmRenderObject() {
}


void axmRenderObject::SetColor(double r, double g, double b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void axmRenderObject::SetColor(double* rgb) {
    SetColor(rgb[0], rgb[1], rgb[2]);
}

double* axmRenderObject::GetColor() {
    return color;
}


void axmRenderObject::SetOpacity(double alpha) {
    opacity = alpha;
}

double axmRenderObject::GetOpacity() {
    return opacity;
}


void axmRenderObject::SetPosition(double x, double y) {
    position[0] = x;
    position[1] = y;
}

void axmRenderObject::SetPosition(double* xy) {
    SetPosition(xy[0], xy[1]);
}

double* axmRenderObject::GetPosition() {
    return position;
}


double axmRenderObject::GetWidth() {
    return width;
}

void axmRenderObject::SetWidth(double w) {
    width = w;
}


double axmRenderObject::GetHeight() {
    return height;    
}

void axmRenderObject::SetHeight(double h) {
    height = h;
}


double axmRenderObject::GetRotation() {
    return rotation;
}

void axmRenderObject::SetRotation(double r) {
    rotation = r;
}



void axmRenderObject::BorderOn() {
    drawBorder = true;
}

void axmRenderObject::BorderOff() {
    drawBorder = false;
}

void axmRenderObject::SetBorderWidth(double pixels) {
    borderWidth = pixels;
}