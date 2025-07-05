#include "ChoosePokemonWidget.h"
#include "Pokemon.h"
#include "DialogWidget.h"
#include <QTimer>
#include <QPixmap>

ChoosePokemonWidget::ChoosePokemonWidget(PokemonCollection *collection, QWidget *parent)
    : QWidget(parent), collection(collection) {

    setFixedSize(300, 250);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);

    QLabel *background = new QLabel(this);
    background->setPixmap(QPixmap(":/other/data/item_bag_background.png").scaled(size()));
    background->setFixedSize(size());
    background->move(0,0);

    struct PokemonData {
        QString name;
        QString imagePath;
        QPoint imagePos;
        QPoint buttonPos;
    };

    QVector<PokemonData> pokemons = {
        {"Charmander", ":/battle/data/battle/charmander.png", QPoint(30, 60), QPoint(30, 180)},
        {"Squirtle",   ":/battle/data/battle/squirtle.png",   QPoint(120, 60), QPoint(110, 180)},
        {"Bulbasaur",  ":/battle/data/battle/bulbasaur.png",  QPoint(210, 60), QPoint(190, 180)}
    };

    for (const auto &poke : pokemons) {
        QLabel *icon = new QLabel(this);
        icon->setPixmap(QPixmap(poke.imagePath).scaled(80, 80));
        icon->setFixedSize(80, 80);
        icon->move(poke.imagePos);
        icon->show();

        QPushButton *button = new QPushButton(poke.name, this);
        button->setFixedWidth(90);
        button->move(poke.buttonPos);

        connect(button, &QPushButton::clicked, this, [=]() {
            showConfirmDialog(poke.name);
        });
    }
}

void ChoosePokemonWidget::showConfirmDialog(const QString &name) {
    if (confirmDialog) {
        confirmDialog->close();
        delete confirmDialog;
    }
    confirmDialog = new QWidget(this);
    confirmDialog->setFixedSize(250, 100);
    confirmDialog->move(25, 75);
    confirmDialog->setStyleSheet("background-color: white; border: 2px solid black;");

    QLabel *label = new QLabel(QString("Do you want to choose %1 as your partner?").arg(name), confirmDialog);
    label->setWordWrap(true);
    label->setFixedSize(230, 40);
    label->move(10, 5);

    QPushButton *yesButton = new QPushButton("Yes", confirmDialog);
    QPushButton *noButton = new QPushButton("No", confirmDialog);
    yesButton->move(50, 60);
    noButton->move(130, 60);

    connect(yesButton, &QPushButton::clicked, this, [=]() {
        QString frontPath = QString(":/battle/data/battle/%1.png").arg(name.toLower());
        QString backPath = QString(":/battle/data/battle/%1_back.png").arg(name.toLower());
        Pokemon *p = Pokemon::createPokemon(name, 1, false);
        collection->addPokemon(p);

        // 改為顯示於父場景
        DialogWidget *dw = new DialogWidget(QStringList({QString("%1 is now in your bag!").arg(name)}), parentWidget(), true);
        QTimer::singleShot(1000, dw, &QWidget::close);
        emit pokemonChosen();
        close();
    });

    connect(noButton, &QPushButton::clicked, this, [=]() {
        confirmDialog->close();
        delete confirmDialog;
        confirmDialog = nullptr;
    });

    confirmDialog->show();
}
