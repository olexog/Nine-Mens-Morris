#ifndef GAME_H
#define GAME_H

class Game
{
public:
    Game();
    unsigned char gameTable[24];
    enum GameState {WhitePlaces = 1, BlackPlaces = 2, WhiteMoves = 3, BlackMoves = 4, WhiteRemoves = 5, BlackRemoves = 6, Finished = 7};
    GameState gameState;
};

#endif // GAME_H
