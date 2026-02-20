//
// Created by Rodney Brown on 2/20/26.
//

#ifndef INC_2DP_COMPONENT_H
#define INC_2DP_COMPONENT_H
#include "ECS.h"

class PositionComponent : public Component {
public:
    int x() {
        return xpos;
    }
    int y() {
        return ypos;
    }
    void init() override {
        xpos = 0;
        ypos = 0;
    }
    void update() override {
        xpos++;
        ypos++;
    }
    void setPos(int x, int y) {
        xpos = x;
        ypos = y;
    }
    private:
        int xpos;
        int ypos;


};
#endif //INC_2DP_COMPONENT_H