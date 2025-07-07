#include "BattleSceneWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>

BattleSceneWidget::BattleSceneWidget(Pokemon *wildPokemon, Bag *bag, PokemonCollection *collection, QWidget *parent)
    : QWidget(parent), wildPokemon(wildPokemon), bag(bag), collection(collection) {

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
    set_background();
    set_playerPokemon();
    set_wildPokemon();
    set_messageLabel();
    set_actionMenu();
    set_fightMenu();
    set_battleBag();
    set_etherMenu();
    set_switchPokemonMenu();
}

void BattleSceneWidget::set_background() {
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/battle/data/battle/battle_scene.png").scaled(size()));
    backgroundLabel->setGeometry(0, 0, 525, 450);
}

void BattleSceneWidget::set_playerPokemon() {
    playerPokemonLabel = new QLabel(this);
    playerPokemonLabel->setPixmap(QPixmap(playerPokemon->getImagePath(false)).scaled(120, 120));
    playerPokemonLabel->move(50, 180);

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

void BattleSceneWidget::set_wildPokemon() {
    wildPokemonLabel = new QLabel(this);
    wildPokemonLabel->setPixmap(QPixmap(wildPokemon->getImagePath(true)).scaled(120, 120));
    wildPokemonLabel->move(340, 60);

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

void BattleSceneWidget::set_messageLabel() {
    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: white; font-weight: bold;");
    messageLabel->setGeometry(36, 350, 450, 60);
}

void BattleSceneWidget::set_actionMenu() {
    actionMenu = new QWidget(this);
    actionMenu->setGeometry(281, 337, 228, 92);
    actionMenu->setStyleSheet("background: transparent;");
    QGridLayout *layout = new QGridLayout(actionMenu);
    layout->setSpacing(5);
    layout->setContentsMargins(0, 0, 0, 0);

    QStringList actions = {"Fight", "Bag", "Pokémon", "Run"};
    for (int i = 0; i < 4; ++i) {
        QFrame *frame = new QFrame(actionMenu);
        frame->setFrameShape(QFrame::Box);
        frame->setStyleSheet(FRAME_BORDER_NORMAL);

        QLabel *label = new QLabel(actions[i], frame);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
        QVBoxLayout *vbox = new QVBoxLayout(frame);
        vbox->setContentsMargins(0, 0, 0, 0);
        vbox->setSpacing(0);
        vbox->addWidget(label);

        frame->setFixedSize(110, 40);
        layout->addWidget(frame, i / 2, i % 2);
        actionFrames.append(frame);

        frame->installEventFilter(this);
    }

    selectedActionIndex = 0;
    updateActionFrameFocus();
}

void BattleSceneWidget::updateActionFrameFocus() {
    for (int i = 0; i < actionFrames.size(); ++i) {
        actionFrames[i]->setStyleSheet(i == selectedActionIndex ? FRAME_BORDER_SELECTED : FRAME_BORDER_NORMAL);
    }
}

void BattleSceneWidget::executeAction(int idx) {
    switch (idx) {
        case 0: onFightClicked(); break;
        case 1: onBagClicked(); break;
        case 2: onPokemonClicked(); break;
        case 3: onRunClicked(); break;
    }
}

void BattleSceneWidget::set_fightMenu() {
    fightMenu = new QWidget(this);
    fightMenu->setGeometry(0, 317, 525, 133);
    fightMenu->hide();

    fightMenuBackground = new QLabel(fightMenu);
    fightMenuBackground->setPixmap(QPixmap(":/battle/data/battle/fight_skill.png").scaled(525, 133));
    fightMenuBackground->setGeometry(0, 0, 525, 133);
    fightMenuBackground->lower();

    QGridLayout *grid = new QGridLayout(fightMenu);
    grid->setContentsMargins(15, 16, 190, 16);
    grid->setSpacing(5);

    QVector<Move*> moves = playerPokemon->getMoves();
    for (int i = 0; i < 4; ++i) {
        QFrame *frame = new QFrame(fightMenu);
        frame->setFrameShape(QFrame::Box);
        frame->setLineWidth(2);
        frame->setStyleSheet(FRAME_BORDER_NORMAL);

        QLabel *label = new QLabel(frame);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
        label->setText(i < moves.size() ? moves[i]->getName() : "No Skill");
        QVBoxLayout *vbox = new QVBoxLayout(frame);
        vbox->setContentsMargins(0, 0, 0, 0);
        vbox->setSpacing(0);
        vbox->addWidget(label);

        frame->setFixedSize(150, 50);
        grid->addWidget(frame, i / 2, i % 2);
        skillFrames.append(frame);

        frame->installEventFilter(this);
    }

    skillPPLabel = new QLabel("-- / --", fightMenu);
    skillPPLabel->setStyleSheet("color: black; font-weight: bold; font-size: 20px");
    skillPPLabel->move(440, 35);

    skillPowerLabel = new QLabel("--", fightMenu);
    skillPowerLabel->setStyleSheet("color: black; font-weight: bold; font-size: 20px");
    skillPowerLabel->move(440, 80);

    selectedSkillIndex = 0;
    updateSkillFrameFocus();
    updateSkillInfoDisplay(selectedSkillIndex);
}

void BattleSceneWidget::updateSkillFrameFocus() {
    for (int i = 0; i < skillFrames.size(); ++i) {
        skillFrames[i]->setStyleSheet(i == selectedSkillIndex ? FRAME_BORDER_SELECTED : FRAME_BORDER_NORMAL);
    }
}

void BattleSceneWidget::set_battleBag() {
    bagMenu = new QWidget(this);
    bagMenu->setGeometry(0, 317, 525, 133);
    bagMenu->setStyleSheet("background-color: white;");
    bagMenu->hide();

    QVBoxLayout *vLayout = new QVBoxLayout(bagMenu);
    vLayout->setContentsMargins(10, 10, 10, 10);
    vLayout->setSpacing(5);

    QLabel *title = new QLabel("Choose an item to use:", bagMenu);
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
    title->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(title);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setSpacing(10);
    hLayout->setContentsMargins(0, 0, 0, 0);

    QStringList items = {
        QString("Use Potion \n(1/%1)").arg(bag->getItemCount("Potion")),
        QString("Use Ether \n(1/%1)").arg(bag->getItemCount("Ether")),
        QString("Throw Poké Ball \n(1/%1)").arg(bag->getItemCount("Poké Ball")),
        "Back"
    };

    for (int i = 0; i < 4; ++i) {
        QFrame *frame = new QFrame(bagMenu);
        frame->setFrameShape(QFrame::Box);
        frame->setLineWidth(2);
        frame->setStyleSheet(FRAME_BORDER_NORMAL);

        QLabel *label = new QLabel(items[i], frame);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-size: 12px; font-weight: bold; color: black;");

        QVBoxLayout *vbox = new QVBoxLayout(frame);
        vbox->setContentsMargins(0, 0, 0, 0);
        vbox->setSpacing(0);
        vbox->addWidget(label);

        frame->setFixedSize(120, 50);
        hLayout->addWidget(frame);
        bagFrames.append(frame);

        frame->installEventFilter(this);
    }

    vLayout->addLayout(hLayout);

    selectedBagIndex = 0;
    updateBagFrameFocus();
}


void BattleSceneWidget::updateBagFrameFocus() {
    for (int i = 0; i < bagFrames.size(); ++i) {
        bagFrames[i]->setStyleSheet(i == selectedBagIndex ? FRAME_BORDER_SELECTED : FRAME_BORDER_NORMAL);
    }
}

void BattleSceneWidget::executeBagAction(int idx) {
    switch (idx) {
        case 0: { // Use Potion
            if (bag->getItemCount("Potion") > 0) {
                int heal = 20;
                int newHp = qMin(playerPokemon->getMaxHp(), playerPokemon->getHp() + heal);
                playerPokemon->setHp(newHp);
                bag->useItem("Potion");
                bagMenu->hide();
                updateInfo();
                QTimer::singleShot(500, this, [this]() { processEnemyTurn(); });
                messageLabel->setText(playerPokemon->getName() + " restored HP!");
            } else {
                messageLabel->setText("No Potion left!");
            }
            break;
        }
        case 1: { // Use Ether
            if (bag->getItemCount("Ether") > 0) {
                bagMenu->hide();
                etherMenu->show();
            } else {
                messageLabel->setText("No Ether left!");
            }
            break;
        }
        case 2: { // Throw Poké Ball
            if (bag->getItemCount("Poké Ball") > 0) {
                double captureRate = 0.5 + 0.5 * (1.0 - (double)wildPokemon->getHp() / wildPokemon->getMaxHp());
                double roll = QRandomGenerator::global()->generateDouble();
                bag->useItem("Poké Ball");
                if (roll < captureRate) {
                    collection->addPokemon(wildPokemon);
                    messageLabel->setText(wildPokemon->getName() + " was caught!");
                    QTimer::singleShot(1000, this, [this]() { emit battleEnded(); close(); });
                } else {
                    messageLabel->setText("The Pokémon broke free!");
                    QTimer::singleShot(500, this, [this]() { processEnemyTurn(); });
                }
            } else {
                messageLabel->setText("No Poké Ball left!");
            }
            break;
        }
        case 3: { // Back
            bagMenu->hide();
            actionMenu->show();
            break;
        }
    }
}

void BattleSceneWidget::updateEtherMenu() {
    QVector<Move*> moves = playerPokemon->getMoves();
    for (int i = 0; i < etherFrames.size(); ++i) {
        QLabel *label = etherFrames[i]->findChild<QLabel *>();
        if (label) {
            if (i < moves.size()) {
                Move *move = moves[i];
                label->setText(move->getName() + "\n"+ QString(" (%1/%2)").arg(move->getCurrentPp()).arg(move->getMaxPp()));
            } else {
                label->setText("No Skill");
            }
        }
    }
}

void BattleSceneWidget::set_etherMenu() {
    etherMenu = new QWidget(this);
    etherMenu->setGeometry(0, 317, 525, 133);
    etherMenu->setStyleSheet("background-color: white;");
    etherMenu->hide();

    QHBoxLayout *layout = new QHBoxLayout(etherMenu);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(10);

    QVector<Move*> moves = playerPokemon->getMoves();
    for (int i = 0; i < 4; ++i) {
        QFrame *frame = new QFrame(etherMenu);
        frame->setFrameShape(QFrame::Box);
        frame->setLineWidth(2);
        frame->setStyleSheet(FRAME_BORDER_NORMAL);

        QString labelText;
        if (i < moves.size()) {
            Move *move = moves[i];
            labelText = move->getName() + QString(" (%1/%2)").arg(move->getCurrentPp()).arg(move->getMaxPp());
        } else {
            labelText = "No Skill";
        }

        QLabel *label = new QLabel(labelText, frame);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
        QVBoxLayout *vbox = new QVBoxLayout(frame);
        vbox->setContentsMargins(0, 0, 0, 0);
        vbox->setSpacing(0);
        vbox->addWidget(label);

        frame->setFixedSize(120, 50);
        layout->addWidget(frame);
        etherFrames.append(frame);

        frame->installEventFilter(this);
    }

    selectedEtherIndex = 0;
    updateEtherFrameFocus();
}

void BattleSceneWidget::updateEtherFrameFocus() {
    for (int i = 0; i < etherFrames.size(); ++i) {
        etherFrames[i]->setStyleSheet(i == selectedEtherIndex ? FRAME_BORDER_SELECTED : FRAME_BORDER_NORMAL);
    }
}

void BattleSceneWidget::executeEtherAction(int idx) {
    QVector<Move*> moves = playerPokemon->getMoves();
    if (idx < moves.size()) {
        Move *move = moves[idx];
        move->recoverPp();
        bag->useItem("Ether");
        messageLabel->setText(move->getName() + "'s PP restored!");
        etherMenu->hide();
        QTimer::singleShot(500, this, [this]() { processEnemyTurn(); });
    } else {
        etherMenu->hide();
        bagMenu->show();
    }
}

void BattleSceneWidget::set_switchPokemonMenu() {
    switchPokemonMenu = new QWidget(this);
    switchPokemonMenu->setGeometry(0, 317, 525, 133);
    switchPokemonMenu->setStyleSheet("background-color: white;");
    switchPokemonMenu->hide();

    QHBoxLayout *layout = new QHBoxLayout(switchPokemonMenu);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(10);

    QVector<Pokemon*> pokemons = collection->getAllPokemons();
    for (int i = 0; i < 4; ++i) {
        QFrame *frame = new QFrame(switchPokemonMenu);
        frame->setFrameShape(QFrame::Box);
        frame->setLineWidth(2);
        frame->setStyleSheet(FRAME_BORDER_NORMAL);

        QString text;
        if (i < pokemons.size()) {
            Pokemon* p = pokemons[i];
            text = p->getName() + QString("\nLv:%1 HP:%2/%3")
                   .arg(p->getLevel()).arg(p->getHp()).arg(p->getMaxHp());
        } else {
            text = "No Pokémon";
        }

        QLabel *label = new QLabel(text, frame);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
        QVBoxLayout *vbox = new QVBoxLayout(frame);
        vbox->setContentsMargins(0, 0, 0, 0);
        vbox->setSpacing(0);
        vbox->addWidget(label);

        frame->setFixedSize(120, 50);
        layout->addWidget(frame);
        switchFrames.append(frame);

        frame->installEventFilter(this);
    }

    selectedSwitchIndex = 0;
    updateSwitchFrameFocus();
}

void BattleSceneWidget::updateSwitchFrameFocus() {
    for (int i = 0; i < switchFrames.size(); ++i) {
        switchFrames[i]->setStyleSheet(i == selectedSwitchIndex ? FRAME_BORDER_SELECTED : FRAME_BORDER_NORMAL);
    }
}

void BattleSceneWidget::executeSwitchPokemonAction(int idx) {
    QVector<Pokemon*> pokemons = collection->getAllPokemons();
    if (idx < pokemons.size()) {
        Pokemon* p = pokemons[idx];
        if (p->getHp() > 0 && p != playerPokemon) {
            playerPokemon = p;
            updateInfo();
            messageLabel->setText("Go! " + p->getName() + "!");
            playerPokemonLabel->setPixmap(QPixmap(playerPokemon->getImagePath(false)).scaled(120, 120));
            switchPokemonMenu->hide();
            QTimer::singleShot(1000, this, [this]() { processEnemyTurn(); });
        } else {
            messageLabel->setText("Cannot switch to this Pokémon!");
        }
    } else {
        switchPokemonMenu->hide();
        actionMenu->show();
    }
}

void BattleSceneWidget::onFightClicked() {
    actionMenu->hide();
    fightMenu->show();
    selectedSkillIndex = 0;
    updateSkillFrameFocus();
    updateSkillInfoDisplay(selectedSkillIndex);
}

void BattleSceneWidget::onBagClicked() {
    actionMenu->hide();
    bagMenu->show();
    selectedBagIndex = 0;
    updateBagFrameFocus();
    updateEtherMenu();
}

void BattleSceneWidget::onPokemonClicked() {
    actionMenu->hide();
    switchPokemonMenu->show();
    selectedSwitchIndex = 0;
    updateSwitchFrameFocus();
}

void BattleSceneWidget::onRunClicked() {
    messageLabel->setText("You ran away!");
    QTimer::singleShot(500, this, [this]() {
        emit battleEnded();
        close();
    });
}

void BattleSceneWidget::updateInfo() {
    playerHpLabel->setText(QString("HP: %1/%2").arg(playerPokemon->getHp()).arg(playerPokemon->getMaxHp()));
    playerHpBar->setValue(playerPokemon->getHp());
    wildHpBar->setValue(wildPokemon->getHp());
}

void BattleSceneWidget::processEnemyTurn() {
    if (wildPokemon->getHp() <= 0) {
        messageLabel->setText(wildPokemon->getName() + " fainted!");
        QTimer::singleShot(1000, this, [this]() {
            emit battleEnded();
            close();
        });
        return;
    }

    QVector<Move*> moves = wildPokemon->getMoves();
    Move* move = nullptr;
    if (!moves.isEmpty()) {
        int idx = QRandomGenerator::global()->bounded(moves.size());
        move = moves[idx];
    }

    QString moveName = move ? move->getName() : "Tackle";
    int damage = move ? move->getPower() : 10;

    QString msg = wildPokemon->getName() + " uses " + moveName + "!";
    messageLabel->setText(msg);

    int calcDamage = (damage + wildPokemon->getAttack() - playerPokemon->getDefense()) * wildPokemon->getLevel();
    if (calcDamage < 1) calcDamage = 1;

    playerPokemon->receiveDamage(calcDamage);
    updateInfo();

    QString damageMsg = wildPokemon->getName() + " dealt " +
                        QString::number(calcDamage) + " damage!";
    messageLabel->setText(damageMsg);

    if (playerPokemon->getHp() <= 0) {
        messageLabel->setText(playerPokemon->getName() + " fainted!");
        QTimer::singleShot(1000, this, [this]() {
            emit battleEnded();
            close();
        });
    } else {
        // 顯示 ActionMenu 以便繼續下一回合
        QTimer::singleShot(1000, this, [this]() {
            messageLabel->setText("Your turn!");
            actionMenu->show();
        });
    }
}


void BattleSceneWidget::updateSkillInfoDisplay(int idx) {
    QVector<Move*> moves = playerPokemon->getMoves();
    if (idx < moves.size()) {
        Move* move = moves[idx];
        skillPowerLabel->setText(QString("%1").arg(move->getPower()));
        skillPPLabel->setText(QString("%1/%2").arg(move->getCurrentPp()).arg(move->getMaxPp()));
    } else {
        skillPowerLabel->setText("--");
        skillPPLabel->setText("--/--");
    }
}

void BattleSceneWidget::handleEscKey() {
    if (fightMenu->isVisible()) {
        fightMenu->hide();
        actionMenu->show();
    } else if (bagMenu->isVisible()) {
        bagMenu->hide();
        actionMenu->show();
    } else if (etherMenu->isVisible()) {
        etherMenu->hide();
        bagMenu->show();
    } else if (switchPokemonMenu->isVisible()) {
        switchPokemonMenu->hide();
        actionMenu->show();
    } else {
        // 可視情況設計：若沒選單開啟則什麼都不做，或關閉戰鬥
        // close();
    }
}


void BattleSceneWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Q) {
        handleEscKey();
        return;
    }
    if (fightMenu->isVisible()) {
        handleMenuKeyPress(event, skillFrames, selectedSkillIndex,
                           [this](int idx) { useSkill(idx); },
                           [this](int idx) { updateSkillInfoDisplay(idx); },
                           2); // SkillMenu 是 2 列
    } else if (actionMenu->isVisible()) {
        handleMenuKeyPress(event, actionFrames, selectedActionIndex,
                           [this](int idx) { executeAction(idx); },
                           nullptr,
                           2); // ActionMenu 是 2 列
    } else if (bagMenu->isVisible()) {
        handleMenuKeyPress(event, bagFrames, selectedBagIndex,
                           [this](int idx) { executeBagAction(idx); },
                           nullptr,
                           4); // BagMenu 是 4 列（水平）
    } else if (etherMenu->isVisible()) {
        handleMenuKeyPress(event, etherFrames, selectedEtherIndex,
                           [this](int idx) { executeEtherAction(idx); },
                           nullptr,
                           4); // EtherMenu 是 4 列
    } else if (switchPokemonMenu->isVisible()) {
        handleMenuKeyPress(event, switchFrames, selectedSwitchIndex,
                           [this](int idx) { executeSwitchPokemonAction(idx); },
                           nullptr,
                           4); // SwitchPokemonMenu 是 4 列
    } else {
        QWidget::keyPressEvent(event);
    }
}

