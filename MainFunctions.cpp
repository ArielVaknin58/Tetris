#pragma once
#include "Board.h"
#include "MainFunctions.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>
using namespace std;


int PrintInitialMainMenu()
{
    int Choice = 0;
    cout << "Hello ! welcome to Tetris " << endl;
    cout << "(1) Start a new game " << endl;
    cout << "(8) Present instructions and keys" << endl;
    cout << "(9) EXIT" << endl;
    Choice = _getch();
    return (int)Choice;
}

int PrintInstructionsAndKeys()
{
    system("cls");
    cout << "                                    **********************************                                           " << endl;
    cout << "In Tetris, players complete lines by moving different tetrominoes which descend onto the playing field." << endl;
    cout << "The full lines disappear and grant the player points (10 per line), and the player can proceed to fill the free spaces." << endl;
    cout << "The game ends when the uncleared lines reach the top of the playing field.The longer the player can delay this outcome, the higher their score will be." << endl;
    cout << "In multiplayer mode, players must last longer than their opponents. \n(Unless the opponents lose at the same time, then the player with the most points will win)." << endl;
    cout << "                                    **********************************                                           " << endl;
    cout << "                                              -GAME KEYS-                                          " << endl;
    cout << "                                  Player 1                       Player 2                              " << endl;
    cout << "   LEFT :                          a or A                         j or J                                   " << endl;
    cout << "   RIGHT :                         d or D                         l or L                                 " << endl;
    cout << "   ROTATE CLOCKWISE :              s or S                         k or K                               " << endl;
    cout << "   ROTATE COUNTERCLOCKWISE :       w or W                         i or I                                             " << endl;
    cout << "   DROP :                          x or X                         m or M                                      " << endl;
    cout << "\n                                 ************************************                                           " << endl;
    cout << "**Press any key to continue**" << endl;
    char c;
    c = _getch();
    system("cls");


    return 0;
}

int PrintPausedMainMenu()
{
    Board board;
    int Choice = 0;
    cout << "*Game is paused* " << endl;
    cout << "(1) Start a new game " << endl;
    cout << "(2) Continue a paused game" << endl;
    cout << "(8) Present instructions and keys" << endl;
    cout << "(9) EXIT" << endl;
    cin >> Choice;
    if (Choice == 2)
        system("cls");

    
    return Choice;

}

bool askForColors() {
    char ColorOn;
    cout << "Would you like to play with or without colors? Press Y for Colors on or N for colors off" << endl;
    Sleep(100);
    ColorOn = _getch();
    system("cls");
    if (ColorOn == 'y' || ColorOn == 'Y')
        return true;
    else if (ColorOn == 'n' || ColorOn == 'N')
        return false;
    else {
        cout << "wrong color input, try again" << endl;
        return askForColors();
    }
 
}
void Gotoxy(int x, int y)
{
    HANDLE hConsoleOutput;
    COORD dwCursorPosition;
    cout.flush();
    dwCursorPosition.X = x;
    dwCursorPosition.Y = y;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void Swap(short& x, short& y)
{
    short temp = x;
    x = y;
    y = temp;
}

void Swap(int& x, int& y)
{
    int temp = x;
    x = y;
    y = temp;

}