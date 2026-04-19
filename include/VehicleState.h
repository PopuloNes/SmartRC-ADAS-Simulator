#ifndef VEHICLESTATE_H
#define VEHICLESTATE_H

struct VehicleState {
    int speedPercent = 0;
    int steeringAngle = 0;

    float batteryLevel = 100.0f;
    float motorTempC = 25.0f;
    bool isOverheated = false;
    bool isTempWarning = false;
    bool isConnected = true;
    bool isRTHActive = false;

    float distanceToWall = 99.0f;
};

#endif