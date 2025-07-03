#pragma once

#include <QString>
#include <QVector>
#include "Move.h"

class Pokemon {
public:
    Pokemon(const QString &name,
            int level,
            int maxHp,
            int attack,
            int defense,
            const QString &frontImagePath,
            const QString &backImagePath);

    QString getName() const;
    int getLevel() const;
    int getHp() const;
    int getMaxHp() const;
    int getAttack() const;
    int getDefense() const;

    void receiveDamage(int damage);
    void heal(int amount);

    void addMove(Move *move);
    QVector<Move*> getMoves() const;

    QString getImagePath(bool useBack) const;

private:
    QString name;
    int level;
    int hp;
    int maxHp;
    int attack;
    int defense;
    bool useFront;
    QVector<Move*> moves;

    QString frontImagePath;
    QString backImagePath;
};
