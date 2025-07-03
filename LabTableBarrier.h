#pragma once
#include "Barrier.h"
#include "PokemonCollection.h"

#include <QWidget>

class LabTableBarrier : public Barrier {
public:
    LabTableBarrier(int x, int y, int w, int h);

    bool isPassable(int playerX, int playerY, int direction) const override;
    bool canInteract(int playerX, int playerY) const override;
    void interactWithPlayer(QWidget *parent, PokemonCollection *pokemonCollection);

private:
    bool hasChosen = false;
};
