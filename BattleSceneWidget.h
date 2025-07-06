#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>

#include "Pokemon.h"
#include "Bag.h"
#include "PokemonCollection.h"

class BattleSceneWidget : public QWidget {
    Q_OBJECT
public:
    explicit BattleSceneWidget(Pokemon *wildPokemon, Bag *bag, PokemonCollection *collection, QWidget *parent = nullptr);

signals:
    void battleEnded(); // 戰鬥結束返回場景

protected:
    void keyPressEvent(QKeyEvent *event) override;

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
    QLabel *messageLabel;

    QLabel *playerPokemonLabel;
    QLabel *playerNameLabel;
    QLabel *playerLevelLabel;
    QLabel *playerHpLabel;
    QProgressBar *playerHpBar;

    QLabel *wildPokemonLabel;
    QLabel *wildNameLabel;
    QLabel *wildLevelLabel;
    QProgressBar *wildHpBar;

    QWidget *actionMenu;
    QPushButton *fightButton;
    QPushButton *bagButton;
    QPushButton *pokemonButton;
    QPushButton *runButton;

    QWidget *fightMenu;
    QLabel *fightMenuBackground;
    QLabel *skillPowerLabel;
    QLabel *skillPPLabel;

    int pre_skl_idx;
    int pre_act_idx;
    int selectedSkillIndex = 0;
    int selectedActionIndex = 0;
    QVector<QPushButton*> actionButtons;
    QVector<QPushButton*> skillButtons;

    void setupUI();
    void updateInfo();
    void processEnemyTurn();

    const QString buttonStyle =
        "QPushButton { color: black; font-weight: bold; background-color: white; }"
        "QPushButton:focus { border: 2px solid blue; }";

    QLabel *bagOverlayLabel;
    QLabel *bagItemLabel;
    QPushButton *potionButton;
    QPushButton *etherButton;
    QPushButton *pokeballButton;
    QPushButton *backButton_bag;
    QPushButton *backButton_fight;
    void showBagMenu();
    void hideBagMenu();

    QWidget *etherMenu;
    QPushButton *etherSkillButtons[4];
    QPushButton *backButton_ether;
    void showEtherMenu();
    void hideEtherMenu();

    QWidget *switchPokemonMenu;
    QPushButton *switchPokemonButtons[4];
    QLabel *switchPokemonInfoLabels[4];
    QPushButton *backButton_switchPokemon;
    void showSwitchPokemonMenu();
    void hideSwitchPokemonMenu();

    // setUI helper
    void set_background();
    void set_playerPokemon();
    void set_wildPokemon();
    void set_messageLabel();
    void set_actionMenu();
    void set_fightMenu();
    void set_battleBag();
    void set_etherMenu();
};
