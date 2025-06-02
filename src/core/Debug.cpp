#include "Debug.h"
#include <glad/glad.h>
#include <iostream>
#include <chrono>
#include <ctime>

namespace Debug {
    bool wireframeEnabled = false;
    bool showFPS = false;
    bool showDebugInfo = false;
    bool showCollisionBoxes = false;
    bool showChunkBoundaries = false;

    void toggleWireframe() {
        wireframeEnabled = !wireframeEnabled;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
    }

    bool isWireframeEnabled() {
        return wireframeEnabled;
    }

    void setWireframe(bool enabled) {
        wireframeEnabled = enabled;
        glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
    }

    void toggleFPS() {
        showFPS = !showFPS;
    }

    bool isFPSEnabled() {
        return showFPS;
    }

    void toggleDebugInfo() {
        showDebugInfo = !showDebugInfo;
    }

    bool isDebugInfoEnabled() {
        return showDebugInfo;
    }

    void toggleCollisionBoxes() {
        showCollisionBoxes = !showCollisionBoxes;
    }

    bool isCollisionBoxesEnabled() {
        return showCollisionBoxes;
    }

    void toggleChunkBoundaries() {
        showChunkBoundaries = !showChunkBoundaries;
    }

    bool isChunkBoundariesEnabled() {
        return showChunkBoundaries;
    }

    // Helper function to get current timestamp
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &time);
        std::string timestamp(buffer);
        timestamp = timestamp.substr(0, timestamp.length() - 1); // Remove newline
        return timestamp;
    }

    void log(const std::string& message) {
        std::cout << "[" << getCurrentTimestamp() << "] INFO: " << message << std::endl;
    }

    void logWarning(const std::string& message) {
        std::cout << "[" << getCurrentTimestamp() << "] WARNING: " << message << std::endl;
    }

    void logError(const std::string& message) {
        std::cerr << "[" << getCurrentTimestamp() << "] ERROR: " << message << std::endl;
    }
} 