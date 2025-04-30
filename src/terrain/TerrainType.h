#ifndef TERRAIN_TYPE_H
#define TERRAIN_TYPE_H
#include <functional>
enum class TerrainType {
    Plains,
    Mountains,
    Desert,
    Snow,
    Count
    // Add more as needed
};
namespace std {
    template <>
    struct hash<TerrainType> {
        std::size_t operator()(const TerrainType& type) const noexcept {
            return std::hash<std::underlying_type_t<TerrainType>>{}(
                static_cast<std::underlying_type_t<TerrainType>>(type)
            );
        }
    };
}
#endif
