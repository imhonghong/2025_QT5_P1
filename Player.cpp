#include "Player.h"
#include <algorithm>

Player::Player(int x, int y, int w, int h)
    : x(x), y(y), width(w), height(h),
      direction(DOWN), isWalking(false), walkFrame(0) {

    // 預先載入所有方向與走路圖片
    pixmaps["B"] = QPixmap(":/player/data/player/player_B.png");
    pixmaps["BW1"] = QPixmap(":/player/data/player/player_BW1.png");
    pixmaps["BW2"] = QPixmap(":/player/data/player/player_BW2.png");

    pixmaps["F"] = QPixmap(":/player/data/player/player_F.png");
    pixmaps["FW1"] = QPixmap(":/player/data/player/player_FW1.png");
    pixmaps["FW2"] = QPixmap(":/player/data/player/player_FW2.png");

    pixmaps["L"] = QPixmap(":/player/data/player/player_L.png");
    pixmaps["LW1"] = QPixmap(":/player/data/player/player_LW1.png");
    pixmaps["LW2"] = QPixmap(":/player/data/player/player_LW2.png");

    pixmaps["R"] = QPixmap(":/player/data/player/player_R.png");
    pixmaps["RW1"] = QPixmap(":/player/data/player/player_RW1.png");
    pixmaps["RW2"] = QPixmap(":/player/data/player/player_RW2.png");
}

void Player::move(int dx, int dy, int mapWidth, int mapHeight) {
    x = std::max(0, std::min(x + dx, mapWidth - width));
    y = std::max(0, std::min(y + dy, mapHeight - height));
}

QRect Player::getRect() const {
    return QRect(x, y, width, height);
}

int Player::getX() const { return x; }
int Player::getY() const { return y; }

void Player::setDirection(Direction dir) {
    direction = dir;
}

void Player::setWalking(bool walking) {
    isWalking = walking;
}

void Player::updateWalkFrame() {
    walkFrame = 1 - walkFrame; // 在 0 與 1 間切換
}

QPixmap Player::getCurrentPixmap() const {
    QString key;
    switch (direction) {
        case UP:
            key = isWalking ? (walkFrame ? "BW1" : "BW2") : "B";
            break;
        case DOWN:
            key = isWalking ? (walkFrame ? "FW1" : "FW2") : "F";
            break;
        case LEFT:
            key = isWalking ? (walkFrame ? "LW1" : "LW2") : "L";
            break;
        case RIGHT:
            key = isWalking ? (walkFrame ? "RW1" : "RW2") : "R";
            break;
    }
    return pixmaps.value(key, QPixmap()); // fallback 為空 QPixmap
}
