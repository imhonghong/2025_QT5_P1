#include "TallGrass.h"
#include <QRandomGenerator>
#include <QDebug>

TallGrass::TallGrass(int x, int y, int width, int height)
    : Barrier(x, y, width, height) {
    generateTriggerRect();
}

void TallGrass::generateTriggerRect() {
    int maxX = rect.width() - 40;
    int maxY = rect.height() - 40;
    int offsetX = QRandomGenerator::global()->bounded(0, maxX + 1);
    int offsetY = QRandomGenerator::global()->bounded(0, maxY + 1);
    triggerRect = QRect(rect.x() + offsetX, rect.y() + offsetY, 40, 40);

    qDebug() << "TallGrass triggerRect at:" << triggerRect;
}
