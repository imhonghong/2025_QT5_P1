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

QMap<QString, QVector<QString>> Pokemon::skillTable = {
    {"Charmander", {"Scratch:10:20", "Growl:15:20", "Scary Face:20:15", "Flare Blitz:25:5"}},
    {"Squirtle", {"Tackle:10:20", "Tail Whip:15:20", "Protect:20:15", "Wave Crash:25:5"}},
    {"Bulbasaur", {"Tackle:10:20", "Growl:15:20", "Growth:20:15", "Razor Leaf:25:5"}}
};

QMap<QString, QStringList> Pokemon::imagePaths = {
    {"Charmander", {":/battle/data/battle/charmander.png", ":/battle/data/battle/charmander_back.png"}},
    {"Squirtle", {":/battle/data/battle/squirtle.png", ":/battle/data/battle/squirtle_back.png"}},
    {"Bulbasaur", {":/battle/data/battle/bulbasaur.png", ":/battle/data/battle/bulbasaur_back.png"}}
};

QMap<QString, QVector<int>> Pokemon::hpTable = {
    {"Charmander", {30, 80, 100}},
    {"Squirtle", {30, 80, 100}},
    {"Bulbasaur", {30, 80, 100}}
};

QMap<QString, QVector<int>> Pokemon::atkTable = {
    {"Charmander", {5, 10, 15}},
    {"Squirtle", {5, 10, 15}},
    {"Bulbasaur", {5, 10, 15}}
};

QMap<QString, QVector<int>> Pokemon::defTable = {
    {"Charmander", {5, 10, 15}},
    {"Squirtle", {5, 10, 15}},
    {"Bulbasaur", {5, 10, 15}}
};


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

Pokemon* Pokemon::createPokemon(const QString &name, int level, bool isWild) {
    if (!hpTable.contains(name)) return nullptr;

    int stage = (level <= 1) ? 0 : (level <= 3) ? 1 : 2;
    int hp = hpTable[name][stage];
    int atk = atkTable[name][stage];
    int def = defTable[name][stage];
    QString front = imagePaths[name][0];
    QString back = imagePaths[name][1];

    Pokemon *p = new Pokemon(name, level, hp, atk, def, front, back);

    // 技能依等級設定
    QVector<QString> skills = skillTable[name];
    if (level >= 1 && skills.size() >= 1) {
        QStringList parts = skills[0].split(":");
        p->addMove(new Move(parts[0], parts[1].toInt(), parts[2].toInt()));
    }
    if (level >= 2 && skills.size() >= 2) {
        QStringList parts = skills[1].split(":");
        p->addMove(new Move(parts[0], parts[1].toInt(), parts[2].toInt()));
    }
    if (level >= 3 && skills.size() >= 3) {
        QStringList parts = skills[2].split(":");
        p->addMove(new Move(parts[0], parts[1].toInt(), parts[2].toInt()));
    }
    if (level >= 4 && skills.size() >= 4) {
        QStringList parts = skills[3].split(":");
        p->addMove(new Move(parts[0], parts[1].toInt(), parts[2].toInt()));
    }

    return p;
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
