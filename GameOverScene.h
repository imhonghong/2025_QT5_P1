#pragma once

#include <QWidget>
#include <QLabel>

class GameOverScene : public QWidget {
    Q_OBJECT

public:
    explicit GameOverScene(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};
