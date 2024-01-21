#pragma once
#include "Player.h"
#include "shapes.h"
#include "Board.h"

class Player;
class shapes;
class Board;

class Game
{
private:
	Player p1;
	Player p2;

public:

	Game();
	Player& GetPlayer1();
	Player& GetPlayer2();
	shapes* GenerateShape(const COORD& point);
	void MainGame();
	int go(bool color);
	void PrintGameInFrame(Board* board1, Board* board2, bool color);
	void DrawShape(shapes* shape);
	void DeleteShape(shapes* Shape);
	
};