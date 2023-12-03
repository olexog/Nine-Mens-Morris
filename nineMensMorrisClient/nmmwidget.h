#ifndef NMMWIDGET_H
#define NMMWIDGET_H

#include <QWidget>

class NMMWidget : public QWidget
{
    Q_OBJECT

public:
    NMMWidget(QWidget* parent = 0, const char* name = 0);

signals:
    void signalStep(int x, int y);

public slots:
    void slotInit();
    void slotState(bool cplayer, unsigned char* state);

protected:
    unsigned char m_State[9];
    unsigned char gameTable[24];
    bool m_bCPlayer;

protected:
    void Init();
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
};

#endif // NMMWIDGET_H
