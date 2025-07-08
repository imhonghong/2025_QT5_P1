#include "GrasslandSceneWidget.h"
#include "Barrier.h"
#include "NPCBarrier.h"
#include "SolidBarrier.h"
#include "Ledge.h"
#include "TallGrass.h"

#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>

GrasslandSceneWidget::GrasslandSceneWidget(Bag *bag, PokemonCollection *pokemonCollection, QWidget *parent)
    : QWidget(parent), bag(bag), pokemonCollection(pokemonCollection) {
    setFixedSize(windowWidth, windowHeight);
    setFocusPolicy(Qt::StrongFocus);

    background.load(":/scene/data/scene/GrassLand.png");
    mapWidth = background.width();
    mapHeight = background.height();

    player = new Player(525, 1619);

    // posLabel = new QLabel(this);
    // posLabel->setStyleSheet("color: white; font-weight: bold; background-color: rgba(0,0,0,128);");
    // posLabel->setGeometry(10, 10, 200, 20);
    addGrasslandBarrier();
}

void GrasslandSceneWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    int bgX = player->getX() + player->getRect().width() / 2 - windowWidth / 2;
    bgX = (bgX < 0) ? 0 : (bgX > mapWidth - windowWidth) ? (mapWidth - windowWidth) : bgX;

    int bgY = player->getY() + player->getRect().height() / 2 - windowHeight / 2;
    bgY = (bgY < 0) ? 0 : (bgY > mapHeight - windowHeight) ? (mapHeight - windowHeight) : bgY;

    int playerScreenX = player->getX() - bgX;
    int playerScreenY = player->getY() - bgY;

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

    painter.drawPixmap(targetRect, background, srcRect);

    QPixmap currentPixmap = player->getCurrentPixmap();
    if (!currentPixmap.isNull()) {
        painter.drawPixmap(playerScreenX, playerScreenY, currentPixmap.scaled(player->getRect().size()));
    } else {
        painter.fillRect(playerScreenX, playerScreenY, player->getRect().width(), player->getRect().height(), Qt::red);
    }

    /*
    painter.setBrush(QColor(0, 255, 0, 100)); // 半透明綠色
    for (const Barrier* barrier : barriers) {
        const TallGrass* grass = dynamic_cast<const TallGrass*>(barrier);
        if (grass) {
            QRect drawRect = grass->getTriggerRect();
            int drawX = drawRect.x() - bgX;
            int drawY = drawRect.y() - bgY;
            painter.drawRect(drawX, drawY, drawRect.width(), drawRect.height());
        }
    }
    */
}

void GrasslandSceneWidget::keyPressEvent(QKeyEvent *event) {
    int step = 5;
    player->setWalking(true);
    int dx = 0, dy = 0;
    if (!canMove) {
        if (event->key() == Qt::Key_B) {
            if (bagWidget && bagWidget->isVisible()) {
                bagWidget->close();
                bagWidget = nullptr;
                canMove = true;
                return;
            }
        }
        return; // 禁止其他按鍵
    }

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
        QRect exitToTown(500, 1660, 50, 7); // 根據地圖調整
        if (exitToTown.intersects(player->getRect()) && event->key() == Qt::Key_Down) {
            emit returnToTown();
            return;
        }
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
    else if (event->key() == Qt::Key_B) {
        if (bagWidget && bagWidget->isVisible()) {
            bagWidget->close();
            bagWidget = nullptr;
            canMove = true;
        } else {
            bagWidget = new BagWidget(bag, pokemonCollection, this);
            bagWidget->show();
            canMove = false;
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

    // 檢查是否觸發 Tall Grass 遭遇野生寶可夢
    for (Barrier* barrier : barriers) {
        TallGrass* grass = dynamic_cast<TallGrass*>(barrier);
        if (grass && !grass->hasTriggered && grass->getTriggerRect().intersects(player->getRect())) {
            grass->hasTriggered = true;
            // 產生隨機野生寶可夢
            int idx = QRandomGenerator::global()->bounded(3);
            QString name;
            if (idx == 0) name = "Charmander";
            else if (idx == 1) name = "Squirtle";
            else name = "Bulbasaur";
            Pokemon *wildPokemon = Pokemon::createPokemon(name, 1, true);

            emit enterBattle(wildPokemon);
            break; // 一次只觸發一次
        }
    }

    player->updateWalkFrame();
    // posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();
}

void GrasslandSceneWidget::keyReleaseEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    player->setWalking(false);
    // posLabel->setText(QString("X: %1, Y: %2").arg(player->getX()).arg(player->getY()));
    update();

}

void GrasslandSceneWidget::addGrasslandBarrier(){
    barriers.append(new SolidBarrier(0, 0, 80, mapHeight)); //左邊牆壁
    barriers.append(new SolidBarrier(0, 1505, 500, 157)); //左下圍欄
    barriers.append(new SolidBarrier(585, 1505, 415, 157)); //左下圍欄
    barriers.append(new SolidBarrier(915, 0, 80, mapHeight)); //右邊牆壁
    barriers.append(new SolidBarrier(580, 0, 330, 70)); //右上樹叢
    barriers.append(new SolidBarrier(80, 0, 335, 70)); //右上樹叢
    barriers.append(new SolidBarrier(330, 130, 85, 360)); //中上樹叢
    barriers.append(new SolidBarrier(80, 590, 80, 110));
    barriers.append(new SolidBarrier(415, 585, 250, 120));
    barriers.append(new SolidBarrier(80, 1005, 420, 115));

    barriers.append(new NPCBarrier(370, 1290, 50, 45,
        {"This is Pallet Town. Begin your adventure!"},""));

    barriers.append(new Ledge(80, 230, 250, 20, Ledge::DOWN)); // 放置 ledge
    barriers.append(new Ledge(80, 440, 250, 20, Ledge::DOWN)); // Ledge::允許通過方向
    barriers.append(new Ledge(160, 645, 250, 20, Ledge::DOWN));
    barriers.append(new Ledge(80, 850, 85, 25, Ledge::DOWN));
    barriers.append(new Ledge(455, 850, 440, 25, Ledge::DOWN));
    barriers.append(new Ledge(745, 1105, 155, 20, Ledge::DOWN));
    barriers.append(new Ledge(420, 1310, 500, 25, Ledge::DOWN));
    barriers.append(new Ledge(415, 225, 250, 25, Ledge::DOWN));
    barriers.append(new Ledge(210, 850, 165, 25, Ledge::DOWN));
    barriers.append(new Ledge(80, 1310,170, 25, Ledge::DOWN));

    // long grass = 40 * 40;
    barriers.append(new TallGrass(500, 1455, 40*2, 40*4)); //下方入口
    barriers.append(new TallGrass(80, 1340, 40*9, 40*4)); //左下
    barriers.append(new TallGrass(625, 1340, 40*7, 40*4)); //右下
    barriers.append(new TallGrass(500, 1000, 40*6, 40*5)); //中間第一階
    barriers.append(new TallGrass(665, 545, 40*6, 40*5)); //中間第二階
    barriers.append(new TallGrass(415, 255, 40*12, 40*5)); //上面
}
