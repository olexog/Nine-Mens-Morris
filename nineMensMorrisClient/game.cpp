#include "game.h"

Game::Game() {
    for (int i = 0; i < 24; i++)
    {
        this->table[i] = 0;
    }
}

void Game::placeMan(int position){
    if (position < 0 || position > 23)
        return;
    this->table[position] = this->manColor;
    this->gameState = this->manColor == ManColor::Black ? GameState::WhitePlaces : GameState::BlackPlaces;
}
