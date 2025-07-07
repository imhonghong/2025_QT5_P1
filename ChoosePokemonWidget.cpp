#include "ChoosePokemonWidget.h"
#include "Pokemon.h"
#include "DialogWidget.h"
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QPixmap>
#include <QDebug>

ChoosePokemonWidget::ChoosePokemonWidget(PokemonCollection *collection, QWidget *parent)
    : QWidget(parent), collection(collection) {
    setFixedSize(300, 250);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);

    QLabel *background = new QLabel(this);
    background->setPixmap(QPixmap(":/other/data/item_bag_background.png").scaled(size()));
    background->setFixedSize(size());
    background->move(0,0);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
    this->grabKeyboard();

    struct PokemonData {
        QString name;
        QString imagePath;
        QPoint pos;
    };

    QVector<PokemonData> pokemons = {
        {"Charmander", ":/battle/data/battle/charmander.png", QPoint(30, 60)},
        {"Squirtle",   ":/battle/data/battle/squirtle.png",   QPoint(120, 60)},
        {"Bulbasaur",  ":/battle/data/battle/bulbasaur.png",  QPoint(210, 60)}
    };

    for (int i = 0; i < pokemons.size(); ++i) {
        QFrame *frame = new QFrame(this);
        frame->setFrameShape(QFrame::Box);
        frame->setLineWidth(2);
        frame->setStyleSheet("border: 2px solid white;");
        frame->setGeometry(pokemons[i].pos.x(), pokemons[i].pos.y(), 60, 80);

        QLabel *icon = new QLabel(frame);
        icon->setPixmap(QPixmap(pokemons[i].imagePath).scaled(60, 60));
        icon->setAlignment(Qt::AlignCenter);
        icon->setGeometry(0, 0, 60, 60);

        QLabel *label = new QLabel(pokemons[i].name, frame);
        label->setObjectName("nameLabel"); // 🩹 加上這行
        label->setAlignment(Qt::AlignCenter);
        label->setGeometry(0, 60, 60, 20);
        label->setStyleSheet("font-size: 12px; font-weight: bold; color: black;");

        pokemonFrames.append(frame);
        frame->installEventFilter(this);
    }

    updateFrameFocus(pokemonFrames, selectedIndex);
}


void ChoosePokemonWidget::closeEvent(QCloseEvent *event) {
    this->releaseKeyboard();
    QWidget::closeEvent(event);
}

void ChoosePokemonWidget::updateFrameFocus(QVector<QFrame*> &frames, int selected) {
    for (int i = 0; i < frames.size(); ++i) {
        frames[i]->setStyleSheet(i == selected ? "border: 2px solid blue;" : "border: 2px solid white;");
    }
}

void ChoosePokemonWidget::keyPressEvent(QKeyEvent *event) {
    if (confirmWidget && confirmWidget->isVisible()) {
        if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
            confirmSelectedIndex = 1 - confirmSelectedIndex;
            updateFrameFocus(confirmFrames, confirmSelectedIndex);
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            if (confirmSelectedIndex == 0) { // Yes
                Pokemon *p = Pokemon::createPokemon(selectedPokemonName, 1, false);
                Q_ASSERT_X(p != nullptr, "ChoosePokemonWidget", "Pokemon::createPokemon returned nullptr");
                collection->addPokemon(p);
                DialogWidget *dw = new DialogWidget(QStringList({QString("%1 is now in your bag!").arg(selectedPokemonName)}), parentWidget(), true);
                QTimer::singleShot(1000, dw, &QWidget::close);
                emit pokemonChosen();
                close();
            }
            confirmWidget->close();
            confirmWidget = nullptr;
        } else if (event->key() == Qt::Key_Escape) {
            confirmWidget->close();
            confirmWidget = nullptr;
        }
        return;
    }

    if (event->key() == Qt::Key_Left && selectedIndex > 0) {
        selectedIndex--;
    } else if (event->key() == Qt::Key_Right && selectedIndex < pokemonFrames.size() - 1) {
        selectedIndex++;
    } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QLabel *label = pokemonFrames[selectedIndex]->findChild<QLabel *>("nameLabel", Qt::FindDirectChildrenOnly);
        if (label) {
            showConfirmDialog(label->text());
        }
    } else if (event->key() == Qt::Key_Escape) {
        close();
    }

    updateFrameFocus(pokemonFrames, selectedIndex);
}

bool ChoosePokemonWidget::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        for (int i = 0; i < pokemonFrames.size(); ++i) {
            if (watched == pokemonFrames[i]) {
                selectedIndex = i;
                updateFrameFocus(pokemonFrames, selectedIndex);
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void ChoosePokemonWidget::showConfirmDialog(const QString &name) {

    selectedPokemonName = name;
    qDebug() << "[showConfirmDialog] selectedPokemonName set to:" << selectedPokemonName;

    if (confirmWidget) {
        confirmWidget->close();
        delete confirmWidget;
    }

    confirmWidget = new QWidget(this);
    confirmWidget->setFixedSize(250, 100);
    confirmWidget->move(25, 75);
    confirmWidget->setStyleSheet("background-color: white; border: 2px solid black;");

    QLabel *label = new QLabel(QString("Choose %1 as your partner?").arg(name), confirmWidget);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(10, 5, 230, 40);

    confirmFrames.clear();

    QFrame *yesFrame = new QFrame(confirmWidget);
    yesFrame->setGeometry(40, 60, 60, 30);
    yesFrame->setFrameShape(QFrame::Box);
    yesFrame->setLineWidth(2);
    yesFrame->setStyleSheet("border: 2px solid blue;");

    QLabel *yesLabel = new QLabel("Yes", yesFrame);
    yesLabel->setAlignment(Qt::AlignCenter);
    yesLabel->setGeometry(0, 0, 60, 30);
    yesLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: black;");

    QFrame *noFrame = new QFrame(confirmWidget);
    noFrame->setGeometry(150, 60, 60, 30);
    noFrame->setFrameShape(QFrame::Box);
    noFrame->setLineWidth(2);
    noFrame->setStyleSheet("border: 2px solid white;");

    QLabel *noLabel = new QLabel("No", noFrame);
    noLabel->setAlignment(Qt::AlignCenter);
    noLabel->setGeometry(0, 0, 60, 30);
    noLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: black;");

    confirmFrames.append(yesFrame);
    confirmFrames.append(noFrame);

    yesFrame->installEventFilter(this);
    noFrame->installEventFilter(this);

    confirmSelectedIndex = 0;
    confirmWidget->show();
}
