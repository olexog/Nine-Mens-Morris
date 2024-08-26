#include "game.h"

#include <QDebug>

Game::Game() {
    table = QList<ManColor>(24);

    for (int i = 0; i < 24; i++)
    {
        this->table[i] = Empty;
    }

    this->whiteMenToBePlaced = 9;
    this->blackMenToBePlaced = 9;
}

Game::Game(QList<ManColor> _table, int _whiteMenToBePlaced, int _blackMenToBePlaced, ManColor _manColor, GameState _gameState)
{
    this->table=_table;
    this->whiteMenToBePlaced = _whiteMenToBePlaced;
    this->blackMenToBePlaced = _blackMenToBePlaced;
    this->manColor = _manColor;
    this->gameState = _gameState;
}

QString Game::getMessage()
{
    return this->message;
}

bool Game::placeMan(int position){
    // game state is not applicable for placing
    if (this->gameState != GameState::WhitePlaces && this->gameState != GameState::BlackPlaces)
    {
        qDebug() << "Game state is invalid for placing!" << this->gameState;
        message = "Game state is invalid for placing!";
        return false;
    }

    if (position < 0 || (position > 23))
    {
        qDebug() << "Position is invalid!";
        message = "Position is invalid!";
        return false;
    }

    // place is not empty
    if (this->table[position] != 0)
    {
        qDebug() << "Position is not empty to place!";
        message = "Position is not empty to place!";
        return false;
    }

    // place
    this->table[position] = this->manColor;

    // succesful placing

    if (this->manColor == White)
    {
        this->whiteMenToBePlaced--;

        if (closedMillOnPosition(position))
        {
            this->gameState = WhiteRemoves;
        }
        else
        {
            if (this->blackMenToBePlaced > 0)
            {
                this->gameState = BlackPlaces;
            }
            else if (this->blackMenToBePlaced == 0)
            {
                this->gameState = BlackMoves;
                qDebug() << "WARNING: possibly invalid move";
            }
            else
            {
                qDebug() << "blackMenToBePlaced is negative!";
            }
        }
    }
    else
    {
        this->blackMenToBePlaced--;

        if (closedMillOnPosition(position))
        {
            this->gameState = BlackRemoves;
        }
        else
        {
            if (this->whiteMenToBePlaced > 0)
            {
                this->gameState = WhitePlaces;
            }
            else if (this->whiteMenToBePlaced == 0)
            {
                this->gameState = WhiteMoves;
                qDebug() << "WARNING: possibly invalid move";
            }
            else
            {
                qDebug() << "whiteMenToBePlaced is negative!";
            }
        }
    }

    // game is won
    if (this->winner() != Empty)
        this->gameState = Finished;

    return true;
}

bool Game::moveMan(int fromPos, int toPos)
{
    if (this->gameState != GameState::WhiteMoves && this->gameState != GameState::BlackMoves)
    {
        qDebug() << "Game state is invalid for moving!" << this->gameState;
        return false;
    }

    if (fromPos < 0 || (fromPos > 23))
    {
        qDebug() << "Source position is invalid!";
        return false;
    }

    if (toPos < 0 || (toPos > 23))
    {
        qDebug() << "Destination position is invalid!";
        return false;
    }

    if (this->table[fromPos] != (int)this->manColor)
    {
        qDebug() << "Wrong man color selected!";
        message = "Wrong man color selected!";
        return false;
    }

    if (this->table[toPos] != (int)ManColor::Empty)
    {
        qDebug() << "Destination position is not empty!";
        message = "Destination position is not empty!";
        return false;
    }

    if (manCount(this->manColor) > 3 && !isNeighbour(fromPos, toPos))
    {
        qDebug() << "Moving only to adjacent cells!";
        message = "Moving only to adjacent cells!";
        return false;
    }

    // moving

    this->table[toPos] = this->table[fromPos];
    this->table[fromPos] = ManColor::Empty;

    // game state change
    if (this->gameState == GameState::WhiteMoves)
    {
        if (closedMillOnPosition(toPos))
        {
            this->gameState = GameState::WhiteRemoves;
        }
        else
        {
            this->gameState = GameState::BlackMoves;
        }
    }
    else
    {
        if (closedMillOnPosition(toPos))
        {
            this->gameState = GameState::BlackRemoves;
        }
        else
        {
            this->gameState = GameState::WhiteMoves;
        }
    }

    // game is won
    if (this->winner() != Empty)
        this->gameState = Finished;

    return true;
}

