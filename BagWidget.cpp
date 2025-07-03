#include "BagWidget.h"
#include <QPixmap>
#include <QLabel>

BagWidget::BagWidget(Bag *bag, PokemonCollection *pokemonCollection, QWidget *parent)
    : QWidget(parent), bag(bag), pokemonCollection(pokemonCollection) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setupDisplay();
}

void BagWidget::setupDisplay() {
    // 顯示上半背景
    backgroundItem = new QLabel(this);
    backgroundItem->setPixmap(QPixmap(":/other/data/item_bag_background.png"));
    backgroundItem->setGeometry(0, 0, 150, 40);

    // 顯示下半背景
    backgroundPokemon = new QLabel(this);
    backgroundPokemon->setPixmap(QPixmap(":/other/data/bag.png"));
    backgroundPokemon->setGeometry(0, 40, 150, 150);

    // ======== 上半：顯示道具 ========
    struct ItemDisplay {
        QString name;
        QString iconPath;
        QPoint position;
    };

    QVector<ItemDisplay> itemsToShow = {
        {"Poké Ball", ":/icon/data/icon/Pokeball_bag.png", QPoint(10, 10)},
        {"Potion",    ":/icon/data/icon/Potion_bag.png",   QPoint(60, 10)},
        {"Ether",     ":/icon/data/icon/Ether_bag.png",    QPoint(105, 11)}
    };

    for (const auto& item : itemsToShow) {
        QLabel *iconLabel = new QLabel(this);
        iconLabel->setPixmap(QPixmap(item.iconPath).scaled(20, 20));
        iconLabel->move(item.position);

        QLabel *countLabel = new QLabel(this);
        countLabel->setStyleSheet("color: black; font-weight: bold; font-size: 14px;");
        countLabel->setFixedSize(40, 20);
        countLabel->move(item.position + QPoint(20, 0)); // 微調位置

        int count = bag->getItemCount(item.name);
        countLabel->setText(QString("x%1").arg(count));

        itemIcons.append(iconLabel);
        itemCounts.append(countLabel);
    }

    // ======== 下半：顯示擁有 Pokémon ========
    QVector<Pokemon*> pokemons = pokemonCollection->getAllPokemons();
    int startY = 45;
    int spacingY = 36;
    int iconOffsetX = 115;

    for (int i = 0; i < pokemons.size(); ++i) {
        Pokemon *pokemon = pokemons[i];

        QLabel *nameLabel = new QLabel(this);
        nameLabel->setText(pokemon->getName());
        nameLabel->setStyleSheet("color: black; font-weight: bold; font-size: 16px;");
        nameLabel->move(15, startY + i * spacingY);
        nameLabel->setFixedSize(150, 32);

        QLabel *iconLabel = new QLabel(this);
        iconLabel->setPixmap(QPixmap(pokemon->getImagePath(true)).scaled(32, 32));
        iconLabel->move(iconOffsetX, startY + i * spacingY);

        pokemonLabels.append(nameLabel);
        pokemonIcons.append(iconLabel);
    }
}
