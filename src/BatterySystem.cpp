#include "BatterySystem.h"
#include <cmath>#

void BatterySystem::update(VehicleState& state) {
    if (state.batteryLevel <= 0.0f) {
        state.batteryLevel = 0.0f;
        state.speedPercent = 0;
        return;
    }

    float currentLoad = static_cast<float>(std::abs(state.speedPercent)) / 100.0f;
    float drain = dischargeRateIdle + (currentLoad * dischargeRateMax);
    
    state.batteryLevel -= drain;

    if (state.batteryLevel <= CRITICAL_LEVEL && !state.isRTHActive) {
        state.isRTHActive = true;
    }
}