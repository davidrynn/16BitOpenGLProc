#include "Config.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void Config::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "No config file found, using defaults" << std::endl;
            return;
        }

        json j;
        file >> j;

        // Window config
        if (j.contains("window")) {
            auto& w = j["window"];
            window.width = w.value("width", window.width);
            window.height = w.value("height", window.height);
            window.title = w.value("title", window.title);
            window.fullscreen = w.value("fullscreen", window.fullscreen);
            window.vsync = w.value("vsync", window.vsync);
        }

        // Input config
        if (j.contains("input")) {
            auto& i = j["input"];
            input.mouseSensitivity = i.value("mouseSensitivity", input.mouseSensitivity);
            input.moveSpeed = i.value("moveSpeed", input.moveSpeed);
            input.invertY = i.value("invertY", input.invertY);
        }

        // Graphics config
        if (j.contains("graphics")) {
            auto& g = j["graphics"];
            graphics.shadowMapSize = g.value("shadowMapSize", graphics.shadowMapSize);
            graphics.renderDistance = g.value("renderDistance", graphics.renderDistance);
            graphics.enableVsync = g.value("enableVsync", graphics.enableVsync);
            graphics.maxFPS = g.value("maxFPS", graphics.maxFPS);
        }

        // Game config
        if (j.contains("game")) {
            auto& g = j["game"];
            game.chunkSize = g.value("chunkSize", game.chunkSize);
            game.viewDistance = g.value("viewDistance", game.viewDistance);
            game.terrainScale = g.value("terrainScale", game.terrainScale);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        std::cerr << "Using default values" << std::endl;
    }
}

void Config::saveToFile(const std::string& filename) const {
    try {
        json j;

        // Window config
        j["window"] = {
            {"width", window.width},
            {"height", window.height},
            {"title", window.title},
            {"fullscreen", window.fullscreen},
            {"vsync", window.vsync}
        };

        // Input config
        j["input"] = {
            {"mouseSensitivity", input.mouseSensitivity},
            {"moveSpeed", input.moveSpeed},
            {"invertY", input.invertY}
        };

        // Graphics config
        j["graphics"] = {
            {"shadowMapSize", graphics.shadowMapSize},
            {"renderDistance", graphics.renderDistance},
            {"enableVsync", graphics.enableVsync},
            {"maxFPS", graphics.maxFPS}
        };

        // Game config
        j["game"] = {
            {"chunkSize", game.chunkSize},
            {"viewDistance", game.viewDistance},
            {"terrainScale", game.terrainScale}
        };

        std::ofstream file(filename);
        file << std::setw(4) << j << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving config: " << e.what() << std::endl;
    }
} 