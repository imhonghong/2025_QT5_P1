#include "NPCBarrier.h"
#include "DialogWidget.h"
#include <QPainter>

NPCBarrier::NPCBarrier(int x, int y, int w, int h, const QStringList &dialogues, const QString &pixmapPath)
    : Barrier(x, y, w, h),
      npcPixmap(pixmapPath),
      dialogues(dialogues) {}

bool NPCBarrier::isPassable(int playerX, int playerY, int direction) const {
    Q_UNUSED(direction);
    return !rect.intersects(QRect(playerX, playerY, 35, 48));
}

bool NPCBarrier::canInteract(int playerX, int playerY) const {
    QRect playerRect(playerX, playerY, 35, 48);
    QRect interactZone = rect.adjusted(-10, -10, 10, 10);
    return interactZone.intersects(playerRect);
}

void NPCBarrier::interact(QWidget *parent) {
    new DialogWidget(dialogues, parent);
}

void NPCBarrier::draw(QPainter *painter, int offsetX, int offsetY) const {
    if (!npcPixmap.isNull()) {
        int drawX = rect.x() - offsetX;
        int drawY = rect.y() - offsetY;
        painter->drawPixmap(drawX, drawY, rect.width(), rect.height(), npcPixmap);
    }
}

void NPCBarrier::drawAt(QPainter *painter, int x, int y) const {
    if (!npcPixmap.isNull()) {
        painter->drawPixmap(x, y, rect.width(), rect.height(), npcPixmap);
    }
}
