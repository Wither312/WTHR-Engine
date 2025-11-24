// Platform.cpp
#include "Platform.hpp"
#include <spdlog/spdlog.h>

void Platform::Init() {
    spdlog::info("Platform initialization");
}

void Platform::Shutdown() {
    spdlog::info("Platform shutdown");
}
