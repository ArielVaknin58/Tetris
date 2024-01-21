#pragma once
#include "Game.h"
#include <conio.h>
#include <iostream>
#include "Player.h"
#include "MainFunctions.h"
#include "Board.h"
#include <Windows.h>
using namespace std;

void Game::MainGame()//Initial interaction with the game, offers options.
{
	int Choice = 0;
	Game Tetris;
	bool colors;
	do
	{
		Choice = PrintInitialMainMenu();
		if (Choice == 49)//User chose 1 to play the game
		{
			system("cls");
			colors = askForColors();
			Choice = Tetris.go(colors);
			while (Choice == 1)
				Choice = Tetris.go(colors);
			if (Choice == 0)
				return;
		}
		else if (Choice == 56)//User chose 8 to get instructions
		{
			Choice = PrintInstructionsAndKeys();
			system("cls");
		}
		else if (Choice != 57)//Input check
		{
			std::cout << "Wrong input,please try again" << endl;
			Sleep(500);
			Choice = (int)_getch();
			system("cls");
		}

	} while (Choice != 57);

	cout << "\n\n";
	if (Choice == 57)//User chose to Exit
		return;

	Gotoxy(0, Tetris.GetPlayer1().GetFrame_Height() + 4);

}

Game::Game()
{
	this->p1 = Player(0);
	this->p2 = Player(1);
}
int chooseShapeColor() 
{

	int color = rand() % 3;
	switch (color)
	{
	case 0:
		return FOREGROUND_BLUE | BACKGROUND_BLUE;
	case 1:
		return FOREGROUND_RED | BACKGROUND_RED;
	case 2:
		return FOREGROUND_GREEN | BACKGROUND_GREEN;
	default:
		break;
	}
}

