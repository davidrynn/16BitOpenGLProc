#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <atomic>
#include <memory>
#include <unordered_set>
#include <utility>
#include <chrono>
#include "Terrain.h"

// Hash function for chunk coordinates
struct ChunkPairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class TerrainThreadPool {
public:
    TerrainThreadPool(size_t numThreads = std::thread::hardware_concurrency() - 1)
        : shouldStop(false)
        , lastUploadTime(std::chrono::steady_clock::now())
        , uploadBudgetMs(16.0f) // Target ~60 FPS
        , chunkUpdateCooldown(100) // 100ms cooldown between updates for same chunk
    {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this, i] { workerFunction(i); });
        }
    }

    virtual ~TerrainThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            shouldStop = true;
        }
        condition.notify_all();
        for (auto& worker : workers) {
            worker.join();
        }
    }

    virtual void queueChunkUpdate(int chunkX, int chunkZ, std::shared_ptr<Terrain> terrain) {
        // Skip if too many pending uploads
        if (getPendingUploadCount() > 32) {
            return;
        }

        // Check cooldown for this chunk
        auto now = std::chrono::steady_clock::now();
        auto key = std::make_pair(chunkX, chunkZ);
        {
            std::lock_guard<std::mutex> lock(processingMutex);
            auto lastUpdate = lastUpdateTime.find(key);
            if (lastUpdate != lastUpdateTime.end()) {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - lastUpdate->second).count();
                if (elapsed < chunkUpdateCooldown) {
                    return; // Skip if too soon
                }
            }
        }

        // Check if this chunk is already being processed
        {
            std::lock_guard<std::mutex> lock(processingMutex);
            if (processingChunks.find(key) != processingChunks.end()) {
                return; // Skip if already being processed
            }
            processingChunks.insert(key);
            lastUpdateTime[key] = now;
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.push([this, chunkX, chunkZ, terrain] {
                auto chunk = terrain->getChunks().find({chunkX, chunkZ});
                if (chunk != terrain->getChunks().end()) {
                    try {
                        // Only generate if not already uploaded
                        if (!chunk->second->isUploaded()) {
                            chunk->second->generate();
                            
                            // Queue for GPU upload on main thread
                            {
                                std::lock_guard<std::mutex> uploadLock(uploadMutex);
                                uploadQueue.push({chunk->second, std::chrono::steady_clock::now()});
                            }
                        }
                    }
                    catch (const std::exception&) {
                        // Handle error silently
                    }
                }

                // Remove from processing set
                {
                    std::lock_guard<std::mutex> lock(processingMutex);
                    processingChunks.erase({chunkX, chunkZ});
                }
            });
        }
        condition.notify_one();
    }

    // Call this from the main thread to process GPU uploads
    virtual void processUploads() {
        using namespace std::chrono;

        auto currentTime = steady_clock::now();
        float elapsedMs = duration_cast<duration<float, std::milli>>(currentTime - lastUploadTime).count();
        if (elapsedMs < uploadBudgetMs) {
            return;
        }

        std::vector<std::shared_ptr<Chunk>> chunksToUpload;
        {
            std::lock_guard<std::mutex> lock(uploadMutex);
            
            // Increased chunks per frame
            const size_t maxChunksPerFrame = 4;  // Was 2
            size_t count = 0;
            
            auto now = steady_clock::now();
            while (!uploadQueue.empty() && count < maxChunksPerFrame) {
                auto& [chunk, timestamp] = uploadQueue.front();
                
                // Reduced minimum age requirement
                auto age = duration_cast<milliseconds>(now - timestamp).count();
                if (age < 8) { // Was 16ms
                    break;
                }
                
                chunksToUpload.push_back(chunk);
                uploadQueue.pop();
                count++;
            }
        }

        if (!chunksToUpload.empty()) {
            auto uploadStart = steady_clock::now();
            
            for (const auto& chunk : chunksToUpload) {
                try {
                    if (!chunk->isUploaded()) {
                        chunk->uploadToGPU();
                    }
                }
                catch (const std::exception&) {
                    // Handle error silently
                }
            }
            
            lastUploadTime = steady_clock::now();
            auto uploadDuration = duration_cast<duration<float, std::milli>>(lastUploadTime - uploadStart).count();
            
            // Adjust upload budget based on actual upload time
            uploadBudgetMs = std::max(16.0f, uploadDuration * 1.5f);
        }
    }

    // Check if a chunk is currently being processed
    virtual bool isChunkProcessing(int x, int z) const {
        std::lock_guard<std::mutex> lock(processingMutex);
        return processingChunks.find({x, z}) != processingChunks.end();
    }

    // Get number of chunks waiting to be uploaded
    virtual size_t getPendingUploadCount() const {
        std::lock_guard<std::mutex> lock(uploadMutex);
        return uploadQueue.size();
    }

private:
    void workerFunction(size_t threadId) {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this] {
                    return shouldStop || !tasks.empty();
                });
                
                if (shouldStop && tasks.empty()) {
                    return;
                }
                
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

    std::queue<std::function<void()>> tasks;
    std::queue<std::pair<std::shared_ptr<Chunk>, std::chrono::steady_clock::time_point>> uploadQueue;
    std::vector<std::thread> workers;
    std::mutex queueMutex;
    mutable std::mutex uploadMutex;
    mutable std::mutex processingMutex;
    std::condition_variable condition;
    std::atomic<bool> shouldStop;
    std::unordered_set<std::pair<int, int>, ChunkPairHash> processingChunks;
    
    // Timing control for GPU uploads
    std::chrono::steady_clock::time_point lastUploadTime;
    float uploadBudgetMs;

    // Add these members
    std::unordered_map<std::pair<int, int>, std::chrono::steady_clock::time_point, ChunkPairHash> lastUpdateTime;
    const int chunkUpdateCooldown; // milliseconds
}; 