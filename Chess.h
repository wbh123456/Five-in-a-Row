#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <graphics.h>
#include <conio.h>


class Chess
{
public:
	std::vector<std::vector<int> > moves;

	bool ai_first = false;
	bool first = true;
	bool win = false;
	std::vector<int> last_move;

	std::vector<std::vector<std::vector<int> > > gaining_vector;//for debugging purpose, initialize in the constructor

	Chess();
	~Chess();
	void check_if_win(); //check if any player win the game
	int winning_point_gain(int x, int y); //evaluate winning point gain at a certain point

};


