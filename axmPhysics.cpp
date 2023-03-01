/*=========================================================================

  Name:        axmPhysics.h

  Author:      David Borland

  Description: Contains functions for calculating the physics in 
               Archimedes

=========================================================================*/


#include "axmPhysics.h"

#include <math.h>


const double axmPhysics::gravity = 9.80665;
const double axmPhysics::pi = 3.14159265;


axmPhysics::axmPhysics() {
}

axmPhysics::~axmPhysics() {
}


double axmPhysics::Weight(double density, double volume) {
    // Compute the mass in g
    double mass = density * volume;

    // Convert from g to kg
    mass /= 1000.0;

    // Return the weight in N = kg m/s^2
    return mass * gravity;
}


double axmPhysics::Pi() {
    return pi;
}


double axmPhysics::CosineInterpolate(double y1, double y2, double mu) {
   double mu2 = (1 - cos(mu * pi)) / 2.0;

   return (y1 * (1 - mu2) + y2 * mu2);
}

double axmPhysics::CubicInterpolate(double y0, double y1,
                                    double y2, double y3,
                                    double mu)
{
    double a0, a1, a2, a3, mu2;

    mu2 = mu * mu;

    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;

    return (a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
}
