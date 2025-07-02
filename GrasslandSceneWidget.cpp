#include "GrasslandSceneWidget.h"
#include <QPainter>
#include <QKeyEvent>

GrasslandSceneWidget::GrasslandSceneWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(windowWidth, windowHeight);
    setFocusPolicy(Qt::StrongFocus);

    background.load(":/scene/data/scene/GrassLand.png"); // 確保圖片存在
    mapWidth = background.width();
    mapHeight = background.height();

    player = new Player(525, 1619);

    posLabel = new QLabel(this);
    posLabel->setStyleSheet("color: white; font-weight: bold; background-color: rgba(0,0,0,128);");
    posLabel->setGeometry(10, 10, 200, 20);
}

void GrasslandSceneWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.fillRect(rect(), Qt::black);

    int playerScreenX = windowWidth / 2 - player->getRect().width() / 2;
    int playerScreenY = windowHeight / 2 - player->getRect().height() / 2;

    int bgX = player->getX() + player->getRect().width() / 2 - windowWidth / 2;
    int bgY = player->getY() + player->getRect().height() / 2 - windowHeight / 2;

    QRect srcRect(
        std::max(0, bgX),
        std::max(0, bgY),
        std::min(windowWidth, mapWidth - bgX),
        std::min(windowHeight, mapHeight - bgY)
    );

    QRect targetRect(
        std::max(0, -bgX),
        std::max(0, -bgY),
        srcRect.width(),
        srcRect.height()
    );

    painter.drawPixmap(targetRect, background, srcRect);

    QPixmap currentPixmap = player->getCurrentPixmap();
    if (!currentPixmap.isNull()) {
        painter.drawPixmap(playerScreenX, playerScreenY, currentPixmap.scaled(player->getRect().size()));
    } else {
        painter.fillRect(playerScreenX, playerScreenY, player->getRect().width(), player->getRect().height(), Qt::red);
    }
}

void GrasslandSceneWidget::keyPressEvent(QKeyEvent *event) {
    int step = 5;
    player->setWalking(true);
    int dx = 0, dy = 0;
    if (event->key() == Qt::Key_Left) {
        player->setDirection(Player::LEFT);
        dx = -step;
    } else if (event->key() == Qt::Key_Right) {
        player->setDirection(Player::RIGHT);
        dx = step;
    } else if (event->key() == Qt::Key_Up) {
        player->setDirection(Player::UP);
        dy = -step;
    } else if (event->key() == Qt::Key_Down) {
        player->setDirection(Player::DOWN);
        QRect exitToTown(500, 1660, 50, 7); // 根據地圖調整
        if (exitToTown.intersects(player->getRect()) && event->key() == Qt::Key_Down) {
            emit returnToTown();
            return;
        }
        dy = step;
    }

    // 出口判斷: Grassland ➔ Town



    player->move(dx, dy, mapWidth, mapHeight);
    player->updateWalkFrame();
    posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();
}

void GrasslandSceneWidget::keyReleaseEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    player->setWalking(false);
    posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();

}
