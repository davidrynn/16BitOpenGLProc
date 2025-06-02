#pragma once

#include "TerrainThreadPool.h"
#include <memory>

class MockTerrainThreadPool : public TerrainThreadPool {
public:
    MockTerrainThreadPool() : TerrainThreadPool(1) {} // Single thread for testing

    // Override methods to do nothing or minimal work
    void queueChunkUpdate(int chunkX, int chunkZ, std::shared_ptr<Terrain> terrain) override {
        // Do nothing in tests
    }

    void processUploads() override {
        // Do nothing in tests
    }

    bool isChunkProcessing(int x, int z) const override {
        return false;
    }

    size_t getPendingUploadCount() const override {
        return 0;
    }
}; 