#include "TitleSceneWidget.h"
#include <QPainter>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>

TitleSceneWidget::TitleSceneWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(525, 450); // 與專案一致
    setFocusPolicy(Qt::StrongFocus);
    QTimer::singleShot(0, this, [this]() {
        this->activateWindow();
        this->raise();
        this->setFocus(Qt::OtherFocusReason);
        qDebug() << "[TitleSceneWidget] has focus (delayed):" << this->hasFocus();
    });

    qDebug() << "TitleSceneWidget has focus:" << this->hasFocus();
    // 建立提示文字
    hintLabel = new QLabel("Press any key to start", this);
    hintLabel->setStyleSheet("font-size: 20px; color: white;;");
    hintLabel->setGeometry(15, 370, 300, 30);

    // 設置透明度效果
    opacityEffect = new QGraphicsOpacityEffect(this);
    hintLabel->setGraphicsEffect(opacityEffect);

    // 設置動畫
    fadeAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
    fadeAnimation->setDuration(1500); // 1.5 秒淡入淡出
    fadeAnimation->setStartValue(0.0);
    fadeAnimation->setEndValue(1.0);
    fadeAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    fadeAnimation->setLoopCount(-1); // 無限循環
    fadeAnimation->start();
}

void TitleSceneWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    QPixmap pixmap(":/scene/data/scene/start_menu.png");

    if (!pixmap.isNull()) {
        painter.drawPixmap(rect(), pixmap);
    } else {
        painter.fillRect(rect(), Qt::black);
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Image not found");
    }
}

void TitleSceneWidget::keyPressEvent(QKeyEvent *event) {

    Q_UNUSED(event);
    emit startGame();
}
