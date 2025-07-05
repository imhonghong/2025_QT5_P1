#include "Pokemon.h"
#include <QDebug>

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

QString Pokemon::getImagePath(bool useFront) const {
    if (useFront)
        return frontImagePath;
    else
        return backImagePath;
}

void Pokemon::levelUp() {
    level++;
    // 每兩級進化（簡易顯示可先跳過）
    attack += 5;
    defense += 5;
    maxHp += 10;
    hp = maxHp;
    qDebug() << name << "leveled up to level" << level;
}

bool Pokemon::isFainted() const {
    return hp <= 0;
}

void Pokemon::restoreAllPp() {
    for (Move* move : moves) {
        move->restore();
    }
    qDebug() << name << "'s PP has been fully restored.";
}
