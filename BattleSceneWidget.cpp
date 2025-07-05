#include "BattleSceneWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>

BattleSceneWidget::BattleSceneWidget(Pokemon *wildPokemon, Bag *bag, PokemonCollection *collection, QWidget *parent)
    : QWidget(parent), wildPokemon(wildPokemon), bag(bag), collection(collection) {

    // 以第一隻 Pokemon 為出戰
    if (!collection->getAllPokemons().isEmpty()) {
        playerPokemon = collection->getAllPokemons().first();
    } else {
        qDebug() << "No player Pokémon found!";
        return;
    }

    setupUI();
    updateInfo();
}

void BattleSceneWidget::setupUI() {
    setFixedSize(525, 450);

    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/battle/data/battle/battle_scene.png").scaled(size()));
    backgroundLabel->setGeometry(0, 0, 525, 450);

    playerPokemonLabel = new QLabel(this);
    playerPokemonLabel->setPixmap(QPixmap(playerPokemon->getImagePath(false)).scaled(120, 120));
    playerPokemonLabel->move(50, 180);

    wildPokemonLabel = new QLabel(this);
    wildPokemonLabel->setPixmap(QPixmap(wildPokemon->getImagePath(true)).scaled(120, 120));
    wildPokemonLabel->move(340, 60);

    playerInfoLabel = new QLabel(this);
    playerInfoLabel->setStyleSheet("color: white; font-weight: bold;");
    playerInfoLabel->setGeometry(20, 370, 250, 60);

    wildInfoLabel = new QLabel(this);
    wildInfoLabel->setStyleSheet("color: black; font-weight: bold;");
    wildInfoLabel->setGeometry(50, 30, 250, 60);

    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: red; font-weight: bold;");
    messageLabel->setGeometry(100, 200, 350, 30);

    fightButton = new QPushButton("Fight", this);
    fightButton->setGeometry(280, 345, 100, 30);
    connect(fightButton, &QPushButton::clicked, this, &BattleSceneWidget::onFightClicked);

    bagButton = new QPushButton("Bag", this);
    bagButton->setGeometry(400, 345, 100, 30);
    connect(bagButton, &QPushButton::clicked, this, &BattleSceneWidget::onBagClicked);

    pokemonButton = new QPushButton("Pokémon", this);
    pokemonButton->setGeometry(280, 395, 100, 30);
    connect(pokemonButton, &QPushButton::clicked, this, &BattleSceneWidget::onPokemonClicked);

    runButton = new QPushButton("Run", this);
    runButton->setGeometry(400, 395, 100, 30);
    connect(runButton, &QPushButton::clicked, this, &BattleSceneWidget::onRunClicked);

    // Fight Skill Menu
    fightMenu = new QWidget(this);
    fightMenu->setGeometry(50, 250, 300, 180);
    fightMenu->setStyleSheet("background-image: url(:/battle/data/battle/fight_skill.png);");
    fightMenu->hide();

    QVBoxLayout *layout = new QVBoxLayout(fightMenu);
    QVector<Move*> moves = playerPokemon->getMoves();
    for (Move *move : moves) {
        QPushButton *skillButton = new QPushButton(
            QString("%1 (Pwr:%2 PP:%3/%4)").arg(move->getName()).arg(move->getPower()).arg(move->getCurrentPp()).arg(move->getMaxPp()),
            fightMenu);
        connect(skillButton, &QPushButton::clicked, this, &BattleSceneWidget::onSkillClicked);
        layout->addWidget(skillButton);
        skillButtons.append(skillButton);
    }
}

void BattleSceneWidget::updateInfo() {
    playerInfoLabel->setText(
        QString("%1 Lv:%2\nHP: %3/%4")
        .arg(playerPokemon->getName())
        .arg(playerPokemon->getLevel())
        .arg(playerPokemon->getHp())
        .arg(playerPokemon->getMaxHp())
    );

    wildInfoLabel->setText(
        QString("%1 Lv:%2\nHP: %3/%4")
        .arg(wildPokemon->getName())
        .arg(wildPokemon->getLevel())
        .arg(wildPokemon->getHp())
        .arg(wildPokemon->getMaxHp())
    );
}

void BattleSceneWidget::onFightClicked() {
    fightMenu->show();
}

void BattleSceneWidget::onSkillClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int idx = skillButtons.indexOf(btn);
    if (idx < 0) return;

    QVector<Move*> moves = playerPokemon->getMoves();
    if (idx >= moves.size()) return;

    Move *move = moves[idx];
    if (move->getCurrentPp() <= 0) {
        messageLabel->setText("No PP left for this move!");
        return;
    }

    // 計算傷害
    int damage = (move->getPower() + playerPokemon->getAttack() - wildPokemon->getDefense()) * playerPokemon->getLevel();
    if (damage < 1) damage = 1;

    wildPokemon->receiveDamage(damage);
    move->use();

    qDebug() << playerPokemon->getName() << "used" << move->getName() << "and dealt" << damage << "damage.";

    updateInfo();
    messageLabel->setText(QString("%1 used %2!").arg(playerPokemon->getName()).arg(move->getName()));

    fightMenu->hide();

    if (wildPokemon->isFainted()) {
        messageLabel->setText(QString("%1 fainted!").arg(wildPokemon->getName()));
        emit battleEnded();
    }
}

void BattleSceneWidget::onBagClicked() {
    messageLabel->setText("Bag is not yet implemented here.");
}

void BattleSceneWidget::onPokemonClicked() {
    messageLabel->setText("Switch Pokémon not implemented yet.");
}

void BattleSceneWidget::onRunClicked() {
    messageLabel->setText("You ran away!");
    emit battleEnded();
}
