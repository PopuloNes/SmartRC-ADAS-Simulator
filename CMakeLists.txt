cmake_minimum_required(VERSION 3.27)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(SmartRC_Simulator)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)
file(GLOB SOURCES "src/*.cpp")
add_executable(SmartRC_Simulator ${SOURCES}
        include/ThermalSystem.h
        "src/ThermalSystem.cpp .cpp")

# Подключаем Raylib к нашей программе
target_link_libraries(SmartRC_Simulator PRIVATE raylib)