#pragma once
#include "shapes.h"
#include "Windows.h"
#include "Game.h"
#include "Player.h"
#include "MainFunctions.h"
#include<iostream>

using namespace std;
//An array to hold the initial values of the shapes
COORD Arr[7][4] =
{
	{ {1,1}, {2,1}, {1,2}, {2,2} }, //Square
	{ {1,1}, {2,1}, {3,1}, {4,1} }, //Rectangle
	{ {1,1}, {1,2}, {2,2}, {1,3} }, //Plus
	{ {2,1}, {3,1}, {1,2}, {2,2} }, //S
	{ {1,1}, {1,2}, {2,2}, {3,2} }, //"lying" L side1
	{ {3,1}, {1,2}, {2,2}, {3,2} }, //"lying" L side2
	{ {1,1}, {2,1}, {2,2}, {3,2} }, //inverted S
};

shapes::shapes()
{
	srand(time(0));
	int random = rand() % 7;
	COORD arr[4];
	std::copy(std::begin(Arr[random]), std::end(Arr[random]), std::begin(coordArr));

	Matrix = InitShapeMatrix(random);
	identifier = (Shape)random;
	width = coordArr[3].X - coordArr[0].X + 1;
	height = coordArr[3].Y - coordArr[0].Y + 1;

}

int** shapes::getMatrix() { return this->Matrix; }

COORD* shapes::GetCoordArr() { return this->coordArr; }

int shapes::GetWidth() { return this->width; }
int shapes::GetHeight() { return this->height; }

void shapes::SetHeight(int num) { this->height = num; }
void shapes::SetWidth(int num) { this->width = num; }

int shapes::GetMatrixROWS() { return 4; }
int shapes::GetMatrixCOLS() { return 4; }


void shapes::rotateShapeClockwise(COORD edge)//We used knowledge from Linear Algebra to rotate the shape's matrix and get the coordinates from the new matrix.
{
	short counter = 0;
	// Transpose
	for (int i = 0; i < this->GetMatrixROWS(); ++i)
	{
		for (int j = i + 1; j < this->GetMatrixCOLS(); ++j)
			swap(this->getMatrix()[i][j], this->getMatrix()[j][i]);

	}
	// Multiply by horizontally mirrored matrix (swap columns)
	for (int i = 0; i < this->GetMatrixROWS(); ++i)
	{
		for (int j = 0; j < this->GetMatrixCOLS() / 2; ++j)
			swap(this->getMatrix()[i][j], this->getMatrix()[i][this->GetMatrixCOLS() - 1 - j]);
	}

	short xDiff = this->GetCoordArr()[counter].X - edge.X;
	short yDiff = this->GetCoordArr()[counter].Y - edge.Y;
	for (int i = 0; i < this->GetMatrixROWS(); i++)
	{
		for (int j = 0; j < this->GetMatrixCOLS(); j++)
		{
			if (this->getMatrix()[i][j] == 1)
			{
				this->GetCoordArr()[counter].X = i + 1;
				this->GetCoordArr()[counter].Y = j + 1;
				swap(this->GetCoordArr()[counter].X, this->GetCoordArr()[counter].Y);
				this->GetCoordArr()[counter].X += xDiff - 1;
				this->GetCoordArr()[counter].Y += yDiff;
				counter++;
			}
			if (counter == 4)
				break;
		}
	}

	Swap(this->height, this->width);
}

shapes::~shapes() 
{
	if (Matrix != nullptr) 
	{
		for (int i = 0; i < 4; ++i) 
			delete[] Matrix[i];
		
		delete[] Matrix;
		Matrix = nullptr;  
	}
}
int** shapes::InitShapeMatrix(int ShapeNum)
{

	int** arr = new int* [this->GetMatrixROWS()];
	for (int i = 0; i < this->GetMatrixROWS(); i++)
	{
		arr[i] = new int[4];
		for (int j = 0; j < this->GetMatrixCOLS(); j++)
			arr[i][j] = 0;
	}
	switch (ShapeNum)
	{
	case 0://Square
		arr[0][0] = arr[0][1] = arr[1][0] = arr[1][1] = 1;
		break;
	case 1://Rectangle
		arr[0][0] = arr[0][1] = arr[0][2] = arr[0][3] = 1;
		break;
	case 2://Plus 
		arr[0][0] = arr[1][0] = arr[1][1] = arr[2][0] = 1;
		break;
	case 3://S 
		arr[0][1] = arr[0][2] = arr[1][0] = arr[1][1] = 1;
		break;
	case 4://L
		arr[0][0] = arr[1][0] = arr[1][1] = arr[1][2] = 1;
		break;
	case 5://Rotated L
		arr[0][2] = arr[1][0] = arr[1][1] = arr[1][2] = 1;
		break;
	case 6://Rotated S
		arr[0][0] = arr[0][1] = arr[1][1] = arr[1][2] = 1;
		break;
	default:
		cout << "wrong ShapeNum input" << endl;
	}
	return arr;
}

