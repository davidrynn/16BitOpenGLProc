#include "Entity.h"

Entity::Entity(glm::vec3 position) : position(position) {}

glm::vec3 Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}