bool Game::removeMan(int pos)
{
    if (this->gameState != WhiteRemoves && this->gameState != BlackRemoves)
    {
        qDebug() << "Game state is invalid for removing!" << this->gameState;
        return false;
    }

    if (pos < 0 || (pos > 23))
    {
        qDebug() << "Position is invalid!";
        return false;
    }

    ManColor opponentColor = this->manColor == White ? Black : White;
    if (this->table[pos] != (int)opponentColor)
    {
        qDebug() << "You have to remove from the opponent's pieces";
        message = "You have to remove from the opponent's pieces!";
        return false;
    }

    if (closedMillOnPosition(pos) && numberOfFreeMan(opponentColor) != 0)
    {
        qDebug() << "Cannot remove piece from closed mill if there are free men.";
        message = "Cannot remove piece from closed mill if there are free men.";
        return false;
    }

    this->table[pos] = Empty;

    if (this->gameState == WhiteRemoves)
    {
        if (this->blackMenToBePlaced > 0)
            this->gameState = BlackPlaces;
        else
            this->gameState = BlackMoves;
    }
    else if (this->gameState == BlackRemoves)
    {
        if (this->whiteMenToBePlaced > 0)
            this->gameState = WhitePlaces;
        else
            this->gameState = WhiteMoves;
    }

    // game is won
    if (this->winner() != Empty)
        this->gameState = Finished;

    return true;
}

bool Game::closedMillOnPosition(int pos)
{
    for (int i = 0; i < MILLS_COUNT; i++)
    {
        // position is on mills[i]
        if ((MILLS[i][0] == pos) || (MILLS[i][1] == pos) || (MILLS[i][2] == pos))
        {
            // all 3 men on mills is the same color and not empty
            if ((this->table[MILLS[i][0]] != ManColor::Empty) &&
                (this->table[MILLS[i][0]] == this->table[MILLS[i][1]]) && (this->table[MILLS[i][0]] == this->table[MILLS[i][2]]))
                return true;
        }
    }

    return false;
}

int Game::numberOfFreeMan(ManColor color)
{
    int count = 0;
    for (int i = 0; i < POSITION_COUNT; i++)
    {
        if (this->table[i] == (int)color && !closedMillOnPosition(i))
            count++;
    }

    return count;
}

bool Game::isNeighbour(int pos1, int pos2)
{
    int k1 = pos1 % 8;
    int k2 = pos2 % 8;
    int c1 = (pos1 - k1) / 8;
    int c2 = (pos2 - k2) / 8;

    // same sqare
    if (c1 == c2)
    {
        // mills along squares
        return abs(k1 - k2) == 1 || abs(k1 - k2) == 7;
    }
    else
    {
        // square connecting mills
        return (k1 % 2 == 1) && k1 == k2 && abs(c1 - c2) == 1;
    }
}

int Game::manCount(ManColor color)
{
    int count = 0;
    for (int i = 0; i < POSITION_COUNT; i++)
    {
        if (this->table[i] == (int)color)
            count++;
    }

    return count;
}

bool Game::canMove(ManColor color)
{
    if (this->manCount(color) < 3)
    {
        qDebug() << "Game has already finished.";
        return false;
    }

    // if jumping, always can move (24-9-3>0))
    if (this->manCount(color) == 3)
        return true;

    for (int i = 0; i < POSITION_COUNT; i++)
    {
        for (int j = 0; j < POSITION_COUNT; j++)
        {
            if (i == j)
                continue;

            if (isNeighbour(i, j))
            {
                if (this->table[i] == (int)color && this->table[j] == (int)Empty)
                    return true;
            }
        }
    }

    return false;
}

Game::ManColor Game::winner()
{
    // game cannot be won in placing state
    if (this->whiteMenToBePlaced > 0 || this->blackMenToBePlaced > 0)
        return Empty;

    // less than 3 men
    if (this->manCount(White) < 3)
        return Black;
    else if (this->manCount(Black) < 3)
        return White;

    // mill closed on 3 men
    if (this->manCount(White) == 3 && (this->gameState == BlackRemoves || this->gameState == Finished))
        return Black;
    else if (this->manCount(Black) == 3 && (this->gameState == WhiteRemoves || this->gameState == Finished))
        return White;

    // cannot move
    if ((this->gameState == WhiteMoves || this->gameState == Finished) && !this->canMove(White))
        return Black;
    else if ((this->gameState == BlackMoves || this->gameState == Finished) && !this->canMove(Black))
        return White;

    return Empty;
}

bool Game::canJump()
{
    return manCount(manColor) == 3;
}

Game::GameState Game::getState()
{
    return this->gameState;
}

void Game::setState(GameState newGameState)
{
    this->gameState = newGameState;
}

Game::ManColor Game::getManColor()
{
    return this->manColor;
}

Game::ManColor Game::getColorAt(int i)
{
    return (Game::ManColor)this->table[i];
}

int Game::getWhiteMenToBePlaced()
{
    return this->whiteMenToBePlaced;
}

int Game::getBlackMenToBePlaced()
{
    return this->blackMenToBePlaced;
}
