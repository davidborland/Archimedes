/*=========================================================================

  Name:        axmLiquid.h

  Author:      David Borland

  Description: Represents the liquid in Archimedes

=========================================================================*/


#ifndef axmLiquid_H
#define axmLiquid_H


#include "axmSquare.h"

class axmProbe;


class axmLiquid : public axmRectangle {
public:
    axmLiquid();
    virtual ~axmLiquid();

    // Density in g/cm^3
    virtual void SetDensity(double d);
    virtual double GetDensity();

    virtual void SetMinDensity(double d);
    virtual double GetMinDensity();

    virtual void SetMaxDensity(double d);
    virtual double GetMaxDensity();

    // Compute bouyant force acting on the probe
    virtual double GetBouyantForce(axmProbe* probe);

    // Get the diplaced liquid volume
    virtual double GetDisplacedVolume();
    virtual double GetSurfaceOffset();

    virtual void Draw();

protected:
    double density;
    double densityRange[2];

    double displacedVolume;
    double surfaceOffset;
};


#endif