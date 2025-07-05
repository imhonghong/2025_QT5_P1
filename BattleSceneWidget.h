#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "Pokemon.h"
#include "Bag.h"
#include "PokemonCollection.h"

class BattleSceneWidget : public QWidget {
    Q_OBJECT
public:
    explicit BattleSceneWidget(Pokemon *wildPokemon, Bag *bag, PokemonCollection *collection, QWidget *parent = nullptr);

signals:
    void battleEnded(); // 戰鬥結束返回場景

private slots:
    void onFightClicked();
    void onSkillClicked();
    void onBagClicked();
    void onPokemonClicked();
    void onRunClicked();

private:
    Pokemon *wildPokemon;
    Pokemon *playerPokemon;
    Bag *bag;
    PokemonCollection *collection;

    QLabel *backgroundLabel;
    QLabel *playerPokemonLabel;
    QLabel *wildPokemonLabel;
    QLabel *playerInfoLabel;
    QLabel *wildInfoLabel;
    QLabel *messageLabel;

    QPushButton *fightButton;
    QPushButton *bagButton;
    QPushButton *pokemonButton;
    QPushButton *runButton;

    QWidget *fightMenu;
    QVector<QPushButton*> skillButtons;

    void setupUI();
    void updateInfo();
};
