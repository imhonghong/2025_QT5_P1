#pragma once

#include <QString>

class Pokemon {
public:
    Pokemon(const QString &name, const QString &iconPath)
        : name(name), iconPath(iconPath) {}

    QString getName() const { return name; }
    QString getIconPath() const { return iconPath; }

private:
    QString name;
    QString iconPath;
};
