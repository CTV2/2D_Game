//
// Created by Rodney Brown on 2/20/26.
//

#ifndef INC_2DP_POSITIONCOMPONENT_H
#define INC_2DP_POSITIONCOMPONENT_H
#include "Component.h"
class PositionComponent : public Component {
public:
    PositionComponent() {
        xpos = 0;
        ypos = 0;
    }
    PositionComponent(int x, int y) {
        xpos = x;
        ypos = y;
    }
    int x() {
        return xpos;
    }
    int y() {
        return ypos;
    }
    void init() override {
        // Keep constructor/setPos values; do not override on init.
    }
    void update() override {
        // Position is externally controlled.
    }
    void setPos(int x, int y) {
        xpos = x;
        ypos = y;
    }
private:
    int xpos;
    int ypos;
};

#endif //INC_2DP_POSITIONCOMPONENT_H
