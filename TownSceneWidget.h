#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QVector>

#include "Barrier.h"
#include "SolidBarrier.h"
#include "NPCBarrier.h"
#include "Player.h"
#include "Bag.h"
#include "BagWidget.h"
#include "PokemonCollection.h"

class TownSceneWidget : public QWidget {
    Q_OBJECT

public:
    explicit TownSceneWidget(Bag *bag, PokemonCollection *pokemonCollection, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void enterGrassland();
    void returnToLab();

private:
    QPixmap background;
    Player *player;
    QVector<Barrier*> barriers;
    Bag* bag;
    PokemonCollection *pokemonCollection;

    BagWidget *bagWidget = nullptr;
    bool canMove = true;

    const int windowWidth = 525;
    const int windowHeight = 450;
    int mapWidth;
    int mapHeight;

    QLabel *posLabel;

    void addTownBarrier();
    void generateBoxes();
};
