/*=========================================================================

  Name:        axmHaptics.cpp

  Author:      David Borland

  Description: Handles haptic feedback via the Novint Falcon device
               for the Archimedes program.

=========================================================================*/


#include "axmHaptics.h"

#include "axmLiquid.h"
#include "axmPhysics.h"
#include "axmProbe.h"

#include <iostream>
#include <math.h>
#include <time.h>


// Continuous servo callback function
HDLServoOpExitCode ForceCB(void* userData)
{
    // Get pointer to haptics object
    axmHaptics* haptics = static_cast<axmHaptics*>(userData);

    // Zero out force
    haptics->forceServo[0] = haptics->forceServo[1] = haptics->forceServo[2] = 0.0;

    // Get current state of haptic device
    haptics->positionServoOld[0] = haptics->positionServo[0];
    haptics->positionServoOld[1] = haptics->positionServo[1];
    haptics->positionServoOld[2] = haptics->positionServo[2];

    hdlToolPosition(haptics->positionServo);
    hdlToolButton(&(haptics->buttonDownServo));

    // Get the position in application space
    haptics->Synchronize();

    // Call the function that computes the force
    haptics->ComputeForce();

    // Send force to device
    if (haptics->forceEnabled) {
        hdlSetToolForce(haptics->forceServo);
    }
    else {
        double zero[3] = { 0.0, 0.0, 0.0 };
        hdlSetToolForce(zero);
    }

    // Make sure to continue processing
    return HDL_SERVOOP_CONTINUE;
}

// On-demand synchronization callback function
HDLServoOpExitCode GetStateCB(void* userData)
{
    // Get pointer to haptics object
    axmHaptics* haptics = static_cast<axmHaptics*>(userData);

    // Call the function that copies data between servo side 
    // and client side
    haptics->Synchronize();

    // Only do this once.
    return HDL_SERVOOP_EXIT;
}


axmHaptics::axmHaptics() {
    // Initialize member variables
    connected = false;

    forceEnabled = true;

    forceCorrection = 0.0;

    deviceHandle = HDL_INVALID_HANDLE;

    positionServo[0] = positionServo[1] = positionServo[2] = 0.0;
    positionServoOld[0] = positionServoOld[1] = positionServoOld[2] = 0.0;
    position[0] = position[1] = position[2] = 0.0;

    buttonDownServo = false;
    buttonDown = false;

//    oldProbeV = 0.0;

    servoOp = HDL_INVALID_HANDLE;
    deviceHandle = HDL_INVALID_HANDLE;

    probe = NULL;
    liquid = NULL;
}

axmHaptics::~axmHaptics() {
    if (servoOp != HDL_INVALID_HANDLE) {
        hdlDestroyServoOp(servoOp);
        servoOp = HDL_INVALID_HANDLE;
    }

    hdlStop();

    if (deviceHandle != HDL_INVALID_HANDLE) {
        hdlUninitDevice(deviceHandle);
        deviceHandle = HDL_INVALID_HANDLE;
    }
}


bool axmHaptics::Initialize(double graphicsWorkspace[6]) {
    connected = false;

    // Initialize the device
    deviceHandle = hdlInitNamedDevice("DEFAULT");

    if (deviceHandle == HDL_INVALID_HANDLE) {
        std::cout << "Could not open device" << std::endl;
        return false;
    }


    // Now that the device is fully initialized, start the servo thread.
    // Failing to do this will result in a non-funtional haptics application.
    hdlStart();

    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not start the servo thread" << std::endl;
        return false;
    }


    // Set up callback function
    servoOp = hdlCreateServoOp(ForceCB, this, false);
    if (servoOp == HDL_INVALID_HANDLE) {
        std::cout << "Invalid servo op handle" << std::endl;
    }

    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not create servo op" << std::endl;
        return false;
    }


    // Make the device current.  All subsequent calls will
    // be directed towards the current device.
    hdlMakeCurrent(deviceHandle);

    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not make device current" << std::endl;
        return false;
    }


    // Get the extents of the device workspace
    hdlDeviceWorkspace(workspace);

    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not get device workspace" << std::endl;
        return false;
    }


    // Generate the transform from haptic space to graphics space
    bool useUniformScale = true;
    hdluGenerateHapticToAppWorkspaceTransform(workspace,
                                              graphicsWorkspace,
                                              false,
                                              haptics2graphics);
    
    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not generate workspace transform" << std::endl;
        return false;
    }

    connected = true;

    return true;
}


void axmHaptics::EnableForce(bool enable) {
    forceEnabled = enable;
}

bool axmHaptics::ForceEnabled() {
    return forceEnabled;
}


void axmHaptics::SetProbe(axmProbe* p) {
    probe = p;
}

void axmHaptics::SetLiquid(axmLiquid* l) {
    liquid = l;
}


double axmHaptics::GetForceCorrection() {
    return forceCorrection;
}

void axmHaptics::SetForceCorrection(double force) {
    forceCorrection = force;
}


bool axmHaptics::ButtonDown() {
    return buttonDown;
}


bool axmHaptics::GetConstrainX() {
    return constrainX;
}

void axmHaptics::ConstrainX(bool constrain) {
    constrainX = constrain;
}


bool axmHaptics::GetConstrainZ() {
    return constrainZ;
}

void axmHaptics::ConstrainZ(bool constrain) {
    constrainZ = constrain;
}


double axmHaptics::GetConstrainStiffness() {
    return springK;
}

void axmHaptics::SetConstrainStiffness(double k) {
    springK = k;
}


void axmHaptics::Update() {
    if (connected) {
        hdlCreateServoOp(GetStateCB, this, true);
    }
}


void axmHaptics::ComputeForce() {
    // The probe weight
    double force = -probe->GetWeight();

    // The bouyant force
    double bouyantForce = liquid->GetBouyantForce(probe);
    force += bouyantForce;

    probe->SetBouyantForce(bouyantForce);

    if (!buttonDown) {            
        forceServo[1] = 0.0;
    }
    else {
        // Feel the force
        probe->SetPosition(position[0], position[1]);
        forceServo[1] = force + forceCorrection;
    }

    // Constrain position with spring-mass-damper
    double c = springK * 4.0;
    if (constrainX) {
        forceServo[0] = -springK * positionServo[0] - c * (positionServo[0] - positionServoOld[0]);
    }
    else {
        forceServo[0] = 0.0;
    }

    if (constrainZ) {
        double z = -0.03;

        forceServo[2] = -springK * (positionServo[2] - z) - c * (positionServo[2] - positionServoOld[2]);
    }
    else {
        forceServo[2] = 0.0;
    }
}


void axmHaptics::Synchronize() {
    // Set the device position
    VectorMatrixMultiply(positionServo, haptics2graphics, position);

    // Set the button
    buttonDown = buttonDownServo;
}

void axmHaptics::VectorMatrixMultiply(double sourceVector[3], double matrix[16], double destinationVector[3])
{
    destinationVector[0] = matrix[0] * sourceVector[0] +
                           matrix[4] * sourceVector[1] +
                           matrix[8] * sourceVector[2] +
                           matrix[12];
    
    destinationVector[1] = matrix[1] * sourceVector[0] +
                           matrix[5] * sourceVector[1] +
                           matrix[9] * sourceVector[2] +
                           matrix[13];

    destinationVector[2] = matrix[2] * sourceVector[0] +
                           matrix[6] * sourceVector[1] +
                           matrix[10] * sourceVector[2] +
                           matrix[14];
}