#ifndef THERMALSYSTEM_H
#define THERMALSYSTEM_H

#include "VehicleState.h"

class ThermalSystem {
private:
    const float AMBIENT_TEMP = 25.0f;
    const float WARNING_TEMP = 80.0f;  
    const float CRITICAL_TEMP = 120.0f; 
    const float MAX_TEMP = 125.0f;      
    const float SAFE_TEMP = 60.0f;      

public:
    void update(VehicleState& state);
};

#endif
