#pragma once
#include "IChunkFactory.h"
#include "Chunk.h"

class DefaultChunkFactory : public IChunkFactory {
public:
    std::shared_ptr<Chunk> createChunk(int x, int z, std::shared_ptr<Terrain> terrain) override {
        return std::make_shared<Chunk>(x, z, terrain);
    }
};
