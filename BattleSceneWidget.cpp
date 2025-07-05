#include "BattleSceneWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>

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

    // 背景
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/battle/data/battle/battle_scene.png").scaled(size()));
    backgroundLabel->setGeometry(0, 0, 525, 450);

    // 玩家寶可夢圖
    playerPokemonLabel = new QLabel(this);
    playerPokemonLabel->setPixmap(QPixmap(playerPokemon->getImagePath(false)).scaled(120, 120));
    playerPokemonLabel->move(50, 180);

    // 野生寶可夢圖
    wildPokemonLabel = new QLabel(this);
    wildPokemonLabel->setPixmap(QPixmap(wildPokemon->getImagePath(true)).scaled(120, 120));
    wildPokemonLabel->move(340, 60);

    // 訊息顯示
    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: white; font-weight: bold;");
    messageLabel->setGeometry(36, 350, 400, 60);

    // 玩家寶可夢資訊
    playerNameLabel = new QLabel(playerPokemon->getName(), this);
    playerNameLabel->setStyleSheet("color: black; font-weight: bold;");
    playerNameLabel->move(320, 230);

    playerLevelLabel = new QLabel(QString("Lv:%1").arg(playerPokemon->getLevel()), this);
    playerLevelLabel->setStyleSheet("color: black; font-weight: bold;");
    playerLevelLabel->move(430, 230);

    playerHpLabel = new QLabel(QString("HP: %1/%2").arg(playerPokemon->getHp()).arg(playerPokemon->getMaxHp()), this);
    playerHpLabel->setStyleSheet("color: black; font-weight: bold;");
    playerHpLabel->move(400, 276);

    playerHpBar = new QProgressBar(this);
    playerHpBar->setGeometry(382, 259, 108, 10);
    playerHpBar->setRange(0, playerPokemon->getMaxHp());
    playerHpBar->setValue(playerPokemon->getHp());
    playerHpBar->setTextVisible(false);
    playerHpBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");

    // 野生寶可夢資訊
    wildNameLabel = new QLabel(wildPokemon->getName(), this);
    wildNameLabel->setStyleSheet("color: black; font-weight: bold;");
    wildNameLabel->move(50, 60);

    wildLevelLabel = new QLabel(QString("Lv:%1").arg(wildPokemon->getLevel()), this);
    wildLevelLabel->setStyleSheet("color: black; font-weight: bold;");
    wildLevelLabel->move(160, 60);

    wildHpBar = new QProgressBar(this);
    wildHpBar->setGeometry(112, 92, 108, 10);
    wildHpBar->setRange(0, wildPokemon->getMaxHp());
    wildHpBar->setValue(wildPokemon->getHp());
    wildHpBar->setTextVisible(false);
    wildHpBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");

    // Action Menu 四宮格按鈕
    actionMenu = new QWidget(this);
    actionMenu->setGeometry(281, 337, 228, 92);
    actionMenu->setStyleSheet("background: transparent;");
    QGridLayout *actionLayout = new QGridLayout(actionMenu);
    actionLayout->setSpacing(5);
    actionLayout->setContentsMargins(0,0,0,0);

    fightButton = new QPushButton("Fight", actionMenu);
    bagButton = new QPushButton("Bag", actionMenu);
    pokemonButton = new QPushButton("Pokémon", actionMenu);
    runButton = new QPushButton("Run", actionMenu);

    fightButton->setFocusPolicy(Qt::StrongFocus);
    bagButton->setFocusPolicy(Qt::StrongFocus);
    pokemonButton->setFocusPolicy(Qt::StrongFocus);
    runButton->setFocusPolicy(Qt::StrongFocus);

    fightButton->setStyleSheet(buttonStyle);
    bagButton->setStyleSheet(buttonStyle);
    pokemonButton->setStyleSheet(buttonStyle);
    runButton->setStyleSheet(buttonStyle);

    actionLayout->addWidget(fightButton, 0, 0);
    actionLayout->addWidget(bagButton, 0, 1);
    actionLayout->addWidget(pokemonButton, 1, 0);
    actionLayout->addWidget(runButton, 1, 1);

    connect(fightButton, &QPushButton::clicked, this, &BattleSceneWidget::onFightClicked);
    connect(bagButton, &QPushButton::clicked, this, &BattleSceneWidget::onBagClicked);
    connect(pokemonButton, &QPushButton::clicked, this, &BattleSceneWidget::onPokemonClicked);
    connect(runButton, &QPushButton::clicked, this, &BattleSceneWidget::onRunClicked);

    actionButtons.append(fightButton);
    actionButtons.append(bagButton);
    actionButtons.append(pokemonButton);
    actionButtons.append(runButton);
    actionButtons[selectedActionIndex]->setFocus();

    // Fight Menu 區域（背景 + GridLayout + 獨立 Label）
    fightMenu = new QWidget(this);
    fightMenu->setGeometry(0, 317, 525, 133);
    fightMenu->hide();

    fightMenuBackground = new QLabel(fightMenu);
    fightMenuBackground->setPixmap(QPixmap(":/battle/data/battle/fight_skill.png").scaled(525, 133));
    fightMenuBackground->setGeometry(0, 0, 525, 133);
    fightMenuBackground->lower();

    QGridLayout *gridLayout = new QGridLayout(fightMenu);
    gridLayout->setContentsMargins(15, 16, 190, 16);
    gridLayout->setSpacing(5);

    QVector<Move*> moves = playerPokemon->getMoves();
    for (int i = 0; i < 4; ++i) {
        QPushButton *skillButton;
        if (i < moves.size()) {
            Move *move = moves[i];
            skillButton = new QPushButton(move->getName(), fightMenu);
            connect(skillButton, &QPushButton::clicked, this, &BattleSceneWidget::onSkillClicked);
        } else {
            skillButton = new QPushButton("No Skill", fightMenu);
            skillButton->setEnabled(false); // 禁用按鈕，無法攻擊
        }
        skillButton->setFocusPolicy(Qt::StrongFocus);
        skillButton->setStyleSheet(buttonStyle);
        gridLayout->addWidget(skillButton, i / 2, i % 2);
        skillButtons.append(skillButton);
    }

    skillPPLabel = new QLabel("--/--", fightMenu);
    skillPPLabel->setStyleSheet("color: black; font-weight: bold; font-size: 20px");
    skillPPLabel->move(440, 35);

    skillPowerLabel = new QLabel("--", fightMenu);
    skillPowerLabel->setStyleSheet("color: black; font-weight: bold; font-size: 20px");
    skillPowerLabel->move(440, 80);


}


