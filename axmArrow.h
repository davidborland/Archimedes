/*=========================================================================

  Name:        axmArrow.h

  Author:      David Borland

  Description: Draws an arrow shape.

=========================================================================*/


#ifndef AXMARROW_H
#define AXMARROW_H


#include "axmRenderObject.h"


class axmArrow : public axmRenderObject {
public:
    axmArrow();
    virtual ~axmArrow();

    // Implement the base class interface
    void Draw();
};


#endif