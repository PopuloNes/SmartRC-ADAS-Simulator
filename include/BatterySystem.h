#ifndef BATTERYSYSTEM_H
#define BATTERYSYSTEM_H

#include "VehicleState.h"

class BatterySystem {
private:
    float dischargeRateIdle = 0.0005f;
    float dischargeRateMax = 0.005f;
    const float CRITICAL_LEVEL = 15.0f;

public:
    void update(VehicleState& state);
};

#endif // BATTERYSYSTEM_H//SMARTRC_SIMULATOR_BATTERYSYSTEM_H