void BattleSceneWidget::updateInfo() {
    playerNameLabel->setText(playerPokemon->getName());
    playerLevelLabel->setText(QString("Lv:%1").arg(playerPokemon->getLevel()));
    playerHpLabel->setText(QString("HP: %1/%2").arg(playerPokemon->getHp()).arg(playerPokemon->getMaxHp()));
    playerHpBar->setMaximum(playerPokemon->getMaxHp());
    playerHpBar->setValue(playerPokemon->getHp());

    wildNameLabel->setText(wildPokemon->getName());
    wildLevelLabel->setText(QString("Lv:%1").arg(wildPokemon->getLevel()));
    wildHpBar->setMaximum(wildPokemon->getMaxHp());
    wildHpBar->setValue(wildPokemon->getHp());
}

void BattleSceneWidget::onFightClicked() {
    fightMenu->show();
    selectedSkillIndex = 0;
    skillButtons[selectedSkillIndex]->setFocus();

    Move *move = playerPokemon->getMoves()[selectedSkillIndex];
    skillPowerLabel->setText(QString("%1").arg(move->getPower()));
    skillPPLabel->setText(QString("%1/%2").arg(move->getCurrentPp()).arg(move->getMaxPp()));
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

void BattleSceneWidget::keyPressEvent(QKeyEvent *event) {
    if (fightMenu->isVisible()) { // 處理技能選擇上下左右

        int row = selectedSkillIndex / 2;
        int col = selectedSkillIndex % 2;

        if (event->key() == Qt::Key_Right) {
            if (col < 1) selectedSkillIndex += 1;
        } else if (event->key() == Qt::Key_Left) {
            if (col > 0) selectedSkillIndex -=1 ;
        } else if (event->key() == Qt::Key_Up) {
            if (row > 0) selectedSkillIndex -= 2;
        } else if (event->key() == Qt::Key_Down) {
            if (row < 1) selectedSkillIndex +=2;
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space) {
            skillButtons[selectedSkillIndex]->click();
            return;
        } else {
            QWidget::keyPressEvent(event);
            return;
        }

        skillButtons[selectedSkillIndex]->setFocus();

        // 更新 skillPowerLabel 與 skillPPLabel
        QVector<Move*> moves = playerPokemon->getMoves();
        if (selectedSkillIndex < moves.size()) {
            Move *move = moves[selectedSkillIndex];
            if (skillPowerLabel && skillPPLabel) {
                skillPowerLabel->setText(QString("Power: %1").arg(move->getPower()));
                skillPPLabel->setText(QString("PP: %1/%2").arg(move->getCurrentPp()).arg(move->getMaxPp()));
            }
        }

    }  else { // Action Menu

        int row = selectedActionIndex / 2;
        int col = selectedActionIndex % 2;

        if (event->key() == Qt::Key_Right) {
            if (selectedActionIndex % 2 == 0) selectedActionIndex += 1;
        } else if (event->key() == Qt::Key_Left) {
            if (selectedActionIndex % 2 == 1) selectedActionIndex -= 1;
        } else if (event->key() == Qt::Key_Up) {
            if (selectedActionIndex >= 2) selectedActionIndex -= 2;
        } else if (event->key() == Qt::Key_Down) {
            if (selectedActionIndex <= 1) selectedActionIndex += 2;
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space) {
            // 確保 Enter 時取當前 Focus 按鈕
            for (int i = 0; i < actionButtons.size(); ++i) {
                if (actionButtons[i]->hasFocus()) {
                    selectedActionIndex = i;
                    break;
                }
            }

            qDebug() << "[Action] Enter Pressed. Index:" << selectedActionIndex
                     << "Button:" << actionButtons[selectedActionIndex]->text();
            actionButtons[selectedActionIndex]->click();
            return;
        } else {
            QWidget::keyPressEvent(event);
            return;
        }

        if (selectedActionIndex >= actionButtons.size()) {
            selectedActionIndex = pre_act_idx;
        }

        qDebug() << "[Action] Focus Index:" << selectedActionIndex
                 << "Button:" << actionButtons[selectedActionIndex]->text();

        actionButtons[selectedActionIndex]->setFocus();
    }
}
