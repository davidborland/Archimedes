/*=========================================================================

  Name:        axmPhysics.h

  Author:      David Borland

  Description: Contains functions for calculating the physics in 
               Archimedes

=========================================================================*/


#ifndef AXMPHYSICS_H
#define AXMPHYSICS_H


#include "axmRectangle.h"


class axmPhysics {
public:
    axmPhysics();
    virtual ~axmPhysics();

    // Compute the weight in N from the density in g/cm^3 and volume in cm^3
    static double Weight(double density, double volume);

    // Pi
    static double Pi();

    // Cosine interpolation
    static double CosineInterpolate(double y1, double y2, double mu);

    // Cubic interpolation
    static double CubicInterpolate(double y0, double y1,
                                   double y2, double y3,
                                   double mu);

protected:
    // Gravity in m/s^2
    const static double gravity;

    // Pi
    const static double pi;
};


#endif