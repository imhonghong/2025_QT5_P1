#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "PokemonCollection.h"

class ChoosePokemonWidget : public QWidget {
    Q_OBJECT
public:
    ChoosePokemonWidget(PokemonCollection *collection, QWidget *parent = nullptr);

signals:
    void pokemonChosen();

private:
    PokemonCollection *collection;
    QWidget *confirmDialog = nullptr;

    void showConfirmDialog(const QString &name);
};
