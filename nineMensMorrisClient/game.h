#ifndef GAME_H
#define GAME_H

class Game
{
public:
    Game();
    unsigned char table[24];
    int whiteMenToBePlaced;
    int blackMenToBePlaced;
    enum ManColor {Empty = 0, Black = 1, White = 2 };
    ManColor manColor;
    enum GameState {WhitePlaces = 1, BlackPlaces = 2, WhiteMoves = 3, BlackMoves = 4, WhiteRemoves = 5, BlackRemoves = 6,
                     Finished = 7, ReadyForNewGame = 8};
    GameState gameState;
    bool placeMan(int position);
    bool moveMan(int fromPos, int toPos);
    bool removeMan(int pos);
    ManColor winner();
private:
    bool closedMillOnPosition(int pos);
    int numberOfFreeMan(ManColor color);
    bool isNeighbour(int pos1, int pos2);
    int manCount(ManColor color);
    bool canMove(ManColor color);
    static constexpr const int MILLS[][3] = { { 0, 1, 2 }, {2, 3, 4}, {4, 5, 6}, {6, 7, 0},
                                                      {8, 9, 10}, {10, 11, 12}, {12, 13, 14}, {14, 15, 8},
                                                      {16, 17, 18}, {18, 19, 20}, {20, 21, 22}, {22, 23, 16},
                                                      {1, 9, 17}, {3, 11, 19}, {5, 13, 21}, {7, 15, 23} };
    static const int MILLS_COUNT = 16;
    static const int POSITION_COUNT = 24;
};

#endif // GAME_H
