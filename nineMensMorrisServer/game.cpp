#include "game.h"

Game::Game() {
    this->gameState = GameState::WhitePlaces;
    for (int i = 0; i < 24; i++)
    {
        this->gameTable[i] = 0;
    }
    this->whiteMenToBePlaced = 9;
    this->blackMenToBePlaced = 9;

    for (int i = 0; i < 8; i++)
    {
        this->gameTable[i] = 1;
    }

    for (int i = 10; i < 18; i++)
    {
        this->gameTable[i] = 2;
    }

    this->whiteMenToBePlaced = 1;
    this->blackMenToBePlaced = 1;
}
