#include "Chess.h"

#define MOVEBLOCK_SIZE 33



bool check_rep(const std::vector<std::vector<int> > &chess, int x, int y, int num_rep); // helper function to check repetitions in a row
int winning_point(std::vector<int> & moves, bool first, std::vector<int> &num_rep); //give a vector with 11 int, return the winning point, bool represents if ai is first

Chess::Chess()
{
	
	for (int i = 0; i != 15; i++) {
		std::vector<int> l;
		for (int j = 0; j != 15; j++) {
			l.push_back(0);
		}
		moves.push_back(l);
	}

	for (int i = 0; i != 15; i++) {
		std::vector<std::vector<int> > l;
		for (int j = 0; j != 15; j++) {
			std::vector<int> l2 = {0, 0, 0, 0, 0, 0, 0, 0};
			l.push_back(l2);
		}
		gaining_vector.push_back(l);
	}

	last_move = { 0,0 };
}


Chess::~Chess()
{
	for (int i = 0; i != 15; i++) {
			moves[i].clear();          
	}
	moves.clear();

	for (int i = 0; i != 15; i++) {
		for (int j = 0; j != 15; j++) {
			gaining_vector[i][j].clear();

		}
		gaining_vector[i].clear();
	}
	gaining_vector.clear();

	last_move.clear();
	
}

void Chess::check_if_win() {
	//check if any neibor is the same color, if true then check if its 5 in a row
	//to avoid unness repetitions, only check 4 directions: R,RD,D,LD

	if (check_rep(moves, 1,0, 5)) win = true;
	else if (check_rep(moves, 1, 1, 5)) win = true;
    else if (check_rep(moves, 0, 1,5) ) win = true;
	else if (check_rep(moves, -1, 1,5)) win = true;
}

int Chess::winning_point_gain(int x, int y) {
	//winning point gain = winning point after - winning point before
	int points_gain = 0;
	std::vector<int> move_block; // the move block that is going to check for winning points
	std::vector<int> num_rep_before = { 0,0,0 }, num_rep_after = {0,0,0}; // {num of 2 repitaitons, num of 3, num of4}
	int x_, y_; // local moves index

	int point_before = 0, point_after = 0;

	//gain on R direction 
	x_ = x -(MOVEBLOCK_SIZE-1)/2;
	for (int i = 0; i != MOVEBLOCK_SIZE; i++) {
		if (x_ >= 0 && x_ <= 14) {
			move_block.push_back(moves[x_][y]);
		}
		else {
			move_block.push_back(3); // 3 represents wall
		}
		x_ += 1;
	}
	int point_before0 = winning_point(move_block, first, num_rep_before);
	move_block[(MOVEBLOCK_SIZE - 1) / 2] = first ? 1 : 2;
	int point_after0 = winning_point(move_block, first, num_rep_after);
	
	move_block.clear();

	
	//gain on RD direction
	x_ = x - (MOVEBLOCK_SIZE - 1) / 2; y_ = y - (MOVEBLOCK_SIZE - 1) / 2;
	for (int i = 0; i != MOVEBLOCK_SIZE; i++) {
		if (x_ >= 0 && x_ <= 14 && y_ >= 0 && y_ <= 14) {
			move_block.push_back(moves[x_][y_]);
		}
		else {
			move_block.push_back(3); // 3 represents wall
		}
		x_ += 1; y_ += 1;
	}
	int point_before1 = winning_point(move_block, first, num_rep_before);
	move_block[(MOVEBLOCK_SIZE - 1) / 2] = first ? 1 : 2;
	int point_after1 = winning_point(move_block, first, num_rep_after);
	
	move_block.clear();

	//gain on D direction
	y_ = y - (MOVEBLOCK_SIZE - 1) / 2;
	for (int i = 0; i != MOVEBLOCK_SIZE; i++) {
		if ( y_ >= 0 && y_ <= 14) {
			move_block.push_back(moves[x][y_]);
		}
		else {
			move_block.push_back(3); // 3 represents wall
		}
		y_ += 1;
	}
	int point_before2 = winning_point(move_block, first, num_rep_before);
	move_block[(MOVEBLOCK_SIZE - 1) / 2] = first ? 1 : 2;
	int point_after2 = winning_point(move_block, first, num_rep_after);
	
	move_block.clear();

	//gain on LD direction
	x_ = x - (MOVEBLOCK_SIZE - 1) / 2; y_ = y + (MOVEBLOCK_SIZE - 1) / 2;
	for (int i = 0; i != MOVEBLOCK_SIZE; i++) {
		if (x_ >= 0 && x_ <= 14 && y_ >= 0 && y_ <= 14) {
			move_block.push_back(moves[x_][y_]);
		}
		else {
			move_block.push_back(3); // 3 represents wall
		}
		x_ += 1; y_ -= 1;
	}
	int point_before3 = winning_point(move_block, first, num_rep_before);
	move_block[(MOVEBLOCK_SIZE - 1) / 2] = first ? 1 : 2;
	int point_after3 = winning_point(move_block, first, num_rep_after);
	
	move_block.clear();

	//points gain = point_after - point_before
	points_gain = point_after0 + point_after1 + point_after2 + point_after3 - point_before0- point_before1 - point_before2 - point_before3;
	

	//FOR DEGUUGING: display points gain at x, y


	gaining_vector[x][y][0] = point_before0;
	gaining_vector[x][y][1] = point_before1;
	gaining_vector[x][y][2] = point_before2;
	gaining_vector[x][y][3] = point_before3;
	gaining_vector[x][y][4] = point_after0;
	gaining_vector[x][y][5] = point_after1;
	gaining_vector[x][y][6] = point_after2;
	gaining_vector[x][y][7] = point_after3;


	return points_gain;

}

