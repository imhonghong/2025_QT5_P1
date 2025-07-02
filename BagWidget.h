#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "Bag.h"

class BagWidget : public QWidget {
    Q_OBJECT

public:
    explicit BagWidget(Bag *bag, QWidget *parent = nullptr);

private:
    Bag *bag;
    QVBoxLayout *layout;
    void updateDisplay();
};
