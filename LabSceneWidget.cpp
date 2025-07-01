#include "LabSceneWidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

LabSceneWidget::LabSceneWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(windowWidth, windowHeight);
    setFocusPolicy(Qt::StrongFocus);

    background.load(":/scene/data/scene/lab.png");
    mapWidth = background.width();
    mapHeight = background.height();

    player = new Player(100, 100); // 初始位置
}

void LabSceneWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // 計算鏡頭 offset
    int offsetX = player->getX() + player->getRect().width() / 2 - windowWidth / 2;
    int offsetY = player->getY() + player->getRect().height() / 2 - windowHeight / 2;

    // 限制 offset 不超出地圖範圍
    offsetX = std::max(0, std::min(offsetX, mapWidth - windowWidth));
    offsetY = std::max(0, std::min(offsetY, mapHeight - windowHeight));

    // 顯示背景對應區域
    QRect sourceRect(offsetX, offsetY, windowWidth, windowHeight);
    painter.drawPixmap(0, 0, background, sourceRect.x(), sourceRect.y(), sourceRect.width(), sourceRect.height());

    // 畫玩家（顯示在中央或靠近邊界）
    int playerScreenX = player->getX() - offsetX;
    int playerScreenY = player->getY() - offsetY;

    QPixmap currentPixmap = player->getCurrentPixmap();
    if (!currentPixmap.isNull()) {
        painter.drawPixmap(playerScreenX, playerScreenY, currentPixmap.scaled(player->getRect().size()));
    } else {
        painter.fillRect(playerScreenX, playerScreenY, player->getRect().width(), player->getRect().height(), Qt::red);
    }
}

void LabSceneWidget::keyPressEvent(QKeyEvent *event) {
    int step = 5;
    player->setWalking(true);
    if (event->key() == Qt::Key_Left) {
        player->setDirection(Player::LEFT);
        player->move(-step, 0, mapWidth, mapHeight);
    } else if (event->key() == Qt::Key_Right) {
        player->setDirection(Player::RIGHT);
        player->move(step, 0, mapWidth, mapHeight);
    } else if (event->key() == Qt::Key_Up) {
        player->setDirection(Player::UP);
        player->move(0, -step, mapWidth, mapHeight);
    } else if (event->key() == Qt::Key_Down) {
        player->setDirection(Player::DOWN);
        player->move(0, step, mapWidth, mapHeight);
    }
    player->updateWalkFrame(); // 切換動畫幀
    qDebug() << "Key pressed:" << event->key();
    update();
}

void LabSceneWidget::keyReleaseEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    player->setWalking(false);
    update();
}

