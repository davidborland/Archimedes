/*=========================================================================

  Name:        axmLiquidContainer.h

  Author:      David Borland

  Description: Draws a rectangular shape.

=========================================================================*/


#ifndef AXMLIQUIDCONTAINER_H
#define AXMLIQUIDCONTAINER_H


#include "axmRenderObject.h"


class axmLiquidContainer : public axmRenderObject {
public:
    axmLiquidContainer();
    virtual ~axmLiquidContainer();

    // Implement the base class interface
    void Draw();
};


#endif