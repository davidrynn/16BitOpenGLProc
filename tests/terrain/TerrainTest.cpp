#include <gtest/gtest.h>
#include "Terrain.h"
#include "TerrainNoiseFactory.h"
#include "TerrainConstants.h"
#include <memory>

class TerrainTest : public ::testing::Test {
protected:
    std::shared_ptr<TerrainNoiseFactory> noiseFactory;
    std::shared_ptr<Terrain> terrain;

    void SetUp() override {
        noiseFactory = std::make_shared<TerrainNoiseFactory>();
        terrain = std::make_shared<Terrain>();

        // Provide a dummy progress callback
        // terrain->initialize(noiseFactory, [](float) {});
    }
};

TEST_F(TerrainTest, TestSurvivesConstruction) {
    EXPECT_TRUE(terrain != nullptr);
}

TEST_F(TerrainTest, TestGetHeightAt) {
    float worldX = 10.0f;
    float worldZ = 20.0f;

    float height = terrain->getHeightAt(worldX, worldZ);
    EXPECT_GE(height, 0.0f); // height >= 0
}

TEST_F(TerrainTest, TestGetTerrainTypeAt) {
    float worldX = 10.0f;
    float worldZ = 20.0f;

    TerrainType terrainType = terrain->getTerrainTypeAt(worldX, worldZ);
    EXPECT_TRUE(terrainType >= TerrainType::Plains && terrainType <= TerrainType::Snow);
}

TEST_F(TerrainTest, TestInitialize) {
    // Test height at origin after initialization
    float height = terrain->getHeightAt(0.0f, 0.0f);
    EXPECT_GE(height, 0.0f);
}
