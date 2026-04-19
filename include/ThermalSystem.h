#ifndef THERMALSYSTEM_H
#define THERMALSYSTEM_H

#include "VehicleState.h"

class ThermalSystem {
private:
    const float AMBIENT_TEMP = 25.0f;
    const float WARNING_TEMP = 80.0f;   // Желтая зона (Предупреждение)
    const float CRITICAL_TEMP = 120.0f; // Красная зона (Глушим мотор)
    const float MAX_TEMP = 125.0f;      // Жесткий лимит ("Запрет на создание Солнца")
    const float SAFE_TEMP = 60.0f;      // Температура остывания

public:
    void update(VehicleState& state);
};

#endif