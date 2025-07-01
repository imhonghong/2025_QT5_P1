#pragma once
#include "Barrier.h"

class SolidBarrier : public Barrier {
public:
    SolidBarrier(int x, int y, int w, int h) : Barrier(x, y, w, h) {}

    bool isPassable(int playerX, int playerY, int direction) const override {
        Q_UNUSED(direction);
        return !rect.intersects(QRect(playerX, playerY, 35, 48)); // 玩家大小
    }
};
