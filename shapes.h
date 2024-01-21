#pragma once
#include "Windows.h"
#include "Player.h"
#include "Game.h"
#include <algorithm>
#include <conio.h>
#include "Board.h"

enum class Shape { SQUARE, RECTANGLE, PLUS, S, L, REVERSES, REVERSEL };
extern COORD Arr[7][4];//7 shapes composed of 4 coordinates
class shapes
{
private:

	short width;
	short height;
	COORD coordArr[4];
	Shape identifier;
	int** Matrix;

public:
	shapes();
	~shapes();
	COORD LowestPoint();
	COORD HighestPoint();
	COORD* RightmostPoint();
	COORD* LeftmostPoint();
	COORD* GetCoordArr();
	int GetWidth();
	int GetHeight();
	int GetMatrixROWS();
	int GetMatrixCOLS();
	int** getMatrix();
	int** InitShapeMatrix(int ShapeNum);
	void SetHeight(int num);
	void SetWidth(int num);
	void rotateShapeClockwise(COORD edge);
	void rotateCounterclockwise(COORD edge);
	void MoveXAxis(int LengthToMove);
	void MoveYAxis(int LengthToMove);
	bool IsFreeToRight(Board* board);
	bool IsFreeToLeft(Board* board);
	bool isFreeToDescend(Board* board);

};
