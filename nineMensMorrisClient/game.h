#ifndef GAME_H
#define GAME_H

class Game
{
public:
    Game();
    unsigned char table[24];
    enum ManColor {Black = 1, White = 2 };
    ManColor manColor;
    enum GameState {WhitePlaces = 1, BlackPlaces = 2, WhiteMoves = 3, BlackMoves = 4, WhiteRemoves = 5, BlackRemoves = 6, Finished = 7};
    GameState gameState;
    void placeMan(int position);
};

#endif // GAME_H
