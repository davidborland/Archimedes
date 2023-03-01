/*=========================================================================

  Name:        axmHaptics.h

  Author:      David Borland

  Description: Handles haptic feedback via the Novint Falcon device
               for the Archimedes program.

=========================================================================*/


#ifndef AXMHAPTICS_H
#define AXMHAPTICS_H


#include <hdl/hdl.h>


class axmLiquid;
class axmProbe;


class axmHaptics {
public:
    axmHaptics();
    virtual ~axmHaptics();

    // Initialize HDAL
    bool Initialize(double graphicsWorkspace[6]);

    // Enable haptics or not
    void EnableForce(bool enable);
    bool ForceEnabled();

    // Set the probe object
    void SetProbe(axmProbe* p);

    // Set the liquid object
    void SetLiquid(axmLiquid* l);

    // Get/set the force correction 
    double GetForceCorrection();
    void SetForceCorrection(double force);

    // Get button down or not
    bool ButtonDown();

    // Set the spring stiffness for constraining to x and z planes
    bool GetConstrainX();
    void ConstrainX(bool constrain);

    bool GetConstrainZ();
    void ConstrainZ(bool constrain);

    double GetConstrainStiffness();
    void SetConstrainStiffness(double k);

    // Called from the application thread when it wants to exchange
    // data with the servo
    void Update();

protected:
    // Define callback functions as friends
    friend HDLServoOpExitCode ForceCB(void *userData);
    friend HDLServoOpExitCode GetStateCB(void *userData);

    // Compute the haptic force
    void ComputeForce();

    //  Move data between servo and application variables
    void Synchronize();

    // Matrix multiply
    // XXX: Switch to quatlib or Haggis if need anything more than this
    void VectorMatrixMultiply(double sourceVector[3], double matrix[16], double destinationVector[3]);

    // Connected to device?
    bool connected;

    // Displaing forces?
    bool forceEnabled;

    // Variables used only by servo thread
    double positionServo[3];
    double positionServoOld[3];
    double forceServo[3];
    bool buttonDownServo;

    // Variables used only by application thread
    double position[3];
    bool buttonDown;

    // Constant force correction for weight of device handle
    double forceCorrection;

    // Spring constant for constraining to x and z planes
    bool constrainX;
    bool constrainZ;
    double springK;

    // Handle to device
    HDLDeviceHandle deviceHandle;

    // Handle to haptic callback 
    HDLServoOpExitCode servoOp;

    // Device workspace dimensions
    double workspace[6];

    // Transform from haptics space to graphics space
    double haptics2graphics[16];

    // The probe object
    axmProbe* probe;

    // The liquid object
    axmLiquid* liquid;

    // Surface of liquid (cm)
    double liquidSurface;
};


#endif