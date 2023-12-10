#include "game.h"

Game::Game() {
    this->gameState = GameState::WhitePlaces;
    for (int i = 0; i < 24; i++)
    {
        this->gameTable[i] = 0;
    }
}
