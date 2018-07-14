#include <../../ai.h>


//ai gives a move
void ai_give_a_move(Chess &chess) {

	//traverse through all possible moves, find the one with the largest gain
	int largest_gain = -10000000000;
	std::vector<std::vector<int> > candidates;
	int x, y;

	//only check for points near moves
	std::vector<std::vector<bool> > check;
	for (int i = 0; i != 15; i++) {
		std::vector<bool> l;
		for (int j = 0; j != 15; j++) {
			l.push_back(false);
		}
		check.push_back(l);
	}

	for (int i = 0; i != 15; i++) {
		for (int j = 0; j != 15; j++) {
			if (chess.moves[i][j] != 0) {
				for (int a = -2; a != 3; a++) {
					for (int b = -2; b != 3; b++) {
						int m = i + a; int n = j + b;
						if (m >= 0 && m <= 14 && n >= 0 && n <= 14) {
							check[m][n] = true;
						}
					}
				}
			}
		}
	}


	//find the best move( with very high score)
	for (int i = 0; i != 15; i++) {
		for (int j = 0; j != 15; j++) {

			if (chess.moves[i][j] == 0 && check[i][j] == true) {

				int gain = chess.winning_point_gain(i, j);

				//out put gain onto the board
				setorigin(310, 60);
				std::string s = std::to_string(gain);
				outtextxy(35 * i, 35 * j, s.c_str());

				//if the move is no better than 5% of the current best move ,then put it to the cendidates
				if (gain > largest_gain) {
					largest_gain = gain;
					x = i; y = j;
					candidates.clear();
					candidates.push_back({ i,j });
				}
				else if(float(largest_gain - gain) <= gain*0.05)  {
					candidates.push_back({i,j});
				}
			}

		}
	}
	srand(unsigned(time(NULL)));
	random_shuffle(candidates.begin(), candidates.end());

	x = candidates[0][0];
	y = candidates[0][1];

	if (chess.ai_first && chess.moves[7][7] == 0) {
		x = 7; y = 7;
	}
	//DEBUGGING:
	
	int x_ = 6, y_ = 6;
		setorigin(0, 0);

		setfillcolor(BLACK);
		solidrectangle(10, 80, 250, 550);
		//display the deatails at largest gaining point
		int before = chess.gaining_vector[x_][y_][0], after = chess.gaining_vector[x_][y_][1];

		std::string s = "debugging : i = " + std::to_string(x_) + " , j = " + std::to_string(y_);
		outtextxy(50, 80, s.c_str());
		s = "points gained = " + std::to_string(after - before);
		outtextxy(50, 100, s.c_str());
		s = "before: " + std::to_string(chess.gaining_vector[x_][y_][0]);
		outtextxy(50, 120, s.c_str());
		s = "after: " +std::to_string(chess.gaining_vector[x_][y_][1]);
		outtextxy(50, 140, s.c_str());
	

    //update chess
	chess.moves[x][y] = chess.first ? 1 : 2;
	chess.last_move = { x,y };
	

	return;
}


