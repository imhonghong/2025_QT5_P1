#pragma once

#include "Barrier.h"
#include <QPixmap>
#include <QStringList>

class NPCBarrier : public Barrier {
public:
    NPCBarrier(int x, int y, int w, int h, const QStringList &dialogues, const QString &pixmapPath);

    bool isPassable(int playerX, int playerY, int direction) const override;
    bool canInteract(int playerX, int playerY) const override;
    void interact(QWidget *parent);

    void draw(QPainter *painter, int offsetX, int offsetY) const;
    void drawAt(QPainter *painter, int x, int y) const;

private:
    QPixmap npcPixmap;
    QStringList dialogues;
};
