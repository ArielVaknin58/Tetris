#pragma once
#include "shapes.h"

class shapes;
class Board
{
private:

    static constexpr int HEIGHT = 18;
    static constexpr int WIDTH = 12;

    int board[HEIGHT + 2][WIDTH + 2];

public:

    Board();
    int(*GetBoard())[WIDTH + 2];
    int GetboardHeight();
    int GetboardWidth();
    int PcheckRowsFull();
    void updateBoard(shapes* shape, COORD& edge);
    void deleteLine(int rowToDelete);
};