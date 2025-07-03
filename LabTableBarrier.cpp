#include "LabTableBarrier.h"
#include "DialogWidget.h"
#include "ChoosePokemonWidget.h"

LabTableBarrier::LabTableBarrier(int x, int y, int w, int h)
    : Barrier(x, y, w, h) {}

bool LabTableBarrier::isPassable(int playerX, int playerY, int direction) const {
    Q_UNUSED(direction);
    return !rect.intersects(QRect(playerX, playerY, 35, 48));
}

bool LabTableBarrier::canInteract(int playerX, int playerY) const {
    QRect playerRect(playerX, playerY, 35, 48);
    QRect interactZone = rect.adjusted(-10, -10, 10, 10);
    return interactZone.intersects(playerRect);
}

void LabTableBarrier::interactWithPlayer(QWidget *parent, PokemonCollection *pokemonCollection) {
    if (hasChosen) {
        new DialogWidget(QStringList({"You already received your Pokémon!"}), parent);
        return;
    }

    ChoosePokemonWidget *chooser = new ChoosePokemonWidget(pokemonCollection, parent);
    chooser->move(100, 50);
    chooser->show();

    QObject::connect(chooser, &ChoosePokemonWidget::pokemonChosen, [=]() {
        hasChosen = true;
    });
}
