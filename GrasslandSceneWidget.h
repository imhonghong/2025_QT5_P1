#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include "Player.h"

class GrasslandSceneWidget : public QWidget {
    Q_OBJECT

public:
    explicit GrasslandSceneWidget(QWidget *parent = nullptr);

signals:
    void returnToTown();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QPixmap background;
    Player *player;

    const int windowWidth = 525;
    const int windowHeight = 450;
    int mapWidth;
    int mapHeight;

    QLabel *posLabel;
};
