#include "BoxBarrier.h"
#include "DialogWidget.h"
#include <QPainter>

BoxBarrier::BoxBarrier(int x, int y, int w, int h, const QString &itemName, int count, const QString &pixmapPath)
    : Barrier(x, y, w, h),
      itemName(itemName),
      count(count),
      collected(false),
      boxPixmap(pixmapPath) {}

bool BoxBarrier::isPassable(int playerX, int playerY, int direction) const {
    Q_UNUSED(direction);
    if (collected) return true;
    return !rect.intersects(QRect(playerX, playerY, 35, 48));
}

bool BoxBarrier::canInteract(int playerX, int playerY) const {
    if (collected) return false;
    QRect playerRect(playerX, playerY, 35, 48);
    QRect interactZone = rect.adjusted(-10, -10, 10, 10);
    return interactZone.intersects(playerRect);
}

void BoxBarrier::interact(QWidget *parent, Bag *bag) {
    if (collected) return;
    bag->addItem(itemName, count);
    new DialogWidget(QStringList({"You found a " + itemName + "!"}), parent, true);
    collected = true;
}

void BoxBarrier::draw(QPainter *painter, int offsetX, int offsetY) const {
    if (!collected && !boxPixmap.isNull()) {
        int drawX = rect.x() - offsetX;
        int drawY = rect.y() - offsetY;
        painter->drawPixmap(drawX, drawY, rect.width(), rect.height(), boxPixmap);
    }
}

void BoxBarrier::drawAt(QPainter *painter, int x, int y) const {
    if (!collected && !boxPixmap.isNull()) {
        painter->drawPixmap(x, y, rect.width(), rect.height(), boxPixmap);
    }
}

bool BoxBarrier::isCollected() const {
    return collected;
}
