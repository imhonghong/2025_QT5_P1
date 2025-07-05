#pragma once

#include <QString>
#include <QMap>

class Bag {
public:
    Bag();

    void addItem(const QString &itemName, int count);
    int getItemCount(const QString &itemName) const;
    QMap<QString, int> getAllItems() const;
    bool useItem(const QString &itemName);

private:
    QMap<QString, int> items;
};
