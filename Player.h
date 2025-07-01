#pragma once

#include <QRect>
#include <QPixmap>
#include <QMap>

class Player {
public:
    enum Direction { UP, DOWN, LEFT, RIGHT };

    Player(int x, int y, int w = 35, int h = 48);

    void move(int dx, int dy, int mapWidth, int mapHeight);
    QRect getRect() const;
    int getX() const;
    int getY() const;

    void setDirection(Direction dir);
    void setWalking(bool walking);
    void updateWalkFrame();

    QPixmap getCurrentPixmap() const;

private:
    int x, y;
    int width, height;

    Direction direction;
    bool isWalking;
    int walkFrame; // 0 或 1

    QMap<QString, QPixmap> pixmaps;
};
