#pragma once

#include <QWidget>
#include <QLabel>
#include <QVector>
#include "Bag.h"
#include "PokemonCollection.h"

class BagWidget : public QWidget {
    Q_OBJECT

public:
    BagWidget(Bag *bag, PokemonCollection *pokemonCollection, QWidget *parent = nullptr);

private:
    Bag *bag;
    PokemonCollection *pokemonCollection;

    QLabel *backgroundItem;
    QLabel *backgroundPokemon;
    QVector<QLabel*> itemIcons;
    QVector<QLabel*> itemCounts;
    QVector<QLabel*> pokemonLabels;
    QVector<QLabel*> pokemonIcons;

    void setupDisplay();
};
