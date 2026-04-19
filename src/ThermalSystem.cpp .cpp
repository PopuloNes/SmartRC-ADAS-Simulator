#include "ThermalSystem.h"
#include <cmath>

void ThermalSystem::update(VehicleState& state) {
    float load = static_cast<float>(std::abs(state.speedPercent)) / 100.0f;
    
    if (load > 0.1f) state.motorTempC += load * 0.03f;
    if (state.motorTempC > AMBIENT_TEMP) state.motorTempC -= 0.01f;

    if (state.motorTempC > MAX_TEMP) {
        state.motorTempC = MAX_TEMP;
    }

    if (state.motorTempC >= CRITICAL_TEMP) {
        state.isOverheated = true;
        state.isTempWarning = false;
    }
    else if (state.motorTempC >= WARNING_TEMP && !state.isOverheated) {
        state.isTempWarning = true;
    }
    else if (state.motorTempC <= SAFE_TEMP) {
        state.isOverheated = false;
        state.isTempWarning = false;
    }
}