bool checkIfBoardFull(Board board)//The function checks if the shapes have reached the top line
{
	for (int i = 1; i < board.GetboardWidth(); i++) {
		if (board.GetBoard()[1][i] == 1)
			return true;
	}
	return false;
}
void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;  // Set the cursor size to a very large value (effectively hiding it)
	info.bVisible = FALSE;  // Set the cursor visibility to false
	SetConsoleCursorInfo(consoleHandle, &info);
}
int Game::go(bool color)//The actual game function 
{

	srand(time(NULL));

	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	int shapeColor2, shapeColor1;
	shapeColor1 = shapeColor2 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

	//Variables initialization
	COORD point1 = this->GetPlayer1().GetEdge();
	COORD point2 = this->GetPlayer2().GetEdge();
	Board board1, board2;
	short WhileBreaker = 0; //which brick broke the loop, to know which to initialize
	shapes* Brick1 = nullptr;
	shapes* Brick2 = nullptr;
	char c;
	short refreshFlag1 = 0;
	short refreshFlag2 = 0;

	while (true)
	{
		if((refreshFlag1 != 0)|| ( refreshFlag2 != 0 ))//If a line has been filled, to refresh the screen.
			system("cls");

		PrintGameInFrame(&board1, &board2, color);
		if (WhileBreaker == 0)
		{
			if (color) 
			{
				shapeColor1 = chooseShapeColor();
				shapeColor2 = chooseShapeColor();
			}
			Brick1 = GenerateShape(this->GetPlayer1().GetEdge());
			Brick2 = GenerateShape(this->GetPlayer2().GetEdge());
			point1 = this->GetPlayer1().GetEdge();
			point2 = this->GetPlayer2().GetEdge();
		}
		else if (WhileBreaker == 1)
		{
			if (color) {
				shapeColor1 = chooseShapeColor();
			}
			Brick1 = GenerateShape(this->GetPlayer1().GetEdge());
			point1 = this->GetPlayer1().GetEdge();
		}
		else if (WhileBreaker == 2)
		{
			if (color) {
				shapeColor2 = chooseShapeColor();
			}
			Brick2 = GenerateShape(this->GetPlayer2().GetEdge());
			point2 = this->GetPlayer2().GetEdge();
		}


		while (true)//A loop to draw shapes' movements.
		{

			Gotoxy(point1.X, point1.Y);
			SetConsoleTextAttribute(console_color, shapeColor1);
			DrawShape(Brick1);
			Gotoxy(point2.X, point2.Y);
			SetConsoleTextAttribute(console_color, shapeColor2);
			DrawShape(Brick2);
			Sleep(200);


			Gotoxy(point1.X, point1.Y);
			DeleteShape(Brick1);
			Gotoxy(point2.X, point2.Y);
			DeleteShape(Brick2);
			Sleep(200);
			hideCursor();
			if ((Brick1->LowestPoint().Y + 1 < this->GetPlayer1().GetFrame_Height() + 1) && (Brick1->isFreeToDescend(&board1)))
				Brick1->MoveYAxis(1);

			else//The square has reached the bottom line
			{
				board1.updateBoard(Brick1, this->GetPlayer1().GetEdge());
				WhileBreaker = 1;
				delete Brick1;
				break;
			}

			if ((Brick2->LowestPoint().Y + 1 < this->GetPlayer2().GetFrame_Height() + 1) && ((Brick2->isFreeToDescend(&board2)))) //&& (board2.GetBoard()[point1.Y - 1 + Square1->GetHeight()][point1.X - 19] == 0))
				Brick2->MoveYAxis(1);

			else //if (Square2->GetHeight() + point2.Y >= this->GetPlayer2().GetFrame_Height() + 1)//The square has reached the bottom line
			{
				board2.updateBoard(Brick2, this->GetPlayer2().GetEdge());
				WhileBreaker = 2;
				delete Brick2;
				break;
			}

			if (_kbhit())
			{
				int choice = 0;
				c = _getch();
				if (c == 27)//ESC character, pauses the game
				{
					Gotoxy(0, this->GetPlayer1().GetFrame_Height() + 4);
					int choice = PrintPausedMainMenu();
					if (choice == 1)
					{
						system("cls");
						delete Brick1;
						delete Brick2;
						Gotoxy(1, 1);
						return 1;
					}
					if (choice == 2) {
						system("cls");
						Game::PrintGameInFrame(&board1, &board2, color);
					}
					else if (choice == 8) {
						PrintInstructionsAndKeys();
						
						system("cls");
						Game::PrintGameInFrame(&board1, &board2, color);
					}
					else if (choice == 9)
					{
						delete Brick1;
						delete Brick2;
						return 0;
					}
						

				}
				//every key pressed is matched to a certain movement, which is allowed is there is manouvering space (the conditions for rotating are harsher to prevent collisions with other shapes)
				else if (((c == 'a') || (c == 'A')) && (Brick1->IsFreeToLeft(&board1)))
					Brick1->MoveXAxis(-1);              
				else if (((c == 'd') || (c == 'D')) && (Brick1->IsFreeToRight(&board1)))
					Brick1->MoveXAxis(1);            
				else if (((c == 'x') || (c == 'X')) && (Brick1->isFreeToDescend(&board1)))
					Brick1->MoveYAxis(1);
				else if ((c == 's') || (c == 'S') && (Brick1->IsFreeToLeft(&board1)) && (Brick1->IsFreeToRight(&board1)) && (Brick1->isFreeToDescend(&board1)))
					Brick1->rotateShapeClockwise(this->GetPlayer1().GetEdge());
				else if ((c == 'w') || (c == 'W') && (Brick1->IsFreeToLeft(&board1)) && (Brick1->IsFreeToRight(&board1)) && (Brick1->isFreeToDescend(&board1)))
					Brick1->rotateCounterclockwise(this->GetPlayer1().GetEdge());


				else if (((c == 'j') || (c == 'J')) && (Brick2->IsFreeToLeft(&board2)))
					Brick2->MoveXAxis(-1);               
				else if (((c == 'l') || (c == 'L')) && (Brick2->IsFreeToRight(&board2)))
					Brick2->MoveXAxis(1);
				else if (((c == 'm') || (c == 'M')) && (Brick2->isFreeToDescend(&board2)))
					Brick2->MoveYAxis(1);
				else if ((c == 'k') || (c == 'K') && (Brick2->IsFreeToLeft(&board2)) && (Brick2->IsFreeToRight(&board2)) && (Brick2->isFreeToDescend(&board2)))
					Brick2->rotateShapeClockwise(this->GetPlayer1().GetEdge());
				else if ((c == 'i') || (c == 'I') && (Brick2->IsFreeToLeft(&board2)) && (Brick2->IsFreeToRight(&board2)) && (Brick2->isFreeToDescend(&board2)))
					Brick2->rotateCounterclockwise(this->GetPlayer1().GetEdge());



				//Fixers. If the coordinates have somehow exceeded the bounds of the game
				if (Brick1->LeftmostPoint()->X < this->GetPlayer1().GetEdge().X)
					Brick1->MoveXAxis(1);

				if (Brick1->RightmostPoint()->X >= this->GetPlayer1().GetEdge().X + this->GetPlayer1().GetFrame_Width())
					Brick1->MoveXAxis(-1);

				if (Brick2->LeftmostPoint()->X < this->GetPlayer2().GetEdge().X)
					Brick2->MoveXAxis(1);

				if (Brick2->RightmostPoint()->X > this->GetPlayer2().GetEdge().X + this->GetPlayer2().GetFrame_Width())
					Brick2->MoveXAxis(-1);
			}
		}
	
		refreshFlag1 = board1.PcheckRowsFull();
		refreshFlag2 = board2.PcheckRowsFull();
		//The score system is 10 points per every line cleared.
		this->GetPlayer1().SetScore(refreshFlag1 * 10);
		this->GetPlayer2().SetScore(refreshFlag2 * 10);
		if ((checkIfBoardFull(board1)) &&(checkIfBoardFull(board2)))//If both players lost at the same time then determine the winner as the player with the highest score
		{
			Sleep(500);
			system("cls");
			cout.flush();

			cout << "BOTH LOST AT THE SAME TIME!!" << endl;
			cout << "Player 1 score : " << this->GetPlayer1().GetScore() << endl;
			cout << "Player 2 score : " << this->GetPlayer2().GetScore() << endl;
			if(this->GetPlayer1().GetScore() > this->GetPlayer2().GetScore())
				cout << "PLAYER 1 WINS CONGRATULATIONS!!" << endl;
			else if (this->GetPlayer1().GetScore() > this->GetPlayer2().GetScore())
				cout << "PLAYER 2 WINS CONGRATULATIONS!!" << endl;
			else
				cout << "THERE IS A TIE!!" << endl;

			cout << "\n**Press any Key to go back to Main Menu**" << endl;
			c = _getch();
			system("cls");
			if (Brick1 == nullptr)
				delete Brick2;
			else if (Brick2 == nullptr)
				delete Brick1;
			return 2;
		}

		else if (checkIfBoardFull(board1))//If player 1 lost
		{
			Sleep(500);
			system("cls");
			cout.flush();

			cout << "PLAYER 2 WINS CONGRATULATIONS!!" << endl;
			cout << "\n**Press any Key to go back to Main Menu**" << endl;
			c = _getch();
			system("cls");

			if (Brick1 == nullptr)
				delete Brick2;
			else if (Brick2 == nullptr)
				delete Brick1;
			return 2;
		}

		else if (checkIfBoardFull(board2))//if player 2 lost
		{
			Sleep(500);
			system("cls");
			cout.flush();

			cout << "PLAYER 1 WINS CONGRATULATIONS!!" << endl;
			cout << "\n**Press any Key to go back to Main Menu**" << endl;
			c = _getch();
			system("cls");

			if (Brick1 == nullptr)
				delete Brick2;
			else if (Brick2 == nullptr)
				delete Brick1;
			return 2;
		}


	};
}