//give a vector with 11 int, return the winning point, bool represents if ai is first, num_rep represents how many rep has existed already (in other directions)
int winning_point(std::vector<int>& moves, bool first, std::vector<int> &num_rep) {
	/*
	winning points is based on (base score) and (multi factor)
	-base score depends on num of self repeitations(how many in a row) and num of repitations exists in other directions (num_rep)
	-multi depends on if there's jumps or bounds around repetitions. bounds are walls or opponent's moves
	-multi also depends on if the move is to attack or defense (attack factor)

	run through the move block twice: once to check point with black moves as self score, once the other
	final score =  white score - black score (if first == true (if ai is frist ))
	         
	*/

	int point = 0;
	//check twice: black for self = 1, white for self = 2
	for (int self = 1; self != 3; self++) {
		//find bound(walls) for the move block 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
		//bounds are walls or opponents moves
		bool bound_in_middle = false;
		if (moves[(MOVEBLOCK_SIZE-1)/2] != self && moves[(MOVEBLOCK_SIZE - 1) / 2] != 0) bound_in_middle = true;
		int min_bound, max_bound;
		int middle_minus_1= ((MOVEBLOCK_SIZE - 3) / 2);
		for (int i = middle_minus_1; i != -1; i--) {
			if (moves[i] != self && moves[i] != 0) {
				min_bound = i;
				break;
			}
			if (i == 0) min_bound = -10;
		}
		int middle_plus_1 = ((MOVEBLOCK_SIZE + 1) / 2);
		for (int i = middle_plus_1; i != MOVEBLOCK_SIZE; i++) {
			if (moves[i] != self && moves[i] != 0) {
				max_bound = i;
				break;
			}
			if (i == MOVEBLOCK_SIZE-1) max_bound = 30;
		}

		//if bounds are within distance of 5, then no points
		if (max_bound - min_bound <= 5 && bound_in_middle == false) {
			continue;
		}
		else if(bound_in_middle == true ){
			if ((MOVEBLOCK_SIZE - 1) / 2 - min_bound <= 5) {
				min_bound = (MOVEBLOCK_SIZE - 1) / 2;
			}
			if (max_bound - (MOVEBLOCK_SIZE - 1) / 2 <= 5) {
				max_bound = (MOVEBLOCK_SIZE - 1) / 2;
			}
			
		}

		//traverse through the moves block and analyze them
		//check for self repitations
		//if there's jumps b/w repitations then reduce score, if repetitions have bounds around them(not alone), then reduce marks 
		int rep = 0;
		int jump = 0; int fake_jump = 0; bool jump_flag = false; //jumps b/w repatiotions
		int alone1 = 0, alone2 = 0, alone3 = 0, alone4 = 0; //has bounds around
		int index_bound1 = min_bound, index_bound2 = max_bound;
		if (min_bound < 0) index_bound1 = 0;
		if (max_bound > moves.size()-2) index_bound2 = moves.size()-1;
		for (int i = index_bound1; i != index_bound2 +1; i++) {
			//if rep
			if (moves[i] == self) {
				rep++;
				//if jump flag in true, then fake jump can turn into jump
				if (jump_flag) {
					jump += fake_jump;
					fake_jump = 0;
				}

				//turn on jump_flag if rep
				jump_flag = true;
				//check for alones(bounds around)
				int alone_dis = bound_in_middle?  min(min(i - min_bound, max_bound - i), std::abs((MOVEBLOCK_SIZE - 1) / 2 - i)) : min(i - min_bound, max_bound - i);
				if (alone_dis == 1) alone1++;
				else if (alone_dis == 2) alone2++;
				else if (alone_dis == 3) alone3++;
				else if (alone_dis == 4) alone4++;
			}

			//if move is blank. then record as jump only if it is b/w repitations
			else if (moves[i] == 0 && jump_flag == true) {
				fake_jump++;
			}

			//if rep stops (hit bounds), then sum up the scroes
			else {
				//calculatee multi factor for the score
				float multi = pow(0.3, alone1) * pow(0.8, alone2) * pow(0.88, alone3) * pow(0.9, alone4);
				if (jump == 1) {
					multi = multi *0.85;
				}
				else if (jump == 2) {
					multi = multi *0.5;
				}
				else if (jump > 2) {
					multi = multi * 0.05;
				}
				//float multi = 1;

				//calculate basse score due to self repitations
				int rep_point = 0;

				if (rep == 1) rep_point = 10;

				else if (rep == 2) {
					if(num_rep[0] == 0)
					rep_point = 50;
					else if (num_rep[0] == 1) {
						rep_point = 200;
					}
					else {
						rep_point = 600;
					}
					if (alone1 == 0 && alone2 == 0 && alone3 == 0 && alone4 == 0 && jump <= 1)num_rep[0]++;
				}

				else if (rep == 3) {
					if (num_rep[1] == 0)
						rep_point = 500;
					else if (num_rep[1] == 1) {
						rep_point = 5000;
					}
					else {
						rep_point = 50000;
					}
					if (alone1 == 0 && alone2 == 0 && alone3 == 0 && alone4 == 0 && jump <= 1) num_rep[1]++;
				}

				else if (rep == 4) {
					num_rep[2]++;
					if (num_rep[2] == 1)
						rep_point = 5000;
					else if (num_rep[2] == 2) {
						rep_point = 50000;
					}
					else {
						rep_point = 500000;
					}
				}
				else if (rep >= 5) rep_point = 50000;

				//if 5 in a row with no jumps
				if (jump == 0 && rep >= 5) {
					if (first == false) {
						if (self == 2) point += 100000000;
						else point -= 100000000;
					}
					else {
						if (self == 1) point += 100000000;
						else point -= 100000000;
					}
				}
				//else, calculate points , jumps ,attact factor, alones
			
				float attact_factor = 0.2;
				if (first == false) {
					if (self == 2)  point += int(rep_point*multi * attact_factor);
					else  point -= int(rep_point*multi);
				}
				else {
					if (self == 1)  point += int(rep_point*multi * attact_factor);
					else  point -= int(rep_point*multi);
				}
			  
				//recover parameters
				rep = 0;
				jump = 0; fake_jump = 0 ; jump_flag = false;
				alone1 = 0; alone2 = 0; alone3 = 0; alone4 = 0;
			}
		}
	}
	/*
	for(int self = 1; self != 3; self++){
	unsigned state = 1;
	
	//traverse through the moves using state machine
	for (auto move_it = moves.begin(); move_it != moves.end(); move_it++) {
		int move = *move_it;

		if (move == 3) continue; //if wall,then go to next

		switch (state) {
		case 1:
			state = move == 0 ? 6 : 2;

			break;
		case 2:
			state = (move != self) ? 16 : 3;
			break;
		case 3:
			state = (move != self) ? 16 : 4;
			break;
		case 4:
			state = (move != self) ? 16 : 5;
			break;
		case 5:
			if (move != self) 	state = 16;
			else {
				state = 1;
				if (self == 2) point += LIVING_5_POINTS;
				else point -= LIVING_5_POINTS;
			}
			break;
		case 6:
			state = (move == 0) ? 12 : 7;

			break;
		case 7:
			state = (move != self) ? 10 : 8;
			break;
		case 8:
			state = (move != self) ? 16 : 9;
			break;
		case 9:
			if (move != self) {
				state = 1;
				if (self == 2) point += LIVING_3_POINTS;
				else point -= LIVING_3_POINTS;
			}
			else state = 5;
			break;
		case 10:
			state = (move != self) ? 17 : 11;
			break;
		case 11:
			if (move != self) {
				state = 1;
				if (self == 2) point += LIVING_2_POINTS;
				else point -= LIVING_2_POINTS;
			}
			else {
				state = 8;
			}
			break;
		case 12:
			state = (move == 0) ? 6 : 13;

			break;
		case 13:
			state = (move != self) ? 10 : 14;
			break;
		case 14:
			state = (move != self) ? 15 : 9;
			break;
		case 15:
			if (move != self) {
				state = 1;
				if (self == 2) point += LIVING_2_POINTS;
				else point -= LIVING_2_POINTS;
			}
			else state = 7;
			break;
		case 16:
			state = (move != self) ? 17 : 7;
			break;
		case 17:
			if (move != self) {
				state = 1;
				if (self == 2) point += LIVING_2_POINTS;
				else point -= LIVING_2_POINTS;
			}
			else state = 12;
			break;

		default:
			//other situations
			state = 1;
			point += OTHER_POINTS;
			break;
		}
	}
	}
	*/

	return point;
}

//helper function
//return true if the specified direction has a num in a row, x is horizontal direction increment, y in vertical direction increment
bool check_rep(const std::vector<std::vector<int> > &moves, int x, int y, int num_rep) {

	//a vector that records the checked points
	std::vector<std::vector<bool> > checked;
	for (int i = 0; i != 15; i++) {
		std::vector<bool> temp;
		for (int j = 0; j != 15; j++) {
			temp.push_back(false);
		}
		checked.push_back(temp);
	}

	//traverse all the points at a certain direction
	for (int i = 0; i != 15; i++) {
		for (int j = 0; j != 15; j++) {
			//if the point has not been checked
			if (checked[i][j] == false && moves[i][j] != 0) {
				int self = moves[i][j];
				int _i = i+x, _j = j+y;
				int num = 1;
				bool rep = true;
				while (rep) {
					if (_i != -1 && _j != -1 && _i != 15 && _j != 15 && self == moves[_i][_j]) {
						num++;
						checked[_i][_j] = true;
						//increment the position
						_i += x;
						_j += y;
						if (num >= num_rep) return true;
						rep = true;
					}

					else {
						rep = false;
					}
				}
			}
		}
	}
	return false;
}
