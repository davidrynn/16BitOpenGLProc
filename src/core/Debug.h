#ifndef DEBUG_H
#define DEBUG_H

#include <string>

namespace Debug {
    // Debug state
    extern bool wireframeEnabled;
    extern bool showFPS;
    extern bool showDebugInfo;
    extern bool showCollisionBoxes;
    extern bool showChunkBoundaries;

    // Debug controls
    void toggleWireframe();
    bool isWireframeEnabled();
    void setWireframe(bool enabled);

    void toggleFPS();
    bool isFPSEnabled();
    
    void toggleDebugInfo();
    bool isDebugInfoEnabled();
    
    void toggleCollisionBoxes();
    bool isCollisionBoxesEnabled();
    
    void toggleChunkBoundaries();
    bool isChunkBoundariesEnabled();

    // Debug logging
    void log(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);
}

#endif