//ai give a move with prediction depth
void ai_give_a_move_advance(Chess &chess) {

	//for debugging, information shows more info in prediction dpeth
	//info[i][j] = {first gain, second move x, second move y, second gain, third gain}
	std::vector<std::vector<std::vector<int> > > information;
	for (int i = 0; i != 15; i++) {
		std::vector<std::vector<int> > l;
		for (int j = 0; j != 15; j++) {
			std::vector<int> ll;
			l.push_back(ll);
		}
		information.push_back(l);
	}

	//traverse through all possible moves, find the one with the largest gain
	int largest_gain = -10000000000;
	std::vector<std::vector<int> > candidates;
	int x, y;

	//only check for points near moves
	std::vector<std::vector<bool> > check;
	for (int i = 0; i != 15; i++) {
		std::vector<bool> l;
		for (int j = 0; j != 15; j++) {
			l.push_back(false);
		}
		check.push_back(l);
	}

	for (int i = 0; i != 15; i++) {
		for (int j = 0; j != 15; j++) {
			if (chess.moves[i][j] != 0) {
				for (int a = -2; a != 3; a++) {
					for (int b = -2; b != 3; b++) {
						int m = i + a; int n = j + b;
						if (m >= 0 && m <= 14 && n >= 0 && n <= 14) {
							check[m][n] = true;
						}
					}
				}
			}
		}
	}


	//find the best move( with very high score)
	for (int i = 0; i != 15; i++) {
		for (int j = 0; j != 15; j++) {

			if (chess.moves[i][j] == 0 && check[i][j] == true) {

				int first_opponent_best_gain = 0,  second_move_gain = 0, second_opponent_gain = 0;

				int first_move_gain = chess.winning_point_gain(i, j);

				//score the chess after this move
				std::vector<int> best_move, best_move2, best_move3;
				Chess chess_2 = chess;
				chess_2.moves[i][j] = chess_2.first ? 1 : 2;
				chess_2.first = !chess_2.first;
				chess_2.check_if_win();
				 first_opponent_best_gain = best_move_gain(chess_2, best_move);
				if (chess_2.win) {
					first_opponent_best_gain = -100000000;
				}
				else {

					Chess chess_3 = chess_2;
					chess_3.moves[best_move[0]][best_move[1]] = chess_3.first ? 1 : 2;
					chess_3.first = !chess_3.first;
					chess_3.check_if_win();

					second_move_gain = best_move_gain(chess_3, best_move2);
					if (chess_3.win) {
						second_move_gain = 100000000;
					}
					else {
						Chess chess_4 = chess_3;
						chess_4.moves[best_move2[0]][best_move2[1]] = chess_4.first ? 1 : 2;
						chess_4.first = !chess_4.first;
						chess_4.check_if_win();

						second_opponent_gain = best_move_gain(chess_4, best_move3);
						if (chess_4.win) {
							second_opponent_gain = -100000000;

						}
					}
				}

				



				int gain = first_move_gain - first_opponent_best_gain;
					gain = gain + second_move_gain - second_opponent_gain;

					information[i][j] = { first_move_gain ,best_move[0], best_move[1],first_opponent_best_gain,best_move2[0],best_move2[1], second_move_gain,best_move3[0],best_move3[1], second_opponent_gain };



				//out put gain onto the board
				setorigin(310, 60);
				std::string s = std::to_string(gain);
				outtextxy(35 * i, 35 * j, s.c_str());

				//if the move is no better than 5% of the current best move ,then put it to the cendidates
				if (gain > largest_gain) {
					largest_gain = gain;
					x = i; y = j;
					candidates.clear();
					candidates.push_back({ i,j });
				}
				else if (float(largest_gain - gain) <= gain*0.05) {
					candidates.push_back({ i,j });
				}
			}

		}
	}
	srand(unsigned(time(NULL)));
	random_shuffle(candidates.begin(), candidates.end());

	x = candidates[0][0];
	y = candidates[0][1];

	if (chess.ai_first && chess.moves[7][7] == 0) {
		x = 7; y = 7;
	}


	//DEBUGGING:
	setorigin(0, 0);

	int x_ = 6, y_ =7;

	setfillcolor(BLACK);
	solidrectangle(10, 80, 250, 550);
	//display the deatails at largest gaining point
	int before = chess.gaining_vector[x_][y_][0], after = chess.gaining_vector[x_][y_][1];

	std::string s = "debugging : i = " + std::to_string(x_) + " , j = " + std::to_string(y_);
	outtextxy(50, 80, s.c_str());
	s = "points gained = " + std::to_string(after - before);
	outtextxy(50, 100, s.c_str());
	s = "before: " + std::to_string(chess.gaining_vector[x_][y_][0]);
	outtextxy(50, 120, s.c_str());
	s = "after: " + std::to_string(chess.gaining_vector[x_][y_][1]);
	outtextxy(50, 140, s.c_str());

	s = "opponent gain: " + std::to_string(information[x_][y_][3]);
	outtextxy(50, 160, s.c_str());
	s = "  opponent best move: " + std::to_string(information[x_][y_][1]) + " , " + std::to_string(information[x_][y_][2]);
	outtextxy(50, 180, s.c_str());

	s = "second move gain: " + std::to_string(information[x_][y_][6]);
	outtextxy(50, 220, s.c_str());
	s = "  second best move: " + std::to_string(information[x_][y_][4]) + " , " + std::to_string(information[x_][y_][5]);
	outtextxy(50, 240, s.c_str());

	s = "second opponent gain: " + std::to_string(information[x_][y_][9]);
	outtextxy(50, 280, s.c_str());
	s = "  second opponent move: " + std::to_string(information[x_][y_][7]) + " , " + std::to_string(information[x_][y_][8]);
	outtextxy(50, 300, s.c_str());
	


	//update chess
	chess.moves[x][y] = chess.first ? 1 : 2;
	chess.last_move = { x,y };

	return;
}

//find the move with best gain, return its gain
int best_move_gain(Chess &chess, std::vector<int>& best_move) {
	//traverse through all possible moves, find the one with the largest gain
	int largest_gain = -10000000000;
	std::vector<std::vector<int> > candidates;
	int x, y;

	//only check for points near moves
	std::vector<std::vector<bool> > check;
	for (int i = 0; i != 15; i++) {
		std::vector<bool> l;
		for (int j = 0; j != 15; j++) {
			l.push_back(false);
		}
		check.push_back(l);
	}

	for (int i = 0; i != 15; i++) {
		for (int j = 0; j != 15; j++) {
			if (chess.moves[i][j] != 0) {
				for (int a = -2; a != 3; a++) {
					for (int b = -2; b != 3; b++) {
						int m = i + a; int n = j + b;
						if (m >= 0 && m <= 14 && n >= 0 && n <= 14) {
							check[m][n] = true;
						}
					}
				}
			}
		}
	}


	//find the best move( with very high score)
	for (int i = 0; i != 15; i++) {
		for (int j = 0; j != 15; j++) {

			if (chess.moves[i][j] == 0 && check[i][j] == true) {

				int gain = chess.winning_point_gain(i, j);

				//if the move is no better than 5% of the current best move ,then put it to the cendidates
				if (gain > largest_gain) {
					largest_gain = gain;
					x = i; y = j;
				}
			}

		}
	}
	best_move = { x, y };


	return largest_gain;
}

