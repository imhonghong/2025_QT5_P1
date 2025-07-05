#include "Move.h"

Move::Move(const QString &name, int power, int maxPp)
    : name(name), power(power), maxPp(maxPp), currentPp(maxPp) {}

QString Move::getName() const { return name; }
int Move::getPower() const { return power; }
int Move::getMaxPp() const { return maxPp; }
int Move::getCurrentPp() const { return currentPp; }

void Move::use() {
    if (currentPp > 0) {
        currentPp--;
    }
}

void Move::recoverPp() {
    currentPp = maxPp;
}
