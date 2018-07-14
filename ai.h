#pragma once
#include <../../Chess.h>
#include <graphics.h>
#include <string>
#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <time.h>

//ai gives a move
void ai_give_a_move (Chess &chess);

//ai give a move with prediction depth
void ai_give_a_move_advance(Chess &chess);

//find the move with best gain, return its gain
int best_move_gain(Chess &chess, std::vector<int> &best_move);
