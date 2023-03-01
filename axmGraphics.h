/*=========================================================================

  Name:        axmGraphics.h

  Author:      David Borland

  Description: Handles drawing graphics for the Archimedes program.

=========================================================================*/


#ifndef AXMGRAPHICS_H
#define AXMGRAPHICS_H


class axmBeaker;
class axmLiquid;
class axmLiquidContainer;
class axmProbe;
class axmRectangle;


class axmGraphics {
public:
    axmGraphics();
    virtual ~axmGraphics();

    void Initialize();
    void Update();
    void UpdateAnimation(bool buttonDown, double timeDelta);
    void Resize(int width, int height);

    axmProbe* GetProbe();
    axmLiquid* GetLiquid();
    axmBeaker* GetBeaker();

    double* GetWorkspace();
    double* GetHapticWorkspace();

    void EnableForceVisualization(bool enable);
    bool ForceVisualizationEnabled();

protected:
    axmProbe* probe;

    axmLiquid* liquid;
    axmLiquidContainer* container;

    axmBeaker* beaker;

    // Workspace of the graphics, and the subset
    // to use for haptics
    double workspace[6];
    double hapticWorkspace[6];

    bool forceVisualizationEnabled;

    // The probe velocity for animation, in cm/s
    double velocity;
};


#endif