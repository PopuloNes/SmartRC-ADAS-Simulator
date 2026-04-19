#include "SafetyBrain.h"

void SafetyBrain::process(int requestedSpeed, int requestedSteering, VehicleState& state) {

    switch (currentState) {

        case SystemState::NORMAL_DRIVE:
            if (state.isOverheated) currentState = SystemState::CRITICAL_HALT;
            else if (requestedSpeed > 0 && state.distanceToWall < 2.5f) currentState = SystemState::COLLISION_AVOIDANCE;
            else if (state.isTempWarning) currentState = SystemState::THERMAL_WARNING;
            else if (state.isRTHActive) currentState = SystemState::RETURN_TO_HOME;
            break;

        case SystemState::COLLISION_AVOIDANCE:
            if (state.isOverheated) currentState = SystemState::CRITICAL_HALT;
            else if (state.distanceToWall >= 2.5f || requestedSpeed <= 0) currentState = SystemState::NORMAL_DRIVE;
            break;

        case SystemState::THERMAL_WARNING:
            if (state.isOverheated) currentState = SystemState::CRITICAL_HALT;
            else if (requestedSpeed > 0 && state.distanceToWall < 2.5f) currentState = SystemState::COLLISION_AVOIDANCE;
            else if (!state.isTempWarning) currentState = SystemState::NORMAL_DRIVE;
            break;

        case SystemState::RETURN_TO_HOME:
            if (state.isOverheated) currentState = SystemState::CRITICAL_HALT;
            else if (state.distanceToWall < 2.5f) currentState = SystemState::COLLISION_AVOIDANCE;
            break;

        case SystemState::CRITICAL_HALT:
            if (!state.isOverheated) currentState = SystemState::NORMAL_DRIVE;
            break;
    }


    switch (currentState) {

        case SystemState::NORMAL_DRIVE:
            state.speedPercent = requestedSpeed;
            state.steeringAngle = requestedSteering;
            lastLogMessage = "Path clear. User in control.";
            break;

        case SystemState::COLLISION_AVOIDANCE:
            state.speedPercent = -50;
            state.steeringAngle = requestedSteering;
            lastLogMessage = "[ALARM] RADAR TRIGGERED! Emergency braking!";
            break;

        case SystemState::THERMAL_WARNING:
            state.speedPercent = (requestedSpeed > 40) ? 40 : requestedSpeed;
            if (state.speedPercent < -40) state.speedPercent = -40;
            state.steeringAngle = requestedSteering;
            lastLogMessage = "[WARNING] MOTOR HOT (>80C)! Power limited to 40%.";
            break;

        case SystemState::RETURN_TO_HOME:
            state.speedPercent = 20;
            state.steeringAngle = 0;
            lastLogMessage = "[OVERRIDE] RTH Protocol Active. Returning to base.";
            break;

        case SystemState::CRITICAL_HALT:
            state.speedPercent = 0;
            state.steeringAngle = requestedSteering;
            lastLogMessage = "[FATAL] ENGINE OVERHEATED (>120C)! Shutting down.";
            break;
    }
}

std::string SafetyBrain::getLastLog() const { return lastLogMessage; }

std::string SafetyBrain::getCurrentStateName() const {
    switch (currentState) {
        case SystemState::NORMAL_DRIVE: return "NORMAL DRIVE";
        case SystemState::COLLISION_AVOIDANCE: return "COLLISION AVOIDANCE";
        case SystemState::RETURN_TO_HOME: return "RETURN TO HOME";
        case SystemState::THERMAL_WARNING: return "THERMAL WARNING";
        case SystemState::CRITICAL_HALT: return "THERMAL SHUTDOWN";
        default: return "UNKNOWN";
    }
}