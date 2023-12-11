#ifndef GAME_H
#define GAME_H

class Game
{
public:
    Game();
    unsigned char gameTable[24];
    int whiteMenToBePlaced;
    int blackMenToBePlaced;
    enum GameState {WhitePlaces = 1, BlackPlaces = 2, WhiteMoves = 3, BlackMoves = 4, WhiteRemoves = 5, BlackRemoves = 6,
                     Finished = 7, ReadyForNewGame = 8};
    enum ManColor {Empty = 0, Black = 1, White = 2 };
    GameState gameState;
};

#endif // GAME_H
