#pragma once
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QVector>
#include "PokemonCollection.h"

class ChoosePokemonWidget : public QWidget {
    Q_OBJECT
public:
    ChoosePokemonWidget(PokemonCollection *collection, QWidget *parent = nullptr);

signals:
    void pokemonChosen();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    PokemonCollection *collection;
    QWidget *confirmDialog = nullptr;
    QVector<QFrame*> pokemonFrames;
    int selectedIndex = 0;

    QWidget *confirmWidget = nullptr;
    QVector<QFrame*> confirmFrames;
    int confirmSelectedIndex = 0;
    QString selectedPokemonName;

    void showConfirmDialog(const QString &name);
    void updateFrameFocus(QVector<QFrame*> &frames, int selected);
};
