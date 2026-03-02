// Implements an ECS collider component for simple AABB collision checks.

#ifndef INC_2DP_COLLIDERCOMPONENT_H
#define INC_2DP_COLLIDERCOMPONENT_H

#include "Component.h"
#include "PositionComponent.h"
#include "SDL3/SDL.h"

class ColliderComponent : public Component {
public:
    // takes the component and  sets a hitbox based on Position and size of component
    ColliderComponent(float width = 0, float height =0, float offsetX = 0.0f, float offsetY = 0.0f)
        : w(width), h(height), ox(offsetX), oy(offsetY) {
    }

    void init() override {
        position = &entity->getComponent<PositionComponent>();
        rect = {position->x() + ox, position->y() + oy, w, h};
    }

    void update() override {
        rect.x = position->x() + ox;
        rect.y = position->y() + oy;
    }

    const SDL_FRect& getCollider() const {
        return rect;
    }

private:
    PositionComponent* position = nullptr;
    SDL_FRect rect{};
    float w = 0.0f;
    float h = 0.0f;
    float ox = 0.0f;
    float oy = 0.0f;
};

#endif //INC_2DP_COLLIDERCOMPONENT_H
