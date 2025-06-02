#pragma once
#include <string>
#include <memory>

struct WindowConfig {
    int width = 1280;
    int height = 720;
    std::string title = "16BitCraft";
    bool fullscreen = false;
    bool vsync = true;
};

struct InputConfig {
    float mouseSensitivity = 0.1f;
    float moveSpeed = 5.0f;
    bool invertY = false;
};

struct GraphicsConfig {
    int shadowMapSize = 1024;
    float renderDistance = 1000.0f;
    bool enableVsync = true;
    int maxFPS = 144;
};

struct GameConfig {
    int chunkSize = 16;
    int viewDistance = 8;
    float terrainScale = 1.0f;
};

class Config {
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    WindowConfig window;
    InputConfig input;
    GraphicsConfig graphics;
    GameConfig game;

    // Getter methods for window config
    int getInt(const std::string& key) const {
        if (key == "window.width") return window.width;
        if (key == "window.height") return window.height;
        if (key == "graphics.shadowMapSize") return graphics.shadowMapSize;
        if (key == "graphics.maxFPS") return graphics.maxFPS;
        if (key == "game.chunkSize") return game.chunkSize;
        if (key == "game.viewDistance") return game.viewDistance;
        return 0; // Default value
    }

    float getFloat(const std::string& key) const {
        if (key == "input.mouseSensitivity") return input.mouseSensitivity;
        if (key == "input.moveSpeed") return input.moveSpeed;
        if (key == "graphics.renderDistance") return graphics.renderDistance;
        if (key == "game.terrainScale") return game.terrainScale;
        return 0.0f; // Default value
    }

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

private:
    Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
}; 