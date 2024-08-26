#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include "tttwidget.h"
#include "game.h"

TTTWidget::TTTWidget(QWidget*, const char*)
{
    Init();
    updateLabels();
}

void TTTWidget::Init()
{

}

void TTTWidget::slotInit()
{
    Init();
    update();
}

void TTTWidget::slotStateReceived(Game receivedSituation)
{
    this->game = receivedSituation;
    update();
    checkForWin();
}

void TTTWidget::updateLabels()
{
    QString colorInfo = game.getManColor() == Game::White ? "WHITE" : "BLACK";

    QString directions = "";

    if (game.getState() == Game::WhitePlaces || game.getState() == Game::BlackPlaces)
    {
        if ((game.getState() == Game::WhitePlaces && game.getManColor() == Game::White) ||
            (game.getState() == Game::BlackPlaces && game.getManColor() == Game::Black))
        {
            directions = "Place a piece on an empty slot!";
        }
        else
        {
            directions = "Waiting for other player to place a piece...";
        }
    }
    else if (game.getState() == Game::WhiteMoves || game.getState() == Game::BlackMoves)
    {
        if ((game.getState() == Game::WhiteMoves && game.getManColor() == Game::White) ||
            (game.getState() == Game::BlackMoves && game.getManColor() == Game::Black))
        {
            if (game.canJump())
                directions = "Move a piece to any emtpy slot. You are free to jump now.";
            else
                directions = "Move a piece to any adjacent slot.";
        }
        else
        {
            directions = "Waiting for other player to move a piece...";
        }
    }
    else if (game.getState() == Game::WhiteRemoves || game.getState() == Game::BlackRemoves)
    {
        if ((game.getState() == Game::WhiteRemoves && game.getManColor() == Game::White) ||
            (game.getState() == Game::BlackRemoves && game.getManColor() == Game::Black))
        {
            directions = "Remove a piece from the opponent's pieces! Free men first.";
        }
        else
        {
            directions = "Waiting for other player to remove a piece...";
        }
    }

    emit signalUpdateLabels(colorInfo, directions, game.getMessage());
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
            if (game.getColorAt(manIndex) != Game::Empty)
            {
                if (game.getColorAt(manIndex) == Game::Black)
                {
                    painter.setPen(blackManPen);
                    painter.setBrush(blackManBrush);
                }
                else if (game.getColorAt(manIndex) == Game::White)
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
    int w = this->width();
    int h = this->height();

    int size = w < h ? w : h;
    float manSize = manRelSize * size;

    int x = e->position().x();
    int y = e->position().y();
    QVector2D clickPos = QVector2D(x, y);

    int selection = -1;
    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < 8; i++)
        {
            QVector2D pos = screenPosition(c, i, size);
            if ((pos - clickPos).length() < manSize)
            {
                selection = 8 * c + i;
            }
        }
    }

    if (selection > -1)
    {
        if ((this->game.getState() == Game::GameState::WhitePlaces && this->game.getManColor() == Game::ManColor::White) ||
            (this->game.getState() == Game::GameState::BlackPlaces && this->game.getManColor() == Game::ManColor::Black))
        {
            if (this->game.placeMan(selection))
            {
                gameStateChanged();
            }
        }

        if ((this->game.getState() == Game::WhiteRemoves && this->game.getManColor() == Game::White) ||
            (this->game.getState() == Game::BlackRemoves && this->game.getManColor() == Game::Black))
        {
            if (this->game.removeMan(selection))
            {
                gameStateChanged();
            }
        }

        if ((this->game.getState() == Game::GameState::WhiteMoves && this->game.getManColor() == Game::ManColor::White) ||
            (this->game.getState() == Game::GameState::BlackMoves && this->game.getManColor() == Game::ManColor::Black))
        {
            // destination is selected
            if (this->selectedMan > -1)
            {
                // clear selection
                if (this->selectedMan == selection)
                {
                    this->selectedMan = -1;
                }
                // move man
                else if (this->game.moveMan(this->selectedMan, selection))
                {
                    gameStateChanged();
                    this->selectedMan = -1;
                }
            }
            // select source position
            else
            {
                if (game.getColorAt(selection) == (int)this->game.getManColor())
                    this->selectedMan = selection;
            }

        }
    }

    this->update();
    updateLabels();
}

void TTTWidget::checkForWin()
{
    if (this->game.getState() == Game::Finished)
    {
        QMessageBox msgBox;
        if (this->game.winner() == Game::White)
            msgBox.setText("Winner is: WHITE");
        else if (this->game.winner() == Game::Black)
            msgBox.setText("Winner is: BLACK");
        else
            msgBox.setText("no winner");
        msgBox.exec();
    }
}

void TTTWidget::gameStateChanged()
{
    emit signalSendNewState(this->game);
    updateLabels();
}
