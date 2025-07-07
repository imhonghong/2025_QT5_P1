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
    {"Charmeleon", {":/battle/data/battle/charmeleon.png", ":/battle/data/battle/charmeleon_back.png"}},
    {"Charizard", {":/battle/data/battle/charizard.png", ":/battle/data/battle/charizard_back.png"}},
    {"Squirtle", {":/battle/data/battle/squirtle.png", ":/battle/data/battle/squirtle_back.png"}},
    {"Wartortle", {":/battle/data/battle/wartortle.png", ":/battle/data/battle/wartortle_back.png"}},
    {"Blastoise", {":/battle/data/battle/blastoise.png", ":/battle/data/battle/blastoise_back.png"}},
    {"Bulbasaur", {":/battle/data/battle/bulbasaur.png", ":/battle/data/battle/bulbasaur_back.png"}},
    {"Ivysaur", {":/battle/data/battle/ivysaur.png", ":/battle/data/battle/ivysaur_back.png"}},
    {"Venusaur", {":/battle/data/battle/venusaur.png", ":/battle/data/battle/venusaur_back.png"}}
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

    qDebug() << "[createPokemon] raw name:" << name;
    QString key = name.trimmed();
    qDebug() << "[createPokemon] trimmed key:" << key;
    if (!hpTable.contains(key)) {
        qDebug() << "createPokemon failed, key not found in hpTable:" << key;
        return nullptr;
    }

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

    // 更新 Atk, Def, MaxHP, HP
    if (atkTable.contains(name)) {
        int stage = (level <= 1) ? 0 : (level <= 3) ? 1 : 2;
        attack = atkTable[name][stage];
        defense = defTable[name][stage];
        maxHp = hpTable[name][stage];
        hp = maxHp;
    }

    // 學會新技能
    // 先加入 nameKey 對應進化前名稱
    QString nameKey = name;
    if (name == "Charmeleon" || name == "Charizard")
        nameKey = "Charmander";
    else if (name == "Wartortle" || name == "Blastoise")
        nameKey = "Squirtle";
    else if (name == "Ivysaur" || name == "Venusaur")
        nameKey = "Bulbasaur";

    // 使用 nameKey 查表
    if (skillTable.contains(nameKey)) {
        QVector<QString> skills = skillTable[nameKey];
        if (level >= 1 && skills.size() >= 1 && moves.size() < 1) {
            QStringList parts = skills[0].split(":");
            addMove(new Move(parts[0], parts[1].toInt(), parts[2].toInt()));
            qDebug() << name << "learned" << parts[0] << "at level" << level;
        }
        if (level >= 2 && skills.size() >= 2 && moves.size() < 2) {
            QStringList parts = skills[1].split(":");
            addMove(new Move(parts[0], parts[1].toInt(), parts[2].toInt()));
            qDebug() << name << "learned" << parts[0] << "at level" << level;
        }
        if (level >= 3 && skills.size() >= 3 && moves.size() < 3) {
            QStringList parts = skills[2].split(":");
            addMove(new Move(parts[0], parts[1].toInt(), parts[2].toInt()));
            qDebug() << name << "learned" << parts[0] << "at level" << level;
        }
        if (level >= 4 && skills.size() >= 4 && moves.size() < 4) {
            QStringList parts = skills[3].split(":");
            addMove(new Move(parts[0], parts[1].toInt(), parts[2].toInt()));
            qDebug() << name << "learned" << parts[0] << "at level" << level;
        }
    }

    // 進化名稱與圖片更新
    if (name == "Charmander" && level >= 3 && level < 5) {
        name = "Charmeleon";
    } else if (name == "Charmeleon" && level >= 5) {
        name = "Charizard";
    } else if (name == "Squirtle" && level >= 3 && level < 5) {
        name = "Wartortle";
    } else if (name == "Wartortle" && level >= 5) {
        name = "Blastoise";
    } else if (name == "Bulbasaur" && level >= 3 && level < 5) {
        name = "Ivysaur";
    } else if (name == "Ivysaur" && level >= 5) {
        name = "Venusaur";
    }

    // 更新圖片
    if (imagePaths.contains(name)) {
        frontImagePath = imagePaths[name][0];
        backImagePath = imagePaths[name][1];
    }

    qDebug() << name << "leveled up to level" << level;
}


bool Pokemon::isFainted() const {
    return hp <= 0;
}

void Pokemon::restoreAllPp() {
    for (Move* move : moves) {
        move->recoverPp();
    }
    qDebug() << name << "'s PP has been fully restored.";
}
