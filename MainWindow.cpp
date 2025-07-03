#include "MainWindow.h"
#include "TitleSceneWidget.h"
#include "LabSceneWidget.h"
#include "TownSceneWidget.h"
#include "GrasslandSceneWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      titleScene(new TitleSceneWidget)
{
    setFixedSize(525, 450);
    setCentralWidget(stackedWidget);

    bag = new Bag();
    pokemonCollection = new PokemonCollection();

    labScene = new LabSceneWidget(bag, pokemonCollection);
    townScene = new TownSceneWidget(bag, pokemonCollection);
    grasslandScene = new GrasslandSceneWidget(bag, pokemonCollection);

    stackedWidget->addWidget(titleScene);
    stackedWidget->addWidget(labScene);
    stackedWidget->addWidget(townScene);
    stackedWidget->addWidget(grasslandScene);
    stackedWidget->setCurrentWidget(titleScene);


    setupConnections();

}

MainWindow::~MainWindow() {}

void MainWindow::setupConnections() {
    connect(titleScene, &TitleSceneWidget::startGame, this, &MainWindow::startGame);
    connect(labScene, &LabSceneWidget::enterTown, this, &MainWindow::enterTown);
    connect(townScene, &TownSceneWidget::enterGrassland, this, &MainWindow::enterGrassland);
    connect(townScene, &TownSceneWidget::returnToLab, this, &MainWindow::returnToLab);
    connect(grasslandScene, &GrasslandSceneWidget::returnToTown, this, &MainWindow::returnToTown);
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
