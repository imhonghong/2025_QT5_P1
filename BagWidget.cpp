#include "BagWidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

BagWidget::BagWidget(Bag *bag, QWidget *parent)
    : QWidget(parent), bag(bag) {

    setFixedSize(400, 500);
    setWindowTitle("Bag");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 上半部分 Item 背景
    QLabel *itemBackground = new QLabel(this);
    itemBackground->setPixmap(QPixmap(":/other/data/item_bag_background.png"));

    QVBoxLayout *itemLayout = new QVBoxLayout(itemBackground);
    itemLayout->setAlignment(Qt::AlignTop);

    auto items = bag->getAllItems();
    if (items.isEmpty()) {
        QLabel *emptyLabel = new QLabel("No items in your bag.", this);
        emptyLabel->setStyleSheet("color: white; font-size: 16px;");
        itemLayout->addWidget(emptyLabel);
    } else {
        for (auto it = items.begin(); it != items.end(); ++it) {
            QHBoxLayout *itemLine = new QHBoxLayout();

            QLabel *iconLabel = new QLabel(this);
            if (it.key().contains("Poké Ball", Qt::CaseInsensitive)) {
                iconLabel->setPixmap(QPixmap(":/icon/data/icon/Pokeball_bag.png").scaled(32, 32));
            } else if (it.key().contains("Potion", Qt::CaseInsensitive)) {
                iconLabel->setPixmap(QPixmap(":/icon/data/icon/Potion_bag.png").scaled(32, 32));
            } else if (it.key().contains("Ether", Qt::CaseInsensitive)) {
                iconLabel->setPixmap(QPixmap(":/icon/data/icon/Ether_bag.png").scaled(32, 32));
            } else {
                iconLabel->setPixmap(QPixmap(":/item/data/item/default_icon.png").scaled(32, 32));
            }

            QLabel *textLabel = new QLabel(QString("%1 x%2").arg(it.key()).arg(it.value()), this);
            textLabel->setStyleSheet("color: white; font-size: 14px;");

            itemLine->addWidget(iconLabel);
            itemLine->addWidget(textLabel);
            itemLayout->addLayout(itemLine);
        }
    }

    mainLayout->addWidget(itemBackground);

    // 下半部分 Pokémon 背景（佔位）
    QLabel *pokemonBackground = new QLabel(this);
    pokemonBackground->setPixmap(QPixmap(":/other/data/bag.png"));
    mainLayout->addWidget(pokemonBackground);
}
