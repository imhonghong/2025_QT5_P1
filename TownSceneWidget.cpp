#include "TownSceneWidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

TownSceneWidget::TownSceneWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(windowWidth, windowHeight);
    setFocusPolicy(Qt::StrongFocus);

    background.load(":/scene/data/scene/Town.png");
    mapWidth = background.width();
    mapHeight = background.height();

    player = new Player(670, 700); // Town 中央起始
    addTownBarrier();

    posLabel = new QLabel(this);
    posLabel->setStyleSheet("color: white; font-weight: bold; background-color: rgba(0,0,0,128);");
    posLabel->setGeometry(10, 10, 200, 20);
}

void TownSceneWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.fillRect(rect(), Qt::black); // 背景填黑

    // 計算圖片左上在視窗中的繪製位置，使玩家保持螢幕中央
    int bgX = player->getX() + player->getRect().width() / 2 - windowWidth / 2;
    bgX = (bgX < 0) ? 0 : (bgX > mapWidth - windowWidth) ? (mapWidth - windowWidth) : bgX;

    int bgY = player->getY() + player->getRect().height() / 2 - windowHeight / 2;
    bgY = (bgY < 0) ? 0 : (bgY > mapHeight - windowHeight) ? (mapHeight - windowHeight) : bgY;

    int playerScreenX = player->getX() - bgX;
    int playerScreenY = player->getY() - bgY;
    // 計算來源區塊 (視窗大小) 與目標繪製位置
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

    // 畫玩家在中央
    QPixmap currentPixmap = player->getCurrentPixmap();
    if (!currentPixmap.isNull()) {
        painter.drawPixmap(playerScreenX, playerScreenY, currentPixmap.scaled(player->getRect().size()));
    } else {
        painter.fillRect(playerScreenX, playerScreenY, player->getRect().width(), player->getRect().height(), Qt::red);
    }

    painter.setBrush(QColor(255, 0, 0, 100)); // 半透明紅色
    for (const Barrier* barrier : barriers) {
        int drawX = barrier->getRect().x() - bgX;
        int drawY = barrier->getRect().y() - bgY;
        painter.drawRect(drawX, drawY, barrier->getRect().width(), barrier->getRect().height());
    }

    for (const Barrier* barrier : barriers) {
        const NPCBarrier* npc = dynamic_cast<const NPCBarrier*>(barrier);
        if (npc) {
            int drawX = npc->getRect().x() - bgX;
            int drawY = npc->getRect().y() - bgY;
            npc->drawAt(&painter, drawX, drawY);
        }
    }

}

void TownSceneWidget::keyPressEvent(QKeyEvent *event) {
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
        // 出口區域 Town ➔ Grassland
        QRect exitToGrassland(500, 0, 80, 20); // 根據 Town 地圖位置調整
        if (exitToGrassland.intersects(player->getRect()) && event->key() == Qt::Key_Up) {
            emit enterGrassland();
            qDebug() << "go to grassLand";
            return;
        }
        QRect exitToLab(670, 695, 30, 10); // 範例位置：下方中央，依實際地圖微調
        if (exitToLab.intersects(player->getRect()) && event->key() == Qt::Key_Up) {
            emit returnToLab();
            return;
        }
        player->setDirection(Player::UP);
        dy = -step;
    } else if (event->key() == Qt::Key_Down) {
        player->setDirection(Player::DOWN);
        dy = step;
    }
    else if (event->key() == Qt::Key_A) { // 互動鍵
        for (Barrier* barrier : barriers) {
            NPCBarrier* npc = dynamic_cast<NPCBarrier*>(barrier);
            if (npc && npc->canInteract(player->getX(), player->getY())) {
                npc->interact(this);
                break;
            }
        }
    }
    int nextX = player->getX() + dx;
    int nextY = player->getY() + dy;
    bool canMove = true;

    // 檢查 Barrier 碰撞
    for (const Barrier* barrier : barriers) {
        if (!barrier->isPassable(nextX, nextY, player->getDirection())) {
            canMove = false;
            break;
        }
    }

    if (canMove) {
        player->move(dx, dy, mapWidth, mapHeight);
    }

    player->updateWalkFrame();
    posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();
}

void TownSceneWidget::keyReleaseEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    player->setWalking(false);
    posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();
}

void TownSceneWidget::addTownBarrier()
{
    barriers.append(new SolidBarrier(295, 850, 170, 150)); //水池
    // barriers.append(new SolidBarrier(750, 952, 250, 8)); //右下方樹叢
    barriers.append(new SolidBarrier(915, 0, 85, 1000)); //右邊樹叢
    barriers.append(new SolidBarrier(580, 0, 420, 95)); //右上樹叢
    barriers.append(new SolidBarrier(0, 0, 500, 95)); //左上樹叢
    barriers.append(new SolidBarrier(0, 0, 85, 1000)); //左邊樹叢
    // barriers.append(new SolidBarrier(0, 952, 240, 8)); //左下方樹叢
    barriers.append(new SolidBarrier(170, 332, 25, 70)); //信箱 1
    barriers.append(new SolidBarrier(205, 172, 210, 230)); //房子 1
    barriers.append(new SolidBarrier(545, 332, 25, 70)); //信箱 2
    barriers.append(new SolidBarrier(580, 172, 210, 230)); //房子 2
    barriers.append(new SolidBarrier(205, 550, 210, 40)); // 欄杆1
    barriers.append(new SolidBarrier(540, 805, 245, 40)); // 欄杆2
    barriers.append(new SolidBarrier(540, 470, 290, 225)); // 大房1

    barriers.append(new NPCBarrier(375, 550, 40, 40,
        {"This is Pallet Town. Begin your adventure!"},""));
    barriers.append(new NPCBarrier(205, 700, 40, 40,
        {"This is Pallet Town. Begin your adventure!"},""));
    barriers.append(new NPCBarrier(670, 805, 40, 40,
        {"This is Pallet Town. Begin your adventure!"},""));

}

