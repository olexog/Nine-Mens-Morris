#include "game.h"

#include <QDebug>

Game::Game() {
    for (int i = 0; i < 24; i++)
    {
        this->table[i] = 0;
    }

    for (int i = 0; i < 8; i++)
    {
        this->table[i] = 1;
    }

    for (int i = 10; i < 18; i++)
    {
        this->table[i] = 2;
    }

    this->whiteMenToBePlaced = 1;
    this->blackMenToBePlaced = 1;
}

bool Game::placeMan(int position){
    // game state is not applicable for placing
    if (this->gameState != GameState::WhitePlaces && this->gameState != GameState::BlackPlaces)
    {
        qDebug() << "Game state is invalid for placing!" << this->gameState;
        return false;
    }

    if (position < 0 || (position > 23))
    {
        qDebug() << "Position is invalid!";
        return false;
    }

    // place is not empty
    if (this->table[position] != 0)
    {
        qDebug() << "Position is not empty to place!";
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
        return false;
    }

    if (this->table[toPos] != (int)ManColor::Empty)
    {
        qDebug() << "Destination position is not empty!";
        return false;
    }

    if (!isNeighbour(fromPos, toPos))
    {
        qDebug() << "Moving only to adjacent cells!";
        return false;
    }

    // moving

    this->table[toPos] = this->table[fromPos];
    this->table[fromPos] = (int)ManColor::Empty;

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
        return false;
    }

    if (closedMillOnPosition(pos) && numberOfFreeMan(opponentColor) != 0)
    {
        qDebug() << "Cannot remove piece from closed mill if there are free men.";
        return false;
    }

    this->table[pos] = (int)Empty;

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
        return abs(k1 - k2) == 1 || abs(k1 - k2) == 7;
    }
    else
    {
        return k1 == k2 && abs(c1 - c2) == 1;
    }
}
