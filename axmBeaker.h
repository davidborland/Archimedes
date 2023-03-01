/*=========================================================================

  Name:        axmBeaker.h

  Author:      David Borland

  Description: Draws a beaker filled with liquid on top of a scale.  The 
               size of the scale is set with the standard axmRenderObject
               interface.  Additional members are used to control the 
               beaker.

=========================================================================*/


#ifndef AXMBEAKER_H
#define AXMBEAKER_H


#include "axmRenderObject.h"

class axmLiquid;
class axmProbe;

class axmBeaker : public axmRenderObject {
public:
    axmBeaker();
    virtual ~axmBeaker();

    // Implement the base class interface
    void Draw();

    // Set the liquid and probe objects
    void SetLiquid(axmLiquid* liquidObject);
    void SetProbe(axmProbe* probeObject);

    // Get the liquid height
    double GetLiquidHeight();

protected:
    axmLiquid* liquid;
    axmProbe* probe;

    double liquidHeight;
};


#endif