Player& Game::GetPlayer1() { return this->p1; }
Player& Game::GetPlayer2() { return this->p2; }

void Game::DeleteShape(shapes* shape)//Deletes what the DrawShape function drew.Gives the effect of movement.
{
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	int shapeColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	for (int i = 0; i < 4; i++)
	{
		SetConsoleTextAttribute(console_color, shapeColor);
		Gotoxy(shape->GetCoordArr()[i].X, shape->GetCoordArr()[i].Y);
		cout << " ";
	}
}

shapes* Game::GenerateShape(const COORD& point)//Creates a shape in relation to the given point.
//Every shape in the game can be described in a 4x4 matrix/array. Therefore the point given is the equivilant to the (1,1) cell.
{

	//The initial coordinates were initiated with the point (1,1) in mind. So in order to make the shape somewhere else we need 
	//to add the coordinate differences to the x and y values respectively to make up for the values.
	int xDiff;
	if (point.X >= 14)//belongs to the 2nd player
		xDiff = point.X - this->GetPlayer2().GetEdge().X + 18;
	else//belongs to the first player
		xDiff = point.X - this->GetPlayer1().GetEdge().X;

	int yDiff = point.Y - 1;//The y value of both edges is 1
	shapes* Pshape = new shapes();

	for (int i = 0; i < 4; i++)
	{
		Pshape->GetCoordArr()[i].X += xDiff;
		Pshape->GetCoordArr()[i].Y += yDiff;
	}

	return Pshape;
}

