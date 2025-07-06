#include "BattleSceneWidget.h"
#include "DialogWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>

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

    set_background();       // 背景
    set_playerPokemon();    // 玩家寶可夢圖和資訊
    set_wildPokemon();      // 野生寶可夢圖和資訊
    set_messageLabel();     // 訊息顯示
    set_actionMenu();       // Action Menu 四宮格按鈕
    set_fightMenu();        // Fight Menu 區域（背景 + GridLayout + 獨立 Label）
    set_battleBag();        // Bag 顯示 Label
    set_etherMenu();        // Ether Menu

    switchPokemonMenu = new QWidget(this);
    switchPokemonMenu->setGeometry(0, 317, 525, 133);
    switchPokemonMenu->setStyleSheet("background-color: white;");
    switchPokemonMenu->hide();

    for (int i = 0; i < 4; ++i) {
        switchPokemonButtons[i] = new QPushButton(switchPokemonMenu);
        switchPokemonButtons[i]->setGeometry(20 + i * 125, 30, 120, 50);
        switchPokemonButtons[i]->setStyleSheet(buttonStyle);
        switchPokemonButtons[i]->hide();

        switchPokemonInfoLabels[i] = new QLabel(switchPokemonMenu);
        switchPokemonInfoLabels[i]->setGeometry(20 + i * 125, 85, 120, 20);
        switchPokemonInfoLabels[i]->setStyleSheet("color: black; font-weight: bold; text-align: center;");
        switchPokemonInfoLabels[i]->setAlignment(Qt::AlignCenter);
        switchPokemonInfoLabels[i]->hide();


        connect(switchPokemonButtons[i], &QPushButton::clicked, this, [this, i]() {
            QVector<Pokemon*> pokemons = collection->getAllPokemons();
            if (i < pokemons.size() && pokemons[i]->getHp() > 0) {
                playerPokemon = pokemons[i];
                updateInfo();
                messageLabel->setText("Go! " + playerPokemon->getName() + "!");
                playerPokemonLabel->setPixmap(QPixmap(playerPokemon->getImagePath(false)).scaled(120, 120));
                hideSwitchPokemonMenu();

                // 進入敵方回合
                QTimer::singleShot(500, this, [this]() {
                    // processEnemyTurn();
                });
            }
        });
    }

    // Back Button
    backButton_switchPokemon = new QPushButton("Back", this);
    backButton_switchPokemon->setGeometry(450, 420, 60, 30);
    backButton_switchPokemon->setStyleSheet(buttonStyle);
    backButton_switchPokemon->hide();

    connect(backButton_switchPokemon, &QPushButton::clicked, this, [this]() {
        hideSwitchPokemonMenu();
    });

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
    backButton_fight->show();
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

    if (wildPokemon->getHp() > 0) {
        QTimer::singleShot(500, this, [this]() {
            processEnemyTurn();
        });
    }
    move->use();

    qDebug() << playerPokemon->getName() << "used" << move->getName() << "and dealt" << damage << "damage.";

    updateInfo();
    messageLabel->setText(QString("%1 used %2!").arg(playerPokemon->getName()).arg(move->getName()));

    fightMenu->hide();

    if (wildPokemon->isFainted()) {
        messageLabel->setText(QString("%1 fainted!").arg(wildPokemon->getName()));

        // 執行升級
        playerPokemon->levelUp();
        messageLabel->setText( "beats " +wildPokemon->getName()+"\n"+playerPokemon->getName() + "level up!");

        // 延遲 1000ms 後關閉戰鬥
        QTimer::singleShot(1000, this, [this]() {
            emit battleEnded();
            close();
        });
    }
}

void BattleSceneWidget::onBagClicked() {
    messageLabel->setText("Choose an item to use:");
    for (auto btn : actionButtons) {
        btn->hide();
    }
    showBagMenu();
}

