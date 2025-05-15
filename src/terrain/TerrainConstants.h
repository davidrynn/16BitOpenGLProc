#ifndef TERRAIN_CONSTANTS_H
#define TERRAIN_CONSTANTS_H

namespace TerrainConstants {

    // Controls how far a biome influences terrain before blending into others
    constexpr float BIOME_BLEND_RADIUS = 50.0f;

    constexpr int INITIAL_CHUNK_RADIUS = 5;

    // Number of biomes the world starts with
    constexpr int DEFAULT_BIOME_COUNT = 4;

    // Dimensions of the biome region map (used to randomly place biome centers)
    constexpr int BIOME_WORLD_WIDTH = 1000;
    constexpr int BIOME_WORLD_HEIGHT = 1000;

    // Rendering distance
    constexpr float TERRAIN_RENDER_DISTANCE = 70.0f;

    // View distance?
    constexpr int VIEW_DISTANCE = 10;

}

#endif
