#pragma once

#include "Barrier.h"
#include "Bag.h"
#include <QPixmap>

class BoxBarrier : public Barrier {
public:
    BoxBarrier(int x, int y, int w, int h, const QString &itemName, int count, const QString &pixmapPath);

    bool isPassable(int playerX, int playerY, int direction) const override;
    bool canInteract(int playerX, int playerY) const override;
    void interact(QWidget *parent, Bag *bag);

    void draw(QPainter *painter, int offsetX, int offsetY) const;
    void drawAt(QPainter *painter, int x, int y) const;

    bool isCollected() const;

private:
    QString itemName;
    int count;
    bool collected;
    QPixmap boxPixmap;
};
