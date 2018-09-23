#include <../../../five_in_a_row/interface.h>



void drawgraph() {

	while (true) {
		init_board();
		Chess chess;



		while (true) {
			if (chess.ai_first) {
				if (chess.first)
					ai_give_a_move_advance(chess);
				else player_give_a_move(chess);
				//ai_give_a_move(chess);
			}
			else {
				if (chess.first) player_give_a_move(chess);
					
				else ai_give_a_move_advance(chess);
			}

			chess.first = !chess.first; // alternate player

			draw_chess(chess);
			chess.check_if_win();//check if any player wins
			if (chess.win == true) {
				print_winner(chess);
				break;
			}
		}



		//wait until mouse is up
		auto m = GetMouseMsg();
		while (!m.mkLButton) {
			m = GetMouseMsg();
		}
		//exit
		closegraph();
	}
}

void init_board() {
	initgraph(850,600);	
	setfillcolor(HSVtoRGB(25, float(0.332), float(0.839)));
	solidrectangle(300,40,810,550); //set border of the game
	//draw board
	setlinecolor(BLACK);
	int x = 310, y = 50;
	for (int i = 0; i != 15; i++) {
		line(310,y,800,y);
		y += 35;
	}
	for (int i = 0; i != 15; i++) {
		line(x, 50, x, 540);
		x += 35;
	}
	//draw marks
	setfillcolor(BLACK);
	solidcircle(555, 295, 3);
	solidcircle(415, 435, 3);
	solidcircle(415, 155, 3);
	solidcircle(695, 435, 3);
	solidcircle(695, 155, 3);	

	outtextxy(100, 30, "Text Output:");

	return;
}

void draw_chess(const Chess &chess) {
	setorigin(310, 50);
	for (int i = 0; i != 15;i++) {
		for (int j = 0; j != 15; j++) {
			if (chess.moves[i][j] == 1) {
				setfillcolor(BLACK);
			}
			else if(chess.moves[i][j] == 2) {
				setfillcolor(WHITE);
			}

			if (chess.moves[i][j] != 0) fillcircle(35*i, 35*j, 12);

			//draw mark for last chess
			setfillcolor(RED);
			fillcircle(35 * chess.last_move[0], 35 * chess.last_move[1], 2);
		}
	}
	return;
}

void player_give_a_move(Chess &chess){
	//wait until mouse is clicked
	MOUSEMSG m = GetMouseMsg();
	while (!m.mkLButton) {
		m = GetMouseMsg();
	}

		if (m.mkLButton == true) {
			//find coordinates of the place
			int row, col;
			int x = m.x - 310, y = m.y - 50;
			col = int(round(x / 35.0)); 
			row = int(round(y / 35.0));
			if (chess.moves[col][row] == 0) {
				chess.moves[col][row] = chess.first ? 1 : 2;

				chess.last_move = { x,y };
			}
			

			//wait until mouse is up
			m = GetMouseMsg();
			while (m.mkLButton) {
				m = GetMouseMsg();
		}
	}
}


void print_winner(Chess &chess) {
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	setorigin(0, 0);
	int x = 550, y = 2;
	if (chess.first) {
		TCHAR s[] = "White Wins!";
		outtextxy(x, y, s);
	}
	else {
		TCHAR s[] = "Black Wins!";
		outtextxy(x, y, s);
	}
	
}
