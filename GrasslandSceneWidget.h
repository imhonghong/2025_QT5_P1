#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>

#include "Player.h"
#include "Barrier.h"
#include "Bag.h"
#include "BagWidget.h"
#include "PokemonCollection.h"


class GrasslandSceneWidget : public QWidget {
    Q_OBJECT

public:
    explicit GrasslandSceneWidget(Bag *bag,
             PokemonCollection *pokemonCollection, QWidget *parent = nullptr);

signals:
    void returnToTown();
    void enterBattle(Pokemon* wildPokemon);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QPixmap background;
    Player *player;
    QVector<Barrier*> barriers;
    Bag *bag;
    PokemonCollection *pokemonCollection;

    BagWidget *bagWidget = nullptr;
    bool canMove = true;

    const int windowWidth = 525;
    const int windowHeight = 450;
    int mapWidth;
    int mapHeight;

    QLabel *posLabel;
    void addGrasslandBarrier();
};
