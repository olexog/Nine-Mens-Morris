#ifndef TTTWIDGET_H
#define TTTWIDGET_H

#include <QWidget>
#include <QVector2D>
#include "game.h"

class TTTWidget : public QWidget
{
    Q_OBJECT

public:
    TTTWidget(QWidget* parent = 0, const char* name = 0);

signals:
    void signalStep(Game newSituation);

public slots:
    void slotInit();
    void slotState(int state, int manColor, unsigned char* table);

protected:
    Game game;
    unsigned char m_State[9];
    unsigned char gameTable[24];
    char selectedMan = -1;
    bool m_bCPlayer;
    float borderMargin = 1.0f/16.0f;
    float rectSpace = (1 - 2 * borderMargin) / 6.0f;
    float manRelSize = 0.08f; // related to size of game table

protected:
    void Init();
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
    QVector2D screenPosition(int c, int i, int size);

};

#endif // TTTWIDGET_H
