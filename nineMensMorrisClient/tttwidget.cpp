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

QVector2D TTTWidget::screenPosition(int c, int i, int size)
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

    return QVector2D(xx,yy);
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
    float manSize = manRelSize * size;

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < 8; i++)
        {
            QVector2D pos = screenPosition(c, i, size);

            painter.setPen(blackPen);
            painter.setBrush(QBrush(Qt::black));
            painter.drawEllipse(pos.x() - emptyCircleSize / 2, pos.y() - emptyCircleSize / 2, emptyCircleSize, emptyCircleSize);

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

                painter.drawEllipse(pos.x() - manSize / 2, pos.y() - manSize / 2, manSize, manSize);
            }
        }
    }

    if (this->selectedMan > -1)
    {
        int c = this->selectedMan / 8;
        int i = this->selectedMan - 8 * c;
        QVector2D pos = screenPosition(c, i, size);

        painter.setPen(QPen(Qt::green, 2));
        painter.setBrush(QBrush(Qt::transparent));
        painter.drawRect(pos.x() - 1.1f * manSize / 2, pos.y() - 1.1f * manSize / 2, 1.1f * manSize, 1.1f * manSize);
    }
}

void TTTWidget::mousePressEvent(QMouseEvent* e)
{
    //if (!m_bCPlayer) return;

    int w = this->width();
    int h = this->height();

    int size = w < h ? w : h;
    float manSize = manRelSize * size;

    int x = e->x();
    int y = e->y();
    QVector2D clickPos = QVector2D(x, y);

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < 8; i++)
        {
            QVector2D pos = screenPosition(c, i, size);
            if ((pos - clickPos).length() < manSize)
            {
                selectedMan = 8 * c + i;
            }

        }
    }

    this->update();
    //emit signalStep(bx, by);
}
