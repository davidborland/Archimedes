/*=========================================================================

  Name:        axmSquare.h

  Author:      David Borland

  Description: Represents a square shape

=========================================================================*/


#ifndef AXMSQUARE_H
#define AXMSQUARE_H


#include "axmRectangle.h"


class axmSquare : public axmRectangle {
public:
    axmSquare();
    virtual ~axmSquare();

    // Overload to keep width and height the same
    virtual void SetWidth(double w);
    virtual void SetHeight(double h);
};


#endif