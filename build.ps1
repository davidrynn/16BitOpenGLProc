# build.ps1 - Generate Visual Studio solution & build in Debug mode

$buildDir = "build"
$toolchainFile = "C:/Users/david/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Clear build folder
if (Test-Path $buildDir) {
    Remove-Item -Recurse -Force $buildDir
}

# Run CMake with Visual Studio generator (this will generate .sln and .vcxproj files)
cmake -S . -B $buildDir `
    -G "Visual Studio 17 2022" `
    -A x64 `
    -DCMAKE_TOOLCHAIN_FILE="$toolchainFile"

# Build using MSBuild (calls msbuild on the generated .sln)
cmake --build $buildDir --config Debug
