/*=========================================================================

  Name:        axmArrowTail.h

  Author:      David Borland

  Description: Draws an arrow with a bifurcated tail.

=========================================================================*/


#ifndef axmArrowTailTAIL_H
#define axmArrowTailTAIL_H


#include "axmArrow.h"


class axmArrowTail : public axmArrow {
public:
    axmArrowTail();
    virtual ~axmArrowTail();

    // Implement the base class interface
    void Draw();

    // Set the necessary container and liquid parameters
    void SetLiquidParameters(double left, double right, double y);

protected:
    double liquidLeft;
    double liquidRight;
    double liquidY;

    void DrawTailSegment(double b0[2], double b1[2], double b2[2], double b3[2],
                         double t0[0], double t1[2], double t2[2], double t3[2]);
};


#endif