#include "LabSceneWidget.h"
#include "SolidBarrier.h"
#include "NPCBarrier.h"
#include "PokemonCollection.h"
#include "LabTableBarrier.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

LabSceneWidget::LabSceneWidget(Bag *bag, PokemonCollection *pokemonCollection, QWidget *parent)
    : QWidget(parent), bag(bag), pokemonCollection(pokemonCollection) {
    setFixedSize(windowWidth, windowHeight);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();


    background.load(":/scene/data/scene/lab.png");
    mapWidth = background.width();
    mapHeight = background.height();

    player = new Player(215, 205); // 初始位置
    addLabBarrier();//障礙物
    exitZone = QRect(195, 430, 30, 20); //出口

    // posLabel = new QLabel(this);
    // posLabel->setStyleSheet("color: white; font-weight: bold; background-color: rgba(0,0,0,128);");
    // posLabel->setGeometry(10, 10, 200, 20);
}

void LabSceneWidget::addLabBarrier()
{
    // barriers.append(new SolidBarrier(50, 50, 100, 50));  // 放置牆壁
    barriers.append(new NPCBarrier(
        175, 135, 35, 48,
        {"I am Professor Oak. Welcome to my laboratory!",
         "You can choose one from three Poké Balls \nas your initial Pokémon in Laboratory."},
        ":/other/data/NPC.png"
    ));
    barriers.append(new SolidBarrier(0, 10, 425, 60));      //上方書架
    barriers.append(new SolidBarrier(280, 245, 145, 65));   //右邊書架
    barriers.append(new SolidBarrier(0, 245, 175, 65));     //左邊書架
    barriers.append(new SolidBarrier(0, 90, 30, 65));       //左邊兩台電腦
    barriers.append(new SolidBarrier(30, 105, 75, 85));     //傳送器
    barriers.append(new SolidBarrier(415, 380, 35, 60));    //右下花瓶
    barriers.append(new SolidBarrier(0, 380, 35, 60));      //左下花瓶

    barriers.append(new LabTableBarrier(280, 130, 100, 50));
}

void LabSceneWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.fillRect(rect(), Qt::black); // 填滿黑色背景

    // 計算圖片左上在視窗中的繪製位置，使玩家保持螢幕中央
    int playerScreenX = windowWidth / 2 - player->getRect().width() / 2;
    int playerScreenY = windowHeight / 2 - player->getRect().height() / 2;

    int bgX = player->getX() + player->getRect().width() / 2 - windowWidth / 2;
    int bgY = player->getY() + player->getRect().height() / 2 - windowHeight / 2;

    // 計算圖片來源與目標繪製區域
    QRect srcRect(
        std::max(0, bgX),
        std::max(0, bgY),
        std::min(windowWidth, mapWidth - bgX),
        std::min(windowHeight, mapHeight - bgY)
    );

    QRect targetRect(
        std::max(0, -bgX),
        std::max(0, -bgY),
        srcRect.width(),
        srcRect.height()
    );

    // 繪製背景
    painter.drawPixmap(targetRect, background, srcRect);

    // 繪製玩家在中央
    QPixmap currentPixmap = player->getCurrentPixmap();
    if (!currentPixmap.isNull()) {
        painter.drawPixmap(playerScreenX, playerScreenY, currentPixmap.scaled(player->getRect().size()));
    } else {
        painter.fillRect(playerScreenX, playerScreenY, player->getRect().width(), player->getRect().height(), Qt::red);
    }

    // 顯示三顆 Poké Ball
    QPixmap ballPixmap(":/other/data/ball.png");
    int ballWidth = 21;
    int ballHeight = 30;

    // 實際位置（地圖絕對座標）
    QPoint ballPositions[3] = {
        QPoint(290, 140),
        QPoint(320, 140),
        QPoint(350, 140)
    };

    for (const QPoint &pos : ballPositions) {
        int drawX = pos.x() - bgX;
        int drawY = pos.y() - bgY;
        painter.drawPixmap(drawX, drawY, ballWidth, ballHeight, ballPixmap);
    }

    // 繪製 NPC
    for (const Barrier* barrier : barriers) {
        const NPCBarrier* npc = dynamic_cast<const NPCBarrier*>(barrier);
        if (npc) {
            int drawX = npc->getRect().x() - bgX;
            int drawY = npc->getRect().y() - bgY;
            npc->drawAt(&painter, drawX, drawY); // 需在 NPCBarrier 新增 drawAt()
        }
    }
}

void LabSceneWidget::keyPressEvent(QKeyEvent *event) {
    int step = 5;
    player->setWalking(true);
    int dx = 0, dy = 0;

    if (event->key() == Qt::Key_Left) {
        player->setDirection(Player::LEFT);
        dx = -step;
    } else if (event->key() == Qt::Key_Right) {
        player->setDirection(Player::RIGHT);
        dx = step;
    } else if (event->key() == Qt::Key_Up) {
        player->setDirection(Player::UP);
        dy = -step;
    } else if (event->key() == Qt::Key_Down) {
        if (exitZone.intersects(player->getRect()) && event->key() == Qt::Key_Down) {
                emit enterTown();
                return; // 避免繼續移動後執行 update
            }
        player->setDirection(Player::DOWN);
        dy = step;
    }

    else if (event->key() == Qt::Key_A) { // 互動鍵
        qDebug() << "Press A for interaction";
        for (Barrier* barrier : barriers) {
            NPCBarrier* npc = dynamic_cast<NPCBarrier*>(barrier);
            if (npc && npc->canInteract(player->getX(), player->getY())) {
                qDebug() << "NPC interaction triggered";
                npc->interact(this); // 傳入 LabSceneWidget 作為 parent
                break;
            }
            // LabTable 互動
            else if (auto table = dynamic_cast<LabTableBarrier*>(barrier)) {
                if (table->canInteract(player->getX(), player->getY())) {
                    table->interactWithPlayer(this, pokemonCollection);
                    canMove = false;
                    break;
                }
            }
        }
    }
    else if (event->key() == Qt::Key_B) {
        if (bagWidget && bagWidget->isVisible()) {
            bagWidget->close();
            bagWidget = nullptr;
            canMove = true;
        } else {
            bagWidget = new BagWidget(bag, pokemonCollection, this);
            int x = (this->width() - bagWidget->width()) / 2;
            int y = (this->height() - bagWidget->height()) / 2;
            bagWidget->move(std::max(0, x), std::max(0, y));
            bagWidget->show();
            canMove = false;
        }
    }

    int nextX = player->getX() + dx;
    int nextY = player->getY() + dy;

    // 執行 Barrier 碰撞判斷
    bool blocked = false;
    for (const Barrier* barrier : barriers) {
        if (!barrier->isPassable(nextX, nextY, player->getDirection())) {
            blocked = true;
            break;
        }
    }

    // 背包開啟時禁止移動
    if (!blocked && canMove) {
        player->move(dx, dy, mapWidth, mapHeight);
    }
    player->updateWalkFrame(); // 切換動畫幀
    // qDebug() << "Key pressed:" << event->key();

    // posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();
}

void LabSceneWidget::keyReleaseEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    player->setWalking(false);

    // posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();
}

