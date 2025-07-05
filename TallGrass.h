#pragma once
#include "Barrier.h"
#include <QRect>

class TallGrass : public Barrier {
public:
    TallGrass(int x, int y, int width, int height);

    bool isPassable(int nextX, int nextY, int direction) const override { return true; }
    bool hasTriggered = false;
    QRect getTriggerRect() const { return triggerRect; }

private:
    QRect triggerRect; // 隱形觸發方塊
    void generateTriggerRect();
};
