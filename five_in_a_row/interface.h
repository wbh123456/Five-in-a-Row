#pragma once

#include <graphics.h>
#include <string>
#include <iostream>
#include <conio.h>
#include <vector>
#include <../../../five_in_a_row/Chess.h>
#include <../../ai.h>

//draw interface of the five in a row game
void drawgraph();

//draw the board
void init_board();

//draw chesses
void draw_chess(const Chess &chess);

//player gives one move
void player_give_a_move(Chess &chess);

//print winner
void print_winner(Chess &chess);
