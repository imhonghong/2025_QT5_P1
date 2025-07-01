#pragma once
#include <QRect>

class Barrier {
protected:
    QRect rect; // 在地圖座標系
public:
    Barrier(int x, int y, int w, int h) : rect(x, y, w, h) {}
    virtual ~Barrier() = default;

    QRect getRect() const { return rect; }

    // 玩家移動後會呼叫判斷是否可通過
    virtual bool isPassable(int playerX, int playerY, int direction) const = 0;

    // 是否可觸發互動
    virtual bool canInteract(int playerX, int playerY) const { return false; }

    // 實際觸發互動行為
    virtual void interact() {}
};
