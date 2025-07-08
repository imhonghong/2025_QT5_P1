#pragma once

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVector>
#include <QFrame>
#include "Pokemon.h"
#include "Bag.h"
#include "PokemonCollection.h"

class MainWindow;
class BattleSceneWidget : public QWidget {
    Q_OBJECT
public:
    explicit BattleSceneWidget(Pokemon *wildPokemon, Bag *bag, PokemonCollection *collection, MainWindow *mainWindow, QWidget *parent = nullptr);

signals:
    void battleEnded();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onFightClicked();
    void onBagClicked();
    void onPokemonClicked();
    void onRunClicked();

private:
    Pokemon *wildPokemon;
    Pokemon *playerPokemon;
    Bag *bag;
    PokemonCollection *collection;
    MainWindow *mainWindow;

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
    QWidget *fightMenu;
    QWidget *bagMenu;
    QWidget *etherMenu;
    QWidget *switchPokemonMenu;

    QLabel *fightMenuBackground;
    QLabel *skillPowerLabel;
    QLabel *skillPPLabel;
    QLabel *bagItemLabel;
    QLabel *bagOverlayLabel;

    int selectedSkillIndex = 0;
    int selectedActionIndex = 0;
    int selectedBagIndex = 0;
    int selectedEtherIndex = 0;
    int selectedSwitchIndex = 0;

    QVector<QFrame*> actionFrames;
    QVector<QFrame*> skillFrames;
    QVector<QFrame*> bagFrames;
    QVector<QFrame*> etherFrames;
    QVector<QFrame*> switchFrames;

    const QString FRAME_BORDER_NORMAL = "border: 2px solid white;";
    const QString FRAME_BORDER_SELECTED = "border: 2px solid blue;";

    void setupUI();
    void set_background();
    void set_playerPokemon();
    void set_wildPokemon();
    void set_messageLabel();
    void set_actionMenu();
    void set_fightMenu();
    void set_battleBag();
    void set_etherMenu();
    void set_switchPokemonMenu();
    void set_floatingHintLabel();

    void updateInfo();

    void updateActionFrameFocus();
    void updateSkillFrameFocus();
    void updateBagFrameFocus();
    void updateEtherFrameFocus();
    void updateSwitchFrameFocus();

    void executeAction(int idx);
    void useSkill(int idx);
    void executeBagAction(int idx);
    void executeEtherAction(int idx);
    void executeSwitchPokemonAction(int idx);

    void updateSkillInfoDisplay(int idx);
    void updateEtherMenu();
    void updateSwitchPokemonMenu();

    void handleEscKey();
    void handleMenuKeyPress(QKeyEvent *event,
                            QVector<QFrame*> &frames,
                            int &selectedIndex,
                            std::function<void(int)> onEnter,
                            std::function<void(int)> onUpdate,
                            int columns);

    void processEnemyTurn();
    void animateHpBar(QProgressBar *bar, int startValue, int endValue);

    QLabel *floatingHintLabel;
    void showFloatingHint(const QString &text);
};
