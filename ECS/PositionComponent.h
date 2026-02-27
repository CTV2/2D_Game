// Implements an ECS component that stores and exposes entity 2D position values.

#ifndef INC_2DP_POSITIONCOMPONENT_H
#define INC_2DP_POSITIONCOMPONENT_H
#include "Component.h"
class PositionComponent : public Component {
public:
    PositionComponent() {
        xpos = 0;
        ypos = 0;
    }
    PositionComponent(float x, float y) {
        xpos = x;
        ypos = y;
    }
    float x() {
        return xpos;
    }
    float y() {
        return ypos;
    }
    void init() override {
        // Keep constructor/setPos values; do not override on init.
    }
    void update() override {
        // Position is externally controlled.
    }
    void setPos(float x, float y) {
        xpos = x;
        ypos = y;
    }
private:
    float xpos;
    float ypos;
};

#endif //INC_2DP_POSITIONCOMPONENT_H
