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

    if (position < 0 || position > 23)
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

    if (this->manColor == ManColor::White)
    {
        this->whiteMenToBePlaced--;

        if (this->blackMenToBePlaced > 0)
        {
            this->gameState = GameState::BlackPlaces;
        }
        else if (this->blackMenToBePlaced == 0)
        {
            this->gameState = GameState::BlackMoves;
            qDebug() << "WARNING: possibly invalid move";
        }
        else
        {
            qDebug() << "blackMenToBePlaced is negative!";
        }
    }
    else
    {
        this->blackMenToBePlaced--;

        if (this->whiteMenToBePlaced > 0)
        {
            this->gameState = GameState::WhitePlaces;
        }
        else if (this->whiteMenToBePlaced == 0)
        {
            this->gameState = GameState::WhiteMoves;
        }
        else
        {
            qDebug() << "whiteMenToBePlaced is negative!";
        }
    }

    return true;
}
