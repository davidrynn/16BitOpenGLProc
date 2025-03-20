#!/bin/bash

# Navigate to the root of the project
cd "$(dirname "$0")"

echo "🚀 Cleaning and rebuilding 16BitCraft..."

# Remove existing build directory
rm -rf build

# Create a new build directory
mkdir build && cd build

# Run CMake and Make
cmake ..
make -j$(nproc)  # Use multiple cores for faster compilation

echo "✅ Build complete!"

# Run the executable if build succeeds
if [ $? -eq 0 ]; then
    echo "🎮 Running 16BitCraft..."
    ./16BitCraft
else
    echo "❌ Build failed! Check errors above."
fi

