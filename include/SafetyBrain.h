// файл: include/SafetyBrain.h
#ifndef SAFETYBRAIN_H
#define SAFETYBRAIN_H

#include "VehicleState.h"
#include <string>

enum class SystemState {
    NORMAL_DRIVE,
    COLLISION_AVOIDANCE,
    RETURN_TO_HOME,
    THERMAL_WARNING,
    CRITICAL_HALT
};

class SafetyBrain {
private:
    SystemState currentState = SystemState::NORMAL_DRIVE;
    std::string lastLogMessage = "System nominal.";

public:
    void process(int requestedSpeed, int requestedSteering, VehicleState& state);
    std::string getLastLog() const;
    std::string getCurrentStateName() const;
};

#endif