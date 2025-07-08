#include "MainWindow.h"
#include "TitleSceneWidget.h"
#include "LabSceneWidget.h"
#include "TownSceneWidget.h"
#include "GrasslandSceneWidget.h"
#include "BattleSceneWidget.h"
#include "GameOverScene.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this))
{
    setFixedSize(525, 450);
    setCentralWidget(stackedWidget);

    bag = new Bag();
    pokemonCollection = new PokemonCollection();
    titleScene = new TitleSceneWidget;
    labScene = new LabSceneWidget(bag, pokemonCollection);
    townScene = new TownSceneWidget(bag, pokemonCollection);
    grasslandScene = new GrasslandSceneWidget(bag, pokemonCollection);
    gameOverScene = new GameOverScene();


    stackedWidget->addWidget(titleScene);
    stackedWidget->addWidget(labScene);
    stackedWidget->addWidget(townScene);
    stackedWidget->addWidget(grasslandScene);
    stackedWidget->addWidget(gameOverScene);

    stackedWidget->setCurrentWidget(titleScene);
    QTimer::singleShot(0, this, [this]() {
        titleScene->setFocus(Qt::OtherFocusReason);
        titleScene->activateWindow();
        titleScene->raise();
        qDebug() << "TitleScene has focus (delayed):" << titleScene->hasFocus();
    });
    setupConnections();

}

MainWindow::~MainWindow() {}

void MainWindow::setupConnections() {
    connect(titleScene, &TitleSceneWidget::startGame, this, &MainWindow::startGame);
    connect(labScene, &LabSceneWidget::enterTown, this, &MainWindow::enterTown);
    connect(townScene, &TownSceneWidget::enterGrassland, this, &MainWindow::enterGrassland);
    connect(townScene, &TownSceneWidget::returnToLab, this, &MainWindow::returnToLab);
    connect(grasslandScene, &GrasslandSceneWidget::returnToTown, this, &MainWindow::returnToTown);
    connect(grasslandScene, &GrasslandSceneWidget::enterBattle, this, &MainWindow::startBattle);
}

void MainWindow::startGame() {
    stackedWidget->setCurrentWidget(labScene);
}

void MainWindow::enterTown() {
    stackedWidget->setCurrentWidget(townScene);
}

void MainWindow::enterGrassland() {
    stackedWidget->setCurrentWidget(grasslandScene);
}

void MainWindow::returnToLab() {
    stackedWidget->setCurrentWidget(labScene);
}

void MainWindow::returnToTown() {
    stackedWidget->setCurrentWidget(townScene);
}

void MainWindow::startBattle(Pokemon* wildPokemon) {
    BattleSceneWidget *battleScene = new BattleSceneWidget(wildPokemon, bag, pokemonCollection, this);

    stackedWidget->addWidget(battleScene);
    stackedWidget->setCurrentWidget(battleScene);

    connect(battleScene, &BattleSceneWidget::battleEnded, this, [=]() {
        stackedWidget->removeWidget(battleScene);
        battleScene->deleteLater();

        // 判斷是否全滅
        bool allDead = true;
        for (Pokemon* p : pokemonCollection->getAllPokemons()) {
            if (p->getHp() > 0) {
                allDead = false;
                break;
            }
        }
        if (allDead) {
            showGameOver();
        } else {
            stackedWidget->setCurrentWidget(grasslandScene);
        }
    });
}

void MainWindow::showGameOver() {
    stackedWidget->setCurrentWidget(gameOverScene);
    QTimer::singleShot(0, gameOverScene, [this]() { gameOverScene->setFocus(); });
}
