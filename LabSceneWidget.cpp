#include "LabSceneWidget.h"
#include <QVBoxLayout>

LabSceneWidget::LabSceneWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("This is the Laboratory Scene.\n(Placeholder)", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 18px;");

    layout->addWidget(label);
}
