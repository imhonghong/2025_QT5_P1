#pragma once
#include "Barrier.h"

class Ledge : public Barrier {
public:
    enum Direction { UP, DOWN, LEFT, RIGHT };
private:
    Direction allowedDirection;
public:
    Ledge(int x, int y, int w, int h, Direction dir)
        : Barrier(x, y, w, h), allowedDirection(dir) {}

    bool isPassable(int playerX, int playerY, int direction) const override {
        QRect playerRect(playerX, playerY, 35, 48);
        if (!rect.intersects(playerRect)) {
            return true;
        }
        return (direction == allowedDirection);
    }
};
