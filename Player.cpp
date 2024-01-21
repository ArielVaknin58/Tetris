#pragma once
#include "Player.h"
#include "shapes.h"
#include <iostream>
using namespace std;

enum class SIZES {HEIGHT = 18, WIDTH = 12};

Player::Player(int side) 
	:Frame_Height((int)SIZES::HEIGHT),
	Frame_Width((int)(SIZES::WIDTH)),
	score(0)
{
	if (side == 0)
	{
		this->Edge.X = 1;
		this->Edge.Y = 1;
	}
	else if(side == 1)
	{
		this->Edge.X = 19;
		this->Edge.Y = 1;
	}	
	else
	{
		this->Edge.X = 10;
		this->Edge.Y = 10;
		cout << "Wrong constructor input";
	}
}


Player::Player()//default constructor
	:Frame_Height((int)SIZES::HEIGHT),
	Frame_Width((int)(SIZES::WIDTH))
{
	this->Edge.X = 1;
	this->Edge.Y = 1;
	this->score = 0;
}


const int& Player::GetScore() { return this->score; }
void Player::SetScore(const int num) { this->score += num; }
void Player::SetEdge(COORD& edge) { this->Edge = edge; }
COORD& Player::GetEdge() { return this->Edge; }