#pragma once
#include <Windows.h>

class Game;
enum class SIZES;

class Player
{
	private:
		int score;
		COORD Edge;
		int Frame_Height;
	    int Frame_Width;

	public:
	Player();
	Player(int side);
	const int& GetScore();
	COORD& GetEdge();
	inline int GetFrame_Height() { return this->Frame_Height; };
	inline int GetFrame_Width() { return this->Frame_Width; };
	void SetScore(const int num);
	void SetEdge(COORD& edge);
	
};