void Game::DrawShape(shapes* shape)//This function draws the coordinates of a given shape with the '#' sign.
{
	for (int i = 0; i < 4; i++)
	{
		Gotoxy(shape->GetCoordArr()[i].X, shape->GetCoordArr()[i].Y);
		cout << "#";
	}
}


void Game::PrintGameInFrame(Board* board1, Board* board2,bool color)//Prints the boards' bounds along with the bricks and their colors.
{

	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	int shapeColor;
	if (color)
		shapeColor = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY | BACKGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	else 
		shapeColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	for (int col = this->GetPlayer1().GetEdge().X; col < this->GetPlayer1().GetFrame_Width() + this->GetPlayer1().GetEdge().X; col++)
	{
		SetConsoleTextAttribute(console_color, shapeColor);
		Gotoxy(col, this->GetPlayer1().GetEdge().Y - 1);
		cout << "-";

		SetConsoleTextAttribute(console_color, shapeColor);
		Gotoxy(col, this->GetPlayer1().GetFrame_Height() + this->GetPlayer1().GetEdge().Y);
		cout << "-";
	}

	for (int row = this->GetPlayer1().GetEdge().Y - 1; row <= this->GetPlayer1().GetFrame_Height() + this->GetPlayer1().GetEdge().Y; row++)
	{
		SetConsoleTextAttribute(console_color, shapeColor);
		Gotoxy(this->GetPlayer1().GetEdge().X - 1, row);
		cout << "|";

		SetConsoleTextAttribute(console_color, shapeColor);
		Gotoxy(this->GetPlayer1().GetFrame_Width() + this->GetPlayer1().GetEdge().X, row);
		cout << "|";
	}

	for (int col = this->GetPlayer2().GetEdge().X; col < this->GetPlayer2().GetFrame_Width() + this->GetPlayer2().GetEdge().X; col++)
	{
		SetConsoleTextAttribute(console_color, shapeColor);
		Gotoxy(col, this->GetPlayer2().GetEdge().Y - 1);
		cout << "-";

		Gotoxy(col, this->GetPlayer2().GetFrame_Height() + this->GetPlayer2().GetEdge().Y);
		cout << "-";
	}

	for (int row = this->GetPlayer2().GetEdge().Y - 1; row <= this->GetPlayer2().GetFrame_Height() + this->GetPlayer2().GetEdge().Y; row++)
	{
		SetConsoleTextAttribute(console_color, shapeColor);
		Gotoxy(this->GetPlayer2().GetEdge().X - 1, row);
		cout << "|";

		SetConsoleTextAttribute(console_color, shapeColor);
		Gotoxy(this->GetPlayer2().GetFrame_Width() + this->GetPlayer2().GetEdge().X, row);
		cout << "|";
	}

	for (int i = 0; i <= this->GetPlayer1().GetFrame_Height(); i++)
	{
		for (int j = 1; j <= this->GetPlayer1().GetFrame_Width(); j++)
			if (board1->GetBoard()[i][j] == 1)
			{
				Gotoxy(j, i);
				cout << "&";
			}
	}

	for (int i = 0; i <= this->GetPlayer2().GetFrame_Height() + 1; i++)
	{
		for (int j = 1; j <= this->GetPlayer2().GetFrame_Width(); j++)
			if (board2->GetBoard()[i][j] == 1)
			{
				Gotoxy(j + 18, i);
				cout << "&";
			}
	}

	SetConsoleTextAttribute(console_color, FOREGROUND_BLUE);
	Gotoxy(1,21);
	cout << "Player 1 score : " << this->GetPlayer1().GetScore();
	Gotoxy(23, 21);
	cout << "Player 2 score : " << this->GetPlayer2().GetScore();

}
