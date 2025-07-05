#pragma once
#include <QString>

class Move {
public:
    Move(const QString &name, int power, int maxPp);

    QString getName() const;
    int getPower() const;
    int getMaxPp() const;
    int getCurrentPp() const;

    void use();
    void recoverPp();

private:
    QString name;
    int power;
    int maxPp;
    int currentPp;
};
