#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "Bag.h"
#include "PokemonCollection.h"

class TitleSceneWidget;
class LabSceneWidget;
class TownSceneWidget;
class GrasslandSceneWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stackedWidget;
    TitleSceneWidget *titleScene;
    LabSceneWidget *labScene;
    TownSceneWidget *townScene;
    GrasslandSceneWidget *grasslandScene;
    PokemonCollection *pokemonCollection;

    void setupConnections();

    Bag *bag;

private slots:
    void startGame();   // 切換到 LabScene
    void enterTown();
    void enterGrassland();
    void returnToLab();
    void returnToTown();
    void startBattle(Pokemon* wildPokemon);
};
