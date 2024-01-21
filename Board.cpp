#include "Board.h"
#include <iostream>
#include "Windows.h"
#include "MainFunctions.h"
#include <iostream>
using namespace std;


Board::Board()//Board constructor
{
    for (int i = 0; i < GetboardHeight(); i++)
    {
        for (int j = 0; j < GetboardWidth(); j++)
        {
            board[i][j] = 0;
            if ((j == 0) || (j == GetboardWidth() - 1))//Place -1 as the board's bounds.
                board[i][j] = -1;
            if (i == GetboardHeight() - 1)
                board[i][j] = -1;
        }

    }
}


void Board::deleteLine(int rowToDelete)//Deletes a full line from the board and brings the rest down
{

    for (int j = 1; j < this->GetboardWidth() - 1; j++)//delete the 1's
        this->GetBoard()[rowToDelete][j] = 0;

    for (int i = rowToDelete; i >= 1 ; i--)//bring down each row 1 down 
    {
        for (int j = 1; j < this->GetboardWidth() -1; j++) 
            this->GetBoard()[i][j] = this->GetBoard()[i-1][j];
        
    }


}

int Board::PcheckRowsFull()//checks if there is a full row and retuns how many of them are in the board 
{
    int removed = 0;
    for (int i = 0; i < this->GetboardHeight() -1; i++) {
        bool full = true;
        for (int j = 1; j < this->GetboardWidth() -1; j++) {
            if (this->GetBoard()[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full == true) {
            this->deleteLine(i);
            removed++;
        }
    }
    return removed;
}

void Board::updateBoard(shapes* shape, COORD& edge)//Gets a shape and adds it to the board.
{

    for (int i = 0; i < 4; i++) {
        int x = shape->GetCoordArr()[i].X;
        int y = shape->GetCoordArr()[i].Y;
        if (edge.X >= 18)
            x -= 18;
        this->GetBoard()[y][x] = 1;
    }
}
int Board::GetboardHeight() { return HEIGHT + 2; }
int Board::GetboardWidth() { return WIDTH + 2; }
int(*Board::GetBoard())[WIDTH + 2] { return this->board; }
