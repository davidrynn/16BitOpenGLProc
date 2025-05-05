#pragma once
#include <memory>

class Chunk;
class Terrain;

class IChunkFactory {
public:
    virtual ~IChunkFactory() = default;
    virtual std::shared_ptr<Chunk> createChunk(int x, int z, std::shared_ptr<Terrain> terrain) = 0;
};
