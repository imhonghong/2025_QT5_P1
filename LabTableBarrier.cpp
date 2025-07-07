#include "LabTableBarrier.h"
#include "DialogWidget.h"
#include "ChoosePokemonWidget.h"
#include "LabSceneWidget.h"

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
    auto lab = qobject_cast<LabSceneWidget*>(parent);
    if (lab) lab->canMove = false;

    if (hasChosen) {
        new DialogWidget(QStringList({"You already received your Pokémon!"}), parent);
        lab->canMove = true;
        return;
    }


    ChoosePokemonWidget *chooser = new ChoosePokemonWidget(pokemonCollection, parent);
    chooser->move(100, 50);
    chooser->show();
    chooser->setFocusPolicy(Qt::StrongFocus);
    chooser->setFocus();
    chooser->grabKeyboard();

    QObject::connect(chooser, &ChoosePokemonWidget::pokemonChosen, [=]() {
        this->hasChosen = true;
        if (lab) {
            lab->canMove = true;
            lab->setFocus(); // 🩹 主動恢復 LabSceneWidget 的 focus
        }
        chooser->releaseKeyboard();
        chooser->close();
    });
    QObject::connect(chooser, &QWidget::destroyed, [=]() {
        if (lab) {
            lab->canMove = true;
            lab->setFocus(); // 🩹 確保關閉時恢復 focus
        }
    });
}
