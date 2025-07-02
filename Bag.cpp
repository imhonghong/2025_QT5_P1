#include "Bag.h"

Bag::Bag() {}

void Bag::addItem(const QString &itemName, int count) {
    items[itemName] += count;
}

int Bag::getItemCount(const QString &itemName) const {
    return items.value(itemName, 0);
}

QMap<QString, int> Bag::getAllItems() const {
    return items;
}
