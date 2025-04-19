# Set VCPKG toolchain path
$toolchain = "C:/Users/david/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Store root path
$rootDir = Get-Location

# Clean up old build directory
Write-Host "Removing old build directory..."
Remove-Item -Recurse -Force -ErrorAction SilentlyContinue "build"

# Create build directory
New-Item -ItemType Directory -Force -Path "build" | Out-Null
Set-Location "build"

# Run CMake with proper toolchain
Write-Host "Running CMake with Visual Studio generator..."
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="$toolchain"

if ($LASTEXITCODE -ne 0) {
    Write-Error "❌ CMake configuration failed."
    exit 1
}

# Build project using full path to MSBuild
Write-Host "Building with MSBuild..."
& "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" ALL_BUILD.vcxproj /p:Configuration=Debug

if ($LASTEXITCODE -ne 0) {
    Write-Error "❌ Build failed."
    exit 1
}

# Return to root directory to run the executable
Set-Location $rootDir

# Run the application from build output
$exePath = ".\\build\\Debug\\16BitCraft.exe"

if (Test-Path $exePath) {
    Write-Host "🎮 Running 16BitCraft..."
    & $exePath
} else {
    Write-Error "❌ Executable not found: $exePath"
}
