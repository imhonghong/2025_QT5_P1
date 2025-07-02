#include "DialogWidget.h"
#include <QKeyEvent>
#include <QMouseEvent>


DialogWidget::DialogWidget(const QStringList &textList, QWidget *parent, bool autoClose) : QWidget(parent), texts(textList) {
    setFixedSize(400, 150);
    setAttribute(Qt::WA_DeleteOnClose);

    setFocusPolicy(Qt::StrongFocus); // 可接收鍵盤事件
    setFocus();                      // 自動取得焦點
    if (autoClose) {
        QTimer::singleShot(400, this, SLOT(close()));
    }
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *background = new QLabel(this);
    background->setPixmap(QPixmap(":/other/data/dialog.png").scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setFixedSize(size());
    background->lower();

    textLabel = new QLabel(texts[0], this); // 顯示第一段
    textLabel->setStyleSheet("color: black; font-size: 16px; font-weight: bold;");
    textLabel->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addWidget(textLabel);
    layout->addStretch();

    setLayout(layout);

    move((parent->width() - width()) / 2, parent->height() - height() - 20);
    show();
}

void DialogWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_A) {
        currentIndex++;
        if (currentIndex < texts.size()) {
            textLabel->setText(texts[currentIndex]);
        } else {
            close();
        }
    }
}

void DialogWidget::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    currentIndex++;
    if (currentIndex < texts.size()) {
        textLabel->setText(texts[currentIndex]);
    } else {
        close();
    }
}
