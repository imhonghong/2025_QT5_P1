#pragma once

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QLabel>
#include "Player.h"
#include "Barrier.h"

class LabSceneWidget : public QWidget {
    Q_OBJECT

public:
    explicit LabSceneWidget(QWidget *parent = nullptr);


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

    QVector<Barrier*> barriers;
    QLabel *posLabel;

    void addLabBarrier();
};
