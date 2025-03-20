#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

class Entity {
public:
    Entity(glm::vec3 position);
    virtual ~Entity() = default;

    virtual void update() = 0;  // Make this an abstract function
    virtual void draw() = 0;

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3& newPosition);

protected:
    glm::vec3 position;
};

#endif
