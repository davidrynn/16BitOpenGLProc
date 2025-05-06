// MockChunkFactory.h
#pragma once
#include "IChunkFactory.h"
#include "Chunk.h"

class MockChunkFactory : public IChunkFactory {
public:
    std::shared_ptr<Chunk> createChunk(int x, int z, std::shared_ptr<Terrain> terrain) override {
        // Just return a placeholder Chunk without any mesh work
        return std::make_shared<Chunk>(x, z, terrain, false);
    }
};
 