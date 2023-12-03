#include <QPainter>
#include <QMouseEvent>
#include "tttwidget.h"

TTTWidget::TTTWidget(QWidget*, const char*)
{
    Init();
}

void TTTWidget::Init()
{
    for (int i = 0; i < 9; ++i) {
        m_State[i] = 0;
    }

    for (int i = 0; i < 24; ++i) {
        gameTable[i] = 0;
    }

    gameTable[0] = 1;
    gameTable[4] = 2;
    gameTable[20] = 2;

    m_bCPlayer = false;
}

void TTTWidget::slotInit()
{
    Init();
    update();
}

void TTTWidget::slotState(bool cplayer, unsigned char* state)
{
    for (int i = 0; i < 9; ++i) {
        m_State[i] = state[i];
    }

    m_bCPlayer = cplayer;
    update();
}

void TTTWidget::paintEvent(QPaintEvent* e)
{
    int w = this->width();
    int h = this->height();
    int bw = w / 3;
    int bh = h / 3;

    int size = w < h ? w : h;
    float borderMargin = 1.0f/16.0f;
    float rectSpace = (1 - 2 * borderMargin) / 6.0f;


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen linepen(Qt::black, 2);
    painter.setPen(linepen);

    painter.drawRect(size * borderMargin, size * borderMargin, 6 * size * rectSpace, 6 * size * rectSpace);
    painter.drawRect(size * borderMargin + size * rectSpace, size * borderMargin + size * rectSpace,
                     4 * size * rectSpace, 4 * size * rectSpace);
    painter.drawRect(size * borderMargin + 2 * size * rectSpace, size * borderMargin + 2 * size * rectSpace,
                     2 * size * rectSpace, 2 * size * rectSpace);

    painter.drawLine(size * borderMargin, size / 2, size * borderMargin + 2 * size * rectSpace, size / 2);
    painter.drawLine(size - size * borderMargin, size / 2, size - size * borderMargin - 2 * size * rectSpace, size / 2);
    painter.drawLine(size / 2, size * borderMargin, size / 2, size * borderMargin + 2 * size * rectSpace);
    painter.drawLine(size / 2, size - size * borderMargin, size / 2, size - size * borderMargin - 2 * size * rectSpace);

    for (int i = 1; i < 3; ++i) {
    }

    QPen Xpen(Qt::blue, 2);
    QPen Open(Qt::red, 2);

    QPen blackPen(Qt::black, 2);

    QPen blackManPen(Qt::black, 2);
    QBrush blackManBrush(Qt::black);

    QPen whiteManPen(Qt::gray, 2);
    QBrush whiteManBrush(Qt::gray);

    float emptyCircleSize = 0.02f * size;
    float manSize = 0.08f * size;

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < 8; i++)
        {
            int xpos = 0;
            if (i == 0 || i == 6 || i == 7) xpos = -1;
            else if (i == 1 || i == 5) xpos = 0;
            else if (i == 2 || i == 3 || i == 4) xpos = 1;

            int ypos = 0;
            if (i == 0 || i == 1 || i == 2) ypos = -1;
            else if (i == 3 || i == 7) ypos = 0;
            else if (i == 4 || i == 5 || i == 6) ypos = 1;

            int xx = size / 2 + xpos * (3 - c) * rectSpace * size;
            int yy = size / 2 + ypos * (3 - c) * rectSpace * size;

            painter.setPen(blackPen);
            painter.setBrush(QBrush(Qt::black));
            painter.drawEllipse(xx - emptyCircleSize / 2, yy - emptyCircleSize / 2, emptyCircleSize, emptyCircleSize);

            int manIndex = 8 * c + i;
            if (gameTable[manIndex] == 1 || gameTable[manIndex] == 2)
            {
                if (gameTable[manIndex] == 1)
                {
                    painter.setPen(blackManPen);
                    painter.setBrush(blackManBrush);
                }
                else
                {
                    painter.setPen(whiteManPen);
                    painter.setBrush(whiteManBrush);
                }

                painter.drawEllipse(xx - manSize / 2, yy - manSize / 2, manSize, manSize);
            }
        }
    }

    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            int i = y*3 + x;
            if (m_State[i] == 1) {
                //X
                painter.setPen(Xpen);
                painter.drawLine(x*bw, y*bh,
                                 (x+1)*bw, (y+1)*bh);
                painter.drawLine(x*bw, (y+1)*bh,
                                 (x+1)*bw, y*bh);
            }
            if (m_State[i] == 2) {
                //O
                painter.setPen(Open);
                painter.drawEllipse(x*bw,y*bh,bw,bh);
            }
        }
    }
}

void TTTWidget::mousePressEvent(QMouseEvent* e)
{
    if (!m_bCPlayer) return;

    int bw = this->width() / 3;
    int bh = this->height() / 3;

    int x = e->x();
    int y = e->y();

    int bx = x / bw;
    int by = y / bh;

    int i = by * 3 + bx;
    if (m_State[i] == 0)
        emit signalStep(bx, by);
}
