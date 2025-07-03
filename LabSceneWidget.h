#pragma once

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QLabel>
#include "Player.h"
#include "Barrier.h"
#include "Bag.h"
#include "BagWidget.h"

class LabSceneWidget : public QWidget {
    Q_OBJECT

public:
    explicit LabSceneWidget(Bag *bag, PokemonCollection *pokemonCollection, QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void enterTown();

private:
    QPixmap background;
    Player *player;

    const int windowWidth = 525;
    const int windowHeight = 450;
    int mapWidth;
    int mapHeight;

    QVector<Barrier*> barriers;
    QLabel *posLabel;
    QRect exitZone;

    void addLabBarrier();

    Bag *bag;
    BagWidget *bagWidget = nullptr;
    PokemonCollection *pokemonCollection;

    bool canMove = true;
};
