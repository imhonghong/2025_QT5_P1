#pragma once

#include <QWidget>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class TitleSceneWidget : public QWidget {
    Q_OBJECT

public:
    explicit TitleSceneWidget(QWidget *parent = nullptr);

signals:
    void startGame();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLabel *hintLabel;
    QGraphicsOpacityEffect *opacityEffect;
    QPropertyAnimation *fadeAnimation;
};
