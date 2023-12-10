#include "game.h"

Game::Game() {
    this->gameState = GameState::WhitePlaces;
    for (int i = 0; i < 24; i++)
    {
        this->gameTable[i] = 0;
    }
    this->whiteMenToBePlaced = 9;
    this->blackMenToBePlaced = 9;
}
