/*=========================================================================

  Name:        axmProbe.h

  Author:      David Borland

  Description: Represents the probe object in Archimedes

=========================================================================*/


#ifndef AXMPROBE_H
#define AXMPROBE_H


#include "axmSquare.h"

class axmArrow;
class axmArrowTail;


class axmProbe : public axmSquare {
public:
    axmProbe();
    virtual ~axmProbe();

    // Position
    virtual void SetPosition(double x, double y);

    // Color
    virtual void SetColor(double r, double g, double b);

    // Where the probe is allowed to go
    virtual void SetExtent(double xMin, double xMax, 
                           double yMin, double yMax);
    virtual bool OnBottom();

    // Density in g/cm^3
    virtual void SetDensity(double d);
    virtual double GetDensity();

    virtual void SetMinDensity(double d);
    virtual double GetMinDensity();

    virtual void SetMaxDensity(double d);
    virtual double GetMaxDensity();

    // Width in cm
    virtual void SetWidth(double w);
    virtual void SetMinWidth(double w);
    virtual double GetMinWidth();

    virtual void SetMaxWidth(double w);
    virtual double GetMaxWidth();

    // Weight in N
    virtual double GetWeight();

    // Bouyant force acting upon the probe
    virtual void SetBouyantForce(double f);

    // Get the arrows
    virtual axmArrow* GetWeightArrow();
    virtual axmArrow* GetBouyantForceArrow();
    virtual axmArrow* GetForceArrow();

protected:
    // Parameters that can be set
    double density;
    double densityRange[2];
    
    double widthRange[2];

    double extent[4];

    double bouyantForce;

    // Calculated weight
    void CalculateWeight();
    double weight;

    // Arrows representing forces
    axmArrow* weightArrow;
    axmArrowTail* bouyantForceArrow; 
    axmArrow* forceArrow;

    // Ranges for scaling the arrows
    double weightRange[2];
    double arrowWidthRangeScale[2];
    double arrowWidthRange[2];

    void CalculateRanges();
    double CalculateArrowWidth(double force);
    void CalculateForce();
};


#endif