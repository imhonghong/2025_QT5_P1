#include "MainWindow.h"
#include "TitleSceneWidget.h"
#include "LabSceneWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      titleScene(new TitleSceneWidget),
      labScene(new LabSceneWidget)
{
    setFixedSize(525, 450);
    setCentralWidget(stackedWidget);

    stackedWidget->addWidget(titleScene);
    stackedWidget->addWidget(labScene);
    stackedWidget->setCurrentWidget(titleScene);

    setupConnections();
}

MainWindow::~MainWindow() {}

void MainWindow::setupConnections() {
    connect(titleScene, &TitleSceneWidget::startGame, this, &MainWindow::startGame);
}

void MainWindow::startGame() {
    stackedWidget->setCurrentWidget(labScene);
}
