/*=========================================================================

  Name:        axmProbe.cpp

  Author:      David Borland

  Description: Represents the probe object in Archimedes

=========================================================================*/


#include "axmProbe.h"

#include "axmArrow.h"
#include "axmArrowTail.h"
#include "axmPhysics.h"

#include <math.h>


axmProbe::axmProbe() {
    density = 1.0;
    densityRange[0] = 0.1;
    densityRange[1] = 2.0;

    widthRange[0] = 0.1;
    widthRange[1] = 2.0;

    extent[0] = extent[2] = 0.0;
    extent[1] = extent[3] = 1.0;

    bouyantForce = 0.0;

    weightArrow = new axmArrow();
    weightArrow->SetColor(1.0, 0.0, 0.0);
    weightArrow->SetRotation(180.0);

    bouyantForceArrow = new axmArrowTail();
    bouyantForceArrow->SetColor(0.0, 0.5, 0.5);
    bouyantForceArrow->SetOpacity(0.5);

    forceArrow = new axmArrow();

    arrowWidthRangeScale[0] = 0.1;
    arrowWidthRangeScale[1] = 1.0;

    CalculateRanges();
}

axmProbe::~axmProbe() {
    delete weightArrow;
    delete bouyantForceArrow;
}


void axmProbe::SetPosition(double x, double y) {
    double w = width / 2.0;

    if (x < extent[0] + w) x = extent[0] + w;
    else if (x > extent[1] - w) x = extent[1] - w;

    if (y < extent[2] + w) y = extent[2] + w;
//    else if (y > extent[3] - w) y = extent[3] - w;

    axmRenderObject::SetPosition(x, y);

    weightArrow->SetPosition(x, y + w + weightArrow->GetWidth() / 2.0);
    bouyantForceArrow->SetPosition(x, y - w - bouyantForceArrow->GetWidth() / 2.0);
    forceArrow->SetPosition(x, y);
}


void axmProbe::SetColor(double r, double g, double b) {
    axmSquare::SetColor(r, g, b);

    forceArrow->SetColor(color[0] * 0.5, color[1] * 0.5, color[2] * 0.5);
}


void axmProbe::SetExtent(double xMin, double xMax, double yMin, double yMax) {
    extent[0] = xMin;
    extent[1] = xMax;
    extent[2] = yMin;
    extent[3] = yMax;

    bouyantForceArrow->SetLiquidParameters(xMin, xMax, 0.0);
}

bool axmProbe::OnBottom() {
    return (position[1] - height / 2.0) <= extent[2];
}


void axmProbe::SetDensity(double d) {
    density = d;

    double c = 1.0 - (density - densityRange[0]) / (densityRange[1] - densityRange[0]);

    double minIntensity = 0.25;
    double maxIntensity = 0.75;
    c = c * (maxIntensity - minIntensity) + minIntensity;
    color[0] = color[1] = color[2] = c;
    forceArrow->SetColor(color[0] * 0.5, color[0] * 0.5, color[0] * 0.5);

    // Calculate the new weight
    CalculateWeight();
}

double axmProbe::GetDensity() {
    return density;
}


void axmProbe::SetMinDensity(double d) {
    densityRange[0] = d;

    CalculateRanges();
}

double axmProbe::GetMinDensity() {
    return densityRange[0]; 
}


void axmProbe::SetMaxDensity(double d) {
    densityRange[1] = d;
    
    CalculateRanges();
}

double axmProbe::GetMaxDensity() {
    return densityRange[1];
}


void axmProbe::SetWidth(double w) {
    axmSquare::SetWidth(w);

    // Calculate the new weight
    CalculateWeight();
}

void axmProbe::SetMinWidth(double d) {
    widthRange[0] = d;
    
    CalculateRanges();
}

double axmProbe::GetMinWidth() {
    return widthRange[0];
}


void axmProbe::SetMaxWidth(double d) {
    widthRange[1] = d;
    
    CalculateRanges();
}

double axmProbe::GetMaxWidth() {
    return widthRange[1];
}


double axmProbe::GetWeight() {
    return weight;
}


void axmProbe::SetBouyantForce(double f) {
    bouyantForce = f;

    // Set the width of the bouyant force arrow
    double w = CalculateArrowWidth(bouyantForce);

    bouyantForceArrow->SetWidth(w);
    bouyantForceArrow->SetHeight(w);

    // Calculate the combined force
    CalculateForce();
}


axmArrow* axmProbe::GetWeightArrow() {
    return weightArrow;
}   

axmArrow* axmProbe::GetBouyantForceArrow() {
    return bouyantForceArrow;
}

axmArrow* axmProbe::GetForceArrow() {
    return forceArrow;
}


void axmProbe::CalculateWeight() {
    weight = axmPhysics::Weight(density, width * width * width);

    // Set the width of the arrow
    double w = CalculateArrowWidth(weight);

    weightArrow->SetWidth(w);
    weightArrow->SetHeight(w);

    // Calculate the combined force
    CalculateForce();
}

void axmProbe::CalculateRanges() {
    weightRange[0] = axmPhysics::Weight(densityRange[0], widthRange[0] * widthRange[0] * widthRange[0]); 
    weightRange[1] = axmPhysics::Weight(densityRange[1], widthRange[1] * widthRange[1] * widthRange[1]); 

    arrowWidthRange[0] = widthRange[0] * arrowWidthRangeScale[0];
    arrowWidthRange[1] = widthRange[1] * arrowWidthRangeScale[1];

    CalculateWeight();
}


double axmProbe::CalculateArrowWidth(double force) {
    if (force < weightRange[0]) {
        return force / weightRange[0] * arrowWidthRange[0];
    }
    else {
        return (force - weightRange[0]) / (weightRange[1] - weightRange[0]) *
               (arrowWidthRange[1] - arrowWidthRange[0]) + arrowWidthRange[0];
    }
}


void axmProbe::CalculateForce() {
    // Set the width of the combined force arrow
    double force = bouyantForce - weight;

    double w = CalculateArrowWidth(fabs(force));

    forceArrow->SetWidth(w);
    forceArrow->SetHeight(w);

    if (force < 0.0) {
        forceArrow->SetRotation(180.0);
    }
    else {
        forceArrow->SetRotation(0.0);
    }
}