bool BattleSceneWidget::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (fightMenu->isVisible()) {
            for (int i = 0; i < skillFrames.size(); ++i) {
                if (watched == skillFrames[i]) {
                    selectedSkillIndex = i;
                    updateSkillFrameFocus();
                    updateSkillInfoDisplay(i);
                    return true;
                }
            }
        } else if (actionMenu->isVisible()) {
            for (int i = 0; i < actionFrames.size(); ++i) {
                if (watched == actionFrames[i]) {
                    selectedActionIndex = i;
                    updateActionFrameFocus();
                    return true;
                }
            }
        } else if (bagMenu->isVisible()) {
            for (int i = 0; i < bagFrames.size(); ++i) {
                if (watched == bagFrames[i]) {
                    selectedBagIndex = i;
                    updateBagFrameFocus();
                    return true;
                }
            }
        } else if (etherMenu->isVisible()) {
            for (int i = 0; i < etherFrames.size(); ++i) {
                if (watched == etherFrames[i]) {
                    selectedEtherIndex = i;
                    updateEtherFrameFocus();
                    return true;
                }
            }
        } else if (switchPokemonMenu->isVisible()) {
            for (int i = 0; i < switchFrames.size(); ++i) {
                if (watched == switchFrames[i]) {
                    selectedSwitchIndex = i;
                    updateSwitchFrameFocus();
                    return true;
                }
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void BattleSceneWidget::useSkill(int idx) {
    QVector<Move*> moves = playerPokemon->getMoves();
    if (idx < 0 || idx >= moves.size()) return;

    Move* move = moves[idx];
    if (move->getCurrentPp() <= 0) {
        messageLabel->setText("No PP left for this move!");
        return;
    }

    int damage = move->getPower() + playerPokemon->getAttack() - wildPokemon->getDefense();
    if (damage <= 0) damage = 1;
    wildPokemon->receiveDamage(damage);
    move->use();

    updateInfo();
    messageLabel->setText(playerPokemon->getName() + " used " + move->getName() + " and dealt " + QString::number(damage) + " damage!");
    fightMenu->hide();

    if (wildPokemon->getHp() <= 0) {
        messageLabel->setText("You win!");
        playerPokemon->levelUp();  // 🩹 加上升級
        updateInfo();              // 🩹 更新介面
        QTimer::singleShot(1000, this, [this]() {
            emit battleEnded();
            close();
        });
    } else {
        QTimer::singleShot(500, this, [this]() { processEnemyTurn(); });
    }
}

void BattleSceneWidget::handleMenuKeyPress(QKeyEvent *event,
                                           QVector<QFrame*> &frames,
                                           int &selectedIndex,
                                           std::function<void(int)> onEnter,
                                           std::function<void(int)> onUpdate,
                                           int columns) {

    int prevIndex = selectedIndex;

    if (event->key() == Qt::Key_Right) {
        if ((selectedIndex + 1) < frames.size() &&
            ((selectedIndex + 1) % columns != 0)) {
            selectedIndex++;
        }
    } else if (event->key() == Qt::Key_Left) {
        if ((selectedIndex % columns) != 0) {
            selectedIndex--;
        }
    } else if (event->key() == Qt::Key_Up) {
        if (selectedIndex - columns >= 0) {
            selectedIndex -= columns;
        }
    } else if (event->key() == Qt::Key_Down) {
        if (selectedIndex + columns < frames.size()) {
            selectedIndex += columns;
        }
    } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space) {
        if (onEnter) onEnter(selectedIndex);
        return;
    } else {
        QWidget::keyPressEvent(event);
        return;
    }

    // Update focus style
    for (int i = 0; i < frames.size(); ++i) {
        frames[i]->setStyleSheet(i == selectedIndex ? FRAME_BORDER_SELECTED : FRAME_BORDER_NORMAL);
    }

    if (onUpdate && prevIndex != selectedIndex) {
        onUpdate(selectedIndex);
    }
}

