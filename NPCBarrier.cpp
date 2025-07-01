#include "NPCBarrier.h"
#include <QDebug>
#include <QPainter>

NPCBarrier::NPCBarrier(int x, int y, int w, int h)
    : Barrier(x, y, w, h), npcPixmap(":/other/data/NPC.png") {}

bool NPCBarrier::isPassable(int playerX, int playerY, int direction) const {
    Q_UNUSED(direction);
    return !rect.intersects(QRect(playerX, playerY, 35, 48));
}

bool NPCBarrier::canInteract(int playerX, int playerY) const {
    return rect.intersects(QRect(playerX, playerY, 35, 48));
}

void NPCBarrier::interact() {
    qDebug() << "NPC: Hello, I am Professor Oak!";
}

void NPCBarrier::draw(QPainter *painter, int offsetX, int offsetY) const{
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
