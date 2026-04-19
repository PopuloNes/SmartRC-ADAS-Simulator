#include "raylib.h"
#include <string>
#include <math.h>
#include "VehicleState.h"
#include "BatterySystem.h"
#include "SafetyBrain.h"
#include "ThermalSystem.h"

using namespace std;

int main() {
    InitWindow(800, 600, "Smart RC - ADAS Simulator");
    SetTargetFPS(60);

    VehicleState myCar;
    BatterySystem battery;
    SafetyBrain brain;
    ThermalSystem thermal;

    float currentSpeed = 0.0f;
    float currentSteering = 0.0f;

    bool isWallSpawned = false;
    float wallVisualOffsetX = 0.0f;

    bool isCruiseControlActive = false;
    float cruiseSpeed = 0.0f;

    while (!WindowShouldClose()) {

        float targetSpeed = 0.0f;
        float targetSteering = 0.0f;

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)) {
            isCruiseControlActive = false;
        }

        if (IsKeyDown(KEY_W)) targetSpeed = 100.0f;
        else if (IsKeyDown(KEY_S)) targetSpeed = -100.0f;
        else if (isCruiseControlActive) targetSpeed = cruiseSpeed;

        if (IsKeyPressed(KEY_C) && currentSpeed > 10.0f) {
            isCruiseControlActive = !isCruiseControlActive;
            cruiseSpeed = currentSpeed;
        }

        if (IsKeyDown(KEY_A)) targetSteering = -45.0f;
        else if (IsKeyDown(KEY_D)) targetSteering = 45.0f;

        currentSpeed += (targetSpeed - currentSpeed) * 0.02f;
        currentSteering += (targetSteering - currentSteering) * 0.15f;

        if (IsKeyDown(KEY_H)) myCar.motorTempC += 0.5f;

        battery.update(myCar);
        thermal.update(myCar);
        brain.process((int)currentSpeed, (int)currentSteering, myCar);

        if (brain.getCurrentStateName() != "NORMAL DRIVE") {
            isCruiseControlActive = false;
        }

        if (IsKeyPressed(KEY_O)) {
            isWallSpawned = true;
            myCar.distanceToWall = 8.0f;
            wallVisualOffsetX = 0.0f;
        }

        if (isWallSpawned) {

            myCar.distanceToWall -= (myCar.speedPercent * 0.002f);
            if (myCar.distanceToWall < 0.1f) myCar.distanceToWall = 0.1f;

            wallVisualOffsetX -= (myCar.steeringAngle * myCar.speedPercent * 0.0002f);
            if (abs(wallVisualOffsetX) > 60.0f) {
                isWallSpawned = false;
                myCar.distanceToWall = 99.0f;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("SMART RC TELEMETRY DASHBOARD", 20, 20, 20, GREEN);
        DrawLine(20, 50, 780, 50, GREEN);

        DrawText("CONTROLLER INPUT:", 20, 80, 20, LIGHTGRAY);
        DrawText(TextFormat("Requested Speed: %d%%", (int)currentSpeed), 40, 110, 20, WHITE);
        if (isCruiseControlActive) DrawText("[CRUISE ON]", 250, 110, 15, SKYBLUE);
        DrawText(TextFormat("Requested Steer: %d deg", (int)currentSteering), 40, 140, 20, WHITE);

        DrawText("ACTUAL VEHICLE STATE:", 400, 80, 20, LIGHTGRAY);

        DrawText("Motor Power:", 420, 110, 15, WHITE);
        DrawRectangle(530, 110, 200, 15, DARKGRAY);

        if (myCar.speedPercent > 0) {
            Color powerColor = (myCar.speedPercent > 70) ? RED : (myCar.speedPercent > 30) ? YELLOW : GREEN;
            DrawRectangle(530, 110, myCar.speedPercent * 2, 15, powerColor);
        } else if (myCar.speedPercent < 0) {
            DrawRectangle(530, 110, abs(myCar.speedPercent) * 2, 15, SKYBLUE);
            DrawText("[ BRAKING ]", 740, 110, 15, RED);
        }
        DrawText(TextFormat("%d%%", myCar.speedPercent), 530 + 5, 110, 15, BLACK);

        DrawText("Steering:", 420, 140, 15, WHITE);
        int wheelX = 630;
        int wheelY = 147;
        Vector2 leftTire = { wheelX - cos(myCar.steeringAngle * DEG2RAD) * 30, wheelY - sin(myCar.steeringAngle * DEG2RAD) * 30 };
        Vector2 rightTire = { wheelX + cos(myCar.steeringAngle * DEG2RAD) * 30, wheelY + sin(myCar.steeringAngle * DEG2RAD) * 30 };

        DrawLineEx(leftTire, rightTire, 8.0f, YELLOW);
        DrawCircle(wheelX, wheelY, 4, WHITE);
        DrawText(TextFormat("%d deg", myCar.steeringAngle), 680, 140, 15, YELLOW);

        DrawLine(20, 180, 780, 180, GREEN);

        DrawText("SENSORS & SAFETY:", 20, 200, 20, LIGHTGRAY);
        DrawText(TextFormat("Active ADAS Mode: [%s]", brain.getCurrentStateName().c_str()), 400, 200, 20, SKYBLUE);

        Color batColor = (myCar.batteryLevel > 20.0f) ? GREEN : RED;
        DrawText(TextFormat("Battery: %.1f%% %s", myCar.batteryLevel, myCar.isRTHActive ? "(RTH ACTIVE)" : ""), 40, 230, 20, batColor);

        Color tempColor = (myCar.isOverheated) ? RED : (myCar.isTempWarning) ? YELLOW : ORANGE;
        DrawText(TextFormat("Motor Temp: %.1f C", myCar.motorTempC), 40, 260, 20, tempColor);

        int radarX = 550;
        int radarY = 360;
        int radarRadius = 120;

        DrawRectangleLines(radarX - 140, radarY - 140, 280, 160, DARKGREEN);
        DrawText("FRONT LIDAR SCANNER (AUTO)", radarX - 130, radarY - 130, 10, GREEN);

        DrawCircleLines(radarX, radarY, radarRadius, Fade(DARKGREEN, 0.5f));
        DrawCircleLines(radarX, radarY, radarRadius * 0.66f, Fade(DARKGREEN, 0.5f));
        DrawCircleLines(radarX, radarY, radarRadius * 0.33f, Fade(DARKGREEN, 0.5f));
        DrawLine(radarX, radarY, radarX - 100, radarY - 100, Fade(DARKGREEN, 0.5f));
        DrawLine(radarX, radarY, radarX + 100, radarY - 100, Fade(DARKGREEN, 0.5f));

        float time = GetTime();
        float sweepAngle = 270.0f + sin(time * 3.0f) * 60.0f;

        Vector2 endPoint = {
            radarX + cos(sweepAngle * DEG2RAD) * radarRadius,
            radarY + sin(sweepAngle * DEG2RAD) * radarRadius
        };
        DrawLineEx({(float)radarX, (float)radarY}, endPoint, 3.0f, LIME);

        if (isWallSpawned && myCar.distanceToWall <= 10.0f) {
            float wallDistancePx = (myCar.distanceToWall / 10.0f) * radarRadius;
            int blipY = radarY - (int)wallDistancePx;

            DrawLineEx({(float)(radarX - 30 + wallVisualOffsetX), (float)blipY},
                       {(float)(radarX + 30 + wallVisualOffsetX), (float)blipY}, 5.0f, RED);

            DrawText(TextFormat("%.1fm", myCar.distanceToWall), radarX + 45 + wallVisualOffsetX, blipY - 5, 15, RED);
            DrawText("OBSTACLE DETECTED!", radarX - 130, radarY - 110, 15, RED);
        } else {
            DrawText("PATH CLEAR", radarX - 130, radarY - 110, 15, LIME);
        }

        DrawLine(20, 390, 780, 390, GREEN);
        DrawText("SAFETY LOGS:", 20, 410, 20, LIGHTGRAY);

        Color logColor = (brain.getLastLog().find("OVERRIDE") != string::npos ||
                          brain.getLastLog().find("ALARM") != string::npos ||
                          brain.getLastLog().find("FATAL") != string::npos ||
                          brain.getLastLog().find("WARNING") != string::npos) ? RED : SKYBLUE;
        DrawText(brain.getLastLog().c_str(), 40, 440, 20, logColor);
        
        DrawText("Controls: [W][A][S][D]-Drive | [C]-Cruise Control | [O]-Wall | [H]-Heat", 20, 560, 15, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}