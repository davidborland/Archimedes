/*=========================================================================

  Name:        axmRectangle.h

  Author:      David Borland

  Description: Draws a rectangular shape.

=========================================================================*/


#ifndef AXMRECTANGLE_H
#define AXMRECTANGLE_H


#include "axmRenderObject.h"


class axmRectangle : public axmRenderObject {
public:
    axmRectangle();
    virtual ~axmRectangle();

    // Implement the base class interface
    void Draw();
};


#endif