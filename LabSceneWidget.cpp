#include "LabSceneWidget.h"
#include "SolidBarrier.h"
#include "NPCBarrier.h"
#include "Ledge.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

LabSceneWidget::LabSceneWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(windowWidth, windowHeight);
    setFocusPolicy(Qt::StrongFocus);

    background.load(":/scene/data/scene/lab.png");
    mapWidth = background.width();
    mapHeight = background.height();

    player = new Player(215, 205); // 初始位置
    addLabBarrier();

    posLabel = new QLabel(this);
    posLabel->setStyleSheet("color: white; font-weight: bold; background-color: rgba(0,0,0,128);");
    posLabel->setGeometry(10, 10, 200, 20);
}

void LabSceneWidget::addLabBarrier()
{
    // barriers.append(new SolidBarrier(50, 50, 100, 50));  // 放置牆壁
    barriers.append(new NPCBarrier(200, 100, 35, 48));      // 放置 NPC
    barriers.append(new SolidBarrier(0, 10, 425, 60));      //上方書架
    barriers.append(new SolidBarrier(280, 245, 145, 65));   //右邊書架
    barriers.append(new SolidBarrier(0, 245, 175, 65));     //左邊書架
    barriers.append(new SolidBarrier(0, 90, 30, 65));       //左邊兩台電腦
    barriers.append(new SolidBarrier(30, 105, 75, 85));     //傳送器
    barriers.append(new SolidBarrier(415, 380, 35, 60));    //右下花瓶
    barriers.append(new SolidBarrier(0, 380, 35, 60));      //左下花瓶
    // barriers.append(new Ledge(300, 200, 100, 20, Ledge::DOWN)); // 放置 ledge
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
        }
    }

    int nextX = player->getX() + dx;
    int nextY = player->getY() + dy;
    bool canMove = true;

    // 執行 Barrier 碰撞判斷
    for (const Barrier* barrier : barriers) {
        if (!barrier->isPassable(nextX, nextY, player->getDirection())) {
            canMove = false;
            break;
        }
    }

    // 若無碰撞，執行移動
    if (canMove) {
        player->move(dx, dy, mapWidth, mapHeight);
    }
    player->updateWalkFrame(); // 切換動畫幀
    qDebug() << "Key pressed:" << event->key();

    posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();
}

void LabSceneWidget::keyReleaseEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    player->setWalking(false);

    posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();
}

