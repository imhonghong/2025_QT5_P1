#include "Pokemon.h"

Pokemon::Pokemon(const QString &name,
                 int level,
                 int maxHp,
                 int attack,
                 int defense,
                 const QString &frontImagePath,
                 const QString &backImagePath)
    : name(name),
      level(level),
      hp(maxHp),
      maxHp(maxHp),
      attack(attack),
      defense(defense),
      frontImagePath(frontImagePath),
      backImagePath(backImagePath) {}

QString Pokemon::getName() const { return name; }
int Pokemon::getLevel() const { return level; }
int Pokemon::getHp() const { return hp; }
int Pokemon::getMaxHp() const { return maxHp; }
int Pokemon::getAttack() const { return attack; }
int Pokemon::getDefense() const { return defense; }

void Pokemon::receiveDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Pokemon::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}

void Pokemon::addMove(Move *move) {
    moves.append(move);
}

QVector<Move*> Pokemon::getMoves() const {
    return moves;
}

QString Pokemon::getImagePath(bool useBack) const {
    if (useBack)
        return backImagePath;
    else
        return frontImagePath;
}
