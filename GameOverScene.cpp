#include "GameOverScene.h"
#include <QPainter>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QTimer>

GameOverScene::GameOverScene(QWidget *parent) : QWidget(parent) {
    setFixedSize(525, 450);

    QLabel *label = new QLabel("Game Over\nPress any key to exit.", this);
    label->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(0, 0, width(), height());

    setFocusPolicy(Qt::StrongFocus);
    QTimer::singleShot(0, this, [this]() { this->setFocus(); });
}

void GameOverScene::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap bg(":/scene/data/scene/game_over.png");
    painter.drawPixmap(0, 0, width(), height(), bg);
}

void GameOverScene::keyPressEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    QCoreApplication::quit();
}