void BattleSceneWidget::onPokemonClicked() {
    showSwitchPokemonMenu();
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

void BattleSceneWidget::processEnemyTurn() {
    // 1) 確認敵人還活著
    if (wildPokemon->getHp() <= 0) {
        messageLabel->setText(wildPokemon->getName() + " fainted!");
        qDebug() << "[EnemyTurn] Enemy fainted, skipping enemy attack.";
        return;
    }

    // 2) 正確隨機選擇技能
    QVector<Move*> moves = wildPokemon->getMoves();
    Move* selectedMove = nullptr;

    if (!moves.isEmpty()) {
        int idx = QRandomGenerator::global()->bounded(moves.size());
        selectedMove = moves[idx];
    }

    QString moveName;
    int damage;

    if (selectedMove) {
        moveName = selectedMove->getName();
        damage = selectedMove->getPower();
    } else {
        moveName = "Tackle";
        damage = 10;
    }

    // 顯示使用技能訊息
    QString useMoveMsg = wildPokemon->getName() + " uses " + moveName + "!";
    messageLabel->setText(useMoveMsg);
    qDebug() << "[EnemyTurn]" << useMoveMsg;

    // 3) 計算傷害
    int currentHp = playerPokemon->getHp();
    int calcDamage = (damage + wildPokemon->getAttack() - playerPokemon->getDefense()) * wildPokemon->getLevel();
    if (calcDamage < 1) calcDamage = 1;
    int newHp = qMax(0, currentHp - calcDamage);
    playerPokemon->setHp(newHp);

    // 更新畫面
    playerHpLabel->setText(QString("HP: %1/%2").arg(newHp).arg(playerPokemon->getMaxHp()));
    playerHpBar->setValue(newHp);

    QString damageMsg = wildPokemon->getName() + " deals " +
                        QString::number(calcDamage) + " damage to " +
                        playerPokemon->getName() + "!";
    messageLabel->setText(damageMsg);
    qDebug() << "[EnemyTurn]" << damageMsg;

    // 4) 玩家死亡檢查
    if (newHp <= 0) {
        QString faintMsg = playerPokemon->getName() + " fainted!";
        messageLabel->setText(faintMsg);
        qDebug() << "[EnemyTurn]" << faintMsg;
        // TODO: 銜接換寶可夢流程
    }

    // 5) 停頓 0.5 秒後顯示輪到玩家
    QTimer::singleShot(1000, this, [this]() {
        messageLabel->setText("Your turn!");
        qDebug() << "[EnemyTurn] Player's turn begins.";
    });
}

void BattleSceneWidget::showBagMenu() {
    QString info = QString("Potion: x%1    Ether: x%2    Poké Ball: x%3")
                       .arg(bag->getItemCount("Potion"))
                       .arg(bag->getItemCount("Ether"))
                       .arg(bag->getItemCount("Poké Ball"));

    bagOverlayLabel->setText("Choose an item to use:\nPotion, Ether, or Poké Ball");
    bagOverlayLabel->show();
    bagItemLabel->setText(info);
    bagItemLabel->show();
    potionButton->show();
    etherButton->show();
    pokeballButton->show();
    backButton_bag ->show();

    // 按鈕禁用處理
    potionButton->setEnabled(bag->getItemCount("Potion") > 0);
    etherButton->setEnabled(bag->getItemCount("Ether") > 0);
    pokeballButton->setEnabled(bag->getItemCount("Poké Ball") > 0);

    for (auto btn : actionButtons) {
        btn->setEnabled(false);
    }
}


void BattleSceneWidget::hideBagMenu() {
    bagItemLabel->hide();
    potionButton->hide();
    etherButton->hide();
    pokeballButton->hide();
    backButton_bag ->hide();

    for (auto btn : actionButtons) {
        btn->setEnabled(true);
        btn->show();
    }
    bagOverlayLabel->hide();
}

void BattleSceneWidget::showEtherMenu() {
    etherMenu->show();
    backButton_ether->show();
    for (int i = 0; i < 4; ++i) {
        QVector<Move*> moves = playerPokemon->getMoves();
        if (i < moves.size()) {
            Move *move = moves[i];
            etherSkillButtons[i]->setText(move->getName() + QString(" (%1/%2)").arg(move->getCurrentPp()).arg(move->getMaxPp()));
            etherSkillButtons[i]->setEnabled(true);
        } else {
            etherSkillButtons[i]->setText("No Skill");
            etherSkillButtons[i]->setEnabled(false);
        }
        etherSkillButtons[i]->show();
    }
}

void BattleSceneWidget::hideEtherMenu() {
    etherMenu->hide();
    backButton_ether->hide();
    for (int i = 0; i < 4; ++i) {
        etherSkillButtons[i]->hide();
    }
    hideBagMenu(); // 回到Bag關閉流程
}

void BattleSceneWidget::showSwitchPokemonMenu() {
    switchPokemonMenu->show();
    backButton_switchPokemon->show();

    QVector<Pokemon*> pokemons = collection->getAllPokemons();
    for (int i = 0; i < 4; ++i) {
        if (i < pokemons.size()) {
            Pokemon* p = pokemons[i];
            QString buttonText = p->getName();
            if (p == playerPokemon) {
                buttonText += " \n(Now)";
            }
            switchPokemonButtons[i]->setText(buttonText);

            // 防呆：若為當前 Pokémon 或已死亡，按鈕無法點擊
            if (p == playerPokemon || p->getHp() <= 0) {
                switchPokemonButtons[i]->setEnabled(false);
            } else {
                switchPokemonButtons[i]->setEnabled(true);
            }

            switchPokemonInfoLabels[i]->setText(QString("Lv:%1 HP:%2/%3")
                .arg(p->getLevel())
                .arg(p->getHp())
                .arg(p->getMaxHp()));
            switchPokemonInfoLabels[i]->show();
        } else {
            switchPokemonButtons[i]->setText("No Pokémon");
            switchPokemonButtons[i]->setEnabled(false);
            switchPokemonInfoLabels[i]->setText("");
            switchPokemonInfoLabels[i]->hide();
        }
        switchPokemonButtons[i]->show();
    }

    // 隱藏 Action 按鈕
    for (auto btn : actionButtons) {
        btn->hide();
    }
}


void BattleSceneWidget::hideSwitchPokemonMenu() {
    switchPokemonMenu->hide();
    backButton_switchPokemon->hide();
    for (int i = 0; i < 4; ++i) {
        switchPokemonButtons[i]->hide();
        switchPokemonInfoLabels[i]->hide();
    }

    // 回到 Action Menu
    for (auto btn : actionButtons) {
        btn->show();
        btn->setEnabled(true);
    }
}


void BattleSceneWidget::set_background(){
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/battle/data/battle/battle_scene.png").scaled(size()));
    backgroundLabel->setGeometry(0, 0, 525, 450);
}

void BattleSceneWidget::set_playerPokemon(){
    playerPokemonLabel = new QLabel(this);
    playerPokemonLabel->setPixmap(QPixmap(playerPokemon->getImagePath(false)).scaled(120, 120));
    playerPokemonLabel->move(50, 180);

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

}

void BattleSceneWidget::set_wildPokemon(){

    // 野生寶可夢圖片
    wildPokemonLabel = new QLabel(this);
    wildPokemonLabel->setPixmap(QPixmap(wildPokemon->getImagePath(true)).scaled(120, 120));
    wildPokemonLabel->move(340, 60);

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
}

void BattleSceneWidget::set_messageLabel(){
    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: white; font-weight: bold;");
    messageLabel->setGeometry(36, 350, 400, 60);
}

void BattleSceneWidget::set_actionMenu(){
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
}

void BattleSceneWidget::set_fightMenu(){
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

    backButton_fight = new QPushButton("Back", fightMenu);
    backButton_fight->setGeometry(460, 100, 60, 30);
    backButton_fight->setStyleSheet(buttonStyle);
    backButton_fight->hide();
    connect(backButton_fight, &QPushButton::clicked, this, [this]() {
        fightMenu->hide();
        for (auto btn : actionButtons) {
            btn->setEnabled(true);
            btn->show();
        }
    });
}

void BattleSceneWidget::set_battleBag(){
    bagOverlayLabel = new QLabel(this);
    bagOverlayLabel->setGeometry(0, 317, 525, 133);
    bagOverlayLabel->setStyleSheet("background-color: white; color: black; font-weight: bold;");
    bagOverlayLabel->setAlignment(Qt::AlignTop);
    bagOverlayLabel->hide();

    bagItemLabel = new QLabel(this);
    bagItemLabel->setStyleSheet("color: black; font-weight: bold;");
    bagItemLabel->setGeometry(20, 350, 400, 40);
    bagItemLabel->hide();

    backButton_bag  = new QPushButton("Back", this);
    backButton_bag ->setGeometry(450, 400, 60, 30);
    backButton_bag ->setStyleSheet(buttonStyle);
    backButton_bag ->hide();
    connect(backButton_bag , &QPushButton::clicked, this, [this]() {
        hideBagMenu();
    });

    // Potion Button
    potionButton = new QPushButton("Use Potion", this);
    potionButton->setGeometry(20, 400, 120, 30);
    potionButton->setStyleSheet(buttonStyle);
    potionButton->hide();
    connect(potionButton, &QPushButton::clicked, this, [this]() {
        int count = bag->getItemCount("Potion");
        if (count > 0) {
            int heal = 20;
            int newHp = qMin(playerPokemon->getMaxHp(), playerPokemon->getHp() + heal);
            playerPokemon->setHp(newHp);
            bag->useItem("Potion");
            updateInfo();
            messageLabel->setText(playerPokemon->getName() + " restored HP!");
        } else {
            messageLabel->setText("No Potion left!");
        }
        hideBagMenu();
        QTimer::singleShot(500, this, [this]() {
            processEnemyTurn();
        });
    });

    // Ether Button
    etherButton = new QPushButton("Use Ether", this);
    etherButton->setGeometry(160, 400, 120, 30);
    etherButton->setStyleSheet(buttonStyle);
    etherButton->hide();
    connect(etherButton, &QPushButton::clicked, this, [this]() {
        int count = bag->getItemCount("Ether");
        if (count > 0) {
            showEtherMenu();
        } else {
            messageLabel->setText("No Ether left!");
            hideBagMenu();
        }
        QTimer::singleShot(500, this, [this]() {
            processEnemyTurn();
        });
    });

    // Poké Ball Button
    pokeballButton = new QPushButton("Throw Poké Ball", this);
    pokeballButton->setGeometry(300, 400, 140, 30);
    pokeballButton->setStyleSheet(buttonStyle);
    pokeballButton->hide();
    connect(pokeballButton, &QPushButton::clicked, this, [this]() {
        int count = bag->getItemCount("Poké Ball");
        if (count > 0) {
            double captureRate = 0.8 * (1.0 - (double)wildPokemon->getHp() / wildPokemon->getMaxHp());
            double roll = QRandomGenerator::global()->generateDouble();
            bag->useItem("Poké Ball");

            if (roll < captureRate) {
                // 捕捉成功
                collection->addPokemon(wildPokemon); // 加入玩家收藏，保留完整狀態
                messageLabel->setText(wildPokemon->getName() + " was caught and added to your bag!");

                QTimer::singleShot(1000, this, [this]() {
                    emit battleEnded();
                    close();
                });
            } else {
                // 捕捉失敗
                messageLabel->setText("The Pokémon broke free!");
                QTimer::singleShot(500, this, [this]() {
                    processEnemyTurn();
                });
            }
        } else {
            messageLabel->setText("No Poké Ball left!");
            QTimer::singleShot(500, this, [this]() {
                processEnemyTurn();
            });
        }
        hideBagMenu();
    });
}

void BattleSceneWidget::set_etherMenu(){
    etherMenu = new QWidget(this);
    etherMenu->setGeometry(0, 317, 525, 133);
    etherMenu->setStyleSheet("background-color: white;");
    etherMenu->hide();

    for (int i = 0; i < 4; ++i) {
        etherSkillButtons[i] = new QPushButton(etherMenu);
        etherSkillButtons[i]->setGeometry(20 + i * 125, 40, 120, 50);
        etherSkillButtons[i]->setStyleSheet(buttonStyle);
        etherSkillButtons[i]->hide();
        connect(etherSkillButtons[i], &QPushButton::clicked, this, [this, i]() {
            QVector<Move*> moves = playerPokemon->getMoves();
            if (i < moves.size()) {
                moves[i]->recoverPp();
                bag->useItem("Ether");
                messageLabel->setText(moves[i]->getName() + "'s PP restored!");
                hideEtherMenu();
            }
        });
    }
    backButton_ether = new QPushButton("Back", this);
    backButton_ether->setGeometry(450, 400, 60, 30);
    backButton_ether->setStyleSheet(buttonStyle);
    backButton_ether->hide();
    connect(backButton_ether, &QPushButton::clicked, this, [this]() {
        hideEtherMenu();
        showBagMenu();
    });
}