void shapes::MoveXAxis(int LengthToMove)
{
	for (int i = 0; i < 4; i++)
		this->GetCoordArr()[i].X += LengthToMove;
}
void shapes::MoveYAxis(int LengthToMove)
{
	for (int i = 0; i < 4; i++)
		this->GetCoordArr()[i].Y += LengthToMove;

}

COORD shapes::LowestPoint() { return this->GetCoordArr()[3]; }
COORD shapes::HighestPoint() { return this->GetCoordArr()[0]; }
COORD* shapes::RightmostPoint()//returns the coordinate with the highest x value
{
	COORD* res = &(this->GetCoordArr()[0]);
	for (int i = 1; i < 4; i++)
		if (this->GetCoordArr()[i].X >= res->X)
			res = &(this->GetCoordArr()[i]);
	return res;
}
COORD* shapes::LeftmostPoint()//returns the coordinate with the smallest x value
{
	COORD* res = &(this->GetCoordArr()[0]);
	for (int i = 1; i < 4; i++)
		if (this->GetCoordArr()[i].X <= res->X)
			res = &(this->GetCoordArr()[i]);
	return res;
}

bool shapes::isFreeToDescend(Board* board)//assume to descend only 1
{
	int y = this->LowestPoint().Y;
	int x = this->LowestPoint().X;
	if (x < 18)
	{
		for (int i = 0; i < 4; i++)
		{
			if (board->GetBoard()[this->GetCoordArr()[i].Y + 1][this->GetCoordArr()[i].X] != 0)
				return false;
		}
	}
	else
	{
		for (int i = this->LeftmostPoint()->X - 18; i <= this->RightmostPoint()->X - 18; i++)
		{
			if (board->GetBoard()[y + 1][i] != 0)
				return false;
		}
	}


	return true;
}

bool shapes::IsFreeToRight(Board* board)//checks if a shape can move 1 pixel to the right
{
	int y = this->RightmostPoint()->Y;
	int x = this->RightmostPoint()->X;
	if (x < 18)
	{
		for (int i = 0; i < 4; i++)
		{
			if (this->GetCoordArr()[i].X == x)
			{
				if (board->GetBoard()[this->GetCoordArr()[i].Y][this->GetCoordArr()[i].X + 1] != 0)
					return false;
			}
			
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (this->GetCoordArr()[i].X == x)
			{
				if (board->GetBoard()[y][x+1] != 0)
					return false;
			}
			
		}
	}


	return true;
}

bool shapes::IsFreeToLeft(Board* board)//checks if the shape can move 1 pixel to the left
{
	int y = this->LeftmostPoint()->Y;
	int x = this->LeftmostPoint()->X;
	if (x < 18)
	{
		for (int i = 0; i < 4; i++)
		{
			if (this->GetCoordArr()[i].X == x)
			{
				if (board->GetBoard()[this->GetCoordArr()[i].Y][this->GetCoordArr()[i].X - 1] != 0)
					return false;
			}

		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (this->GetCoordArr()[i].X == x)
			{
				if (board->GetBoard()[y][x - 1] != 0)
					return false;
			}

		}
	}

	return true;
}



void shapes::rotateCounterclockwise(COORD edge)// Function to rotate a matrix counterclockwise, similar rational to the rotateCloclwise function
{
	short counter = 0;

	// Reverse columns
	for (int j = 0; j < this->GetMatrixCOLS() / 2; ++j) {
		for (int i = 0; i < this->GetMatrixROWS(); ++i)
			swap(this->getMatrix()[i][j], this->getMatrix()[i][this->GetMatrixCOLS() - 1 - j]);

	}

	// Transpose
	for (int i = 0; i < this->GetMatrixROWS(); ++i) {
		for (int j = i + 1; j < this->GetMatrixCOLS(); ++j) {
			swap(this->getMatrix()[i][j], this->getMatrix()[j][i]);
		}
	}


	short xDiff = this->GetCoordArr()[counter].X - edge.X;
	short yDiff = this->GetCoordArr()[counter].Y - edge.Y;
	for (int i = 0; i < this->GetMatrixROWS(); i++)
	{
		for (int j = 0; j < this->GetMatrixCOLS(); j++)
		{
			if (this->getMatrix()[i][j] == 1)
			{
				this->GetCoordArr()[counter].X = i + 1;
				this->GetCoordArr()[counter].Y = j + 1;
				swap(this->GetCoordArr()[counter].X, this->GetCoordArr()[counter].Y);
				this->GetCoordArr()[counter].X += xDiff - 1;
				this->GetCoordArr()[counter].Y += yDiff;
				counter++;
			}
			if (counter == 4)
				break;
		}
	}

	Swap(this->height, this->width);
}