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

bool Bag::useItem(const QString &itemName) {
    if (items.contains(itemName) && items[itemName] > 0) {
        items[itemName]--;

        /*if (items[itemName] == 0) {
            items.remove(itemName); // 用完時從Bag中移除
        }
        */
        return true;
    }
    return false;
}
