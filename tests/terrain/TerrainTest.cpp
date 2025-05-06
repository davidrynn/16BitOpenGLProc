#include <gtest/gtest.h>
#include "Terrain.h"
#include "TerrainNoiseFactory.h"
#include "MockChunkFactory.h"

class TerrainTest : public ::testing::Test {
protected:
    std::shared_ptr<Terrain> terrain;
    std::shared_ptr<TerrainNoiseFactory> noiseFactory;

    void SetUp() override {
        noiseFactory = std::make_shared<TerrainNoiseFactory>();
        // terrain is constructed in each test individually
    }
};
 
TEST_F(TerrainTest, TestInitialize) {
    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>();
    terrain->setChunkFactory(std::make_shared<MockChunkFactory>());

    float lastProgress = 0.0f;
    terrain->initialize(noiseFactory, [&](float progress) {
        lastProgress = progress;
    });

    const auto& chunks = terrain->getChunks();
    EXPECT_FALSE(chunks.empty());
    EXPECT_GT(lastProgress, 0.0f);
}

TEST_F(TerrainTest, TestSurvivesConstruction) {
    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>();
    SUCCEED(); // No crash means pass
}

TEST_F(TerrainTest, TestGetHeightAt) {
    terrain = std::make_shared<Terrain>();
    terrain->setChunkFactory(std::make_shared<MockChunkFactory>());
    terrain->initialize(noiseFactory, nullptr);

    float height = terrain->getHeightAt(0.0f, 0.0f);
    EXPECT_GE(height, 0.0f);  // Basic sanity check
}

TEST_F(TerrainTest, TestGetTerrainTypeAt) {
    terrain = std::make_shared<Terrain>();
    terrain->setChunkFactory(std::make_shared<MockChunkFactory>());
    terrain->initialize(std::make_shared<TerrainNoiseFactory>(), nullptr);
    TerrainType type = terrain->getTerrainTypeAt(10.0f, 10.0f);
    EXPECT_TRUE(type == TerrainType::Plains || type == TerrainType::Mountains ||
                type == TerrainType::Desert || type == TerrainType::Snow);
}

TEST_F(TerrainTest, TestInitializeWithChunks) {
    terrain = std::make_shared<Terrain>();
    terrain->setChunkFactory(std::make_shared<MockChunkFactory>());
    terrain->initialize(noiseFactory, nullptr);

    const auto& chunks = terrain->getChunks();
    EXPECT_FALSE(chunks.empty());
}

TEST_F(TerrainTest, TestInitializeGetHeightAt) {
    terrain = std::make_shared<Terrain>();
    terrain->setChunkFactory(std::make_shared<MockChunkFactory>());
    terrain->initialize(noiseFactory, nullptr);

    float height = terrain->getHeightAt(5.0f, 5.0f);
    EXPECT_GE(height, 0.0f);
}
