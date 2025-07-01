#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStringList>

class DialogWidget : public QWidget {
    Q_OBJECT

public:
    explicit DialogWidget(const QStringList &textList, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QLabel *textLabel;
    QStringList texts;
    int currentIndex = 0;
};
