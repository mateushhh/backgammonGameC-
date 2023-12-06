#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "conio2.h"
using namespace std;

#define UP 1
#define DOWN -1
#define BARX 29
#define OUTX 59 
#define INPUTLENGTH 16
#define NORMAL 1
#define HIT 2
#define BEAROFF 3
#define REDBAR 24
#define WHITEBAR 25
#define REDOUT 26
#define WHITEOUT 27

int diceRoll() {
	int dice;
	dice = (rand() % 6) + 1;
	return dice;
}

int diceSum(int dice[4], int n) {
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum += dice[i];
	}
	return sum;
}

//dice battle

struct board_t {
	char board[29] = "R0000W0W000RW000R0R0000WRWRW"; // pola 1-24 // barRW // out RW
	int board_numbers[28] = { 2,0,0,0,0,5,0,3,0,0,0,5,5,0,0,0,3,0,5,0,0,0,0,2,0,3,0,0 };
};

void activePlayer(char player){
	gotoxy(64, 11);
	if (player == 'W')
		cout << "White's turn";
	else
		cout << "Red's turn";
}

void fillBarOut(const board_t board) {
	int y;
	gotoxy(OUTX, y = 3); 
	for (int i = 0; i < board.board_numbers[REDOUT] && i < 8; i++) {
		cout << "R";
		gotoxy(OUTX, y += 1);
	}
	gotoxy(OUTX, y = 19); 
	for (int i = 0; i < board.board_numbers[WHITEOUT] && i < 8; i++) {
		cout << "W";
		gotoxy(OUTX, y -= 1);
	}
	gotoxy(BARX, y = 3); 
	for (int i = 0; i < board.board_numbers[REDBAR] && i < 8; i++) {
		cout << "R";
		gotoxy(BARX, y += 1);
	}
	gotoxy(BARX, y = 19); 
	for (int i = 0; i < board.board_numbers[WHITEBAR] && i < 8; i++) {
		cout << "W";
		gotoxy(BARX, y -= 1);
	}
}

void fillCollumn(const board_t board, int x, int y, int pawn_index, int direction) {
	gotoxy(x, y);
	if (board.board_numbers[pawn_index] != 0) {
		for (int i = 0; i < board.board_numbers[pawn_index] && i < 8; i++) {
			cout << board.board[pawn_index];
			gotoxy(x, y -= direction);
		}
	}
}

void fillBoard(const board_t board) {
	int pawn_index = 0;
	for (int x = 0; x < 12; x++) {
		if (x < 6)
			fillCollumn(board, 54 - (x * 4), 19, pawn_index, UP);
		else
			fillCollumn(board, 48 - (x * 4), 19, pawn_index, UP);
		pawn_index += 1;
	}

	for (int x = 0; x < 12; x++) {
		if (x < 6) {
			fillCollumn(board, 4 + (x * 4), 3, pawn_index, DOWN);
		}
		else {
			fillCollumn(board, 10 + (x * 4), 3, pawn_index, DOWN);
		}
		pawn_index += 1;
	}

	fillBarOut(board);
}

void drawBoard(const board_t board) {
	clrscr();
	gotoxy(1, 1);
	clrscr();
	cout <<"  13  14  15  16  17  18   BAR  19  20  21  22  23  24   OUT \n";
	cout << "||-----------------------||---||-----------------------||---\n";
	for (int i = 0; i < 8; i++)
		cout <<"||   |   |   |   |   |   ||   ||   |   |   |   |   |   ||\n";
	cout << "============================================================\n";
	for (int i = 0; i < 8; i++)
		cout <<"||   |   |   |   |   |   ||   ||   |   |   |   |   |   ||\n";
	cout << "||-----------------------||---||-----------------------||---\n";
	cout <<"  12  11  10   9   8   7   BAR   6   5   4   3   2   1   OUT \n";

	fillBoard(board);
}

char* moveInput(const char* placeholder, int x, int y) {
	gotoxy(x, y);
	cout << placeholder;
	_setcursortype(_NORMALCURSOR);
	int i = 0;
	char ch[INPUTLENGTH];
	while ((ch[i] = toupper(getch())) != '\r') {
		if (ch[i] == '\b') {
			if (i > 0) {
				cout << '\b' << ' ' << '\b';
				i--;
			}
		}
		else {
			cout << ch[i];
			i++;
		}
	}
	ch[i] = '\0';
	return ch;
}

int validateBearOff(board_t board, char player) {
	if (player == 'R') {
		if (board.board[REDBAR] > 0)
			return 0;
		for (int i = 6; i < 24; i++) {
			if (board.board[i] == player)
				return 0;
		}
	}
	else {
		if (board.board[WHITEBAR] > 0)
			return 0;
		for (int i = 17; i > 0; i--) {
			if (board.board[i] == player)
				return 0;
		}
	}
	return 1;
}

int validateDice(board_t board, int move[2], char player, int dice) {
	if (player == 'R') {
		if (move[0] == REDBAR)
			if (dice == 1 + move[1])
				return 1;
		if (dice == (move[0] - move[1])) {
			return 1;
		}
	}
	else {
		if (move[0] == WHITEBAR)
			if (dice == 24 - move[1])
				return 1;
		if (dice == (-1)*(move[0] - move[1])) {
			return 1;
		}
	}
	return 0;
}

int validatePos(board_t board, int move[2], char player) {
	if (board.board[move[0]] == board.board[move[1]] || board.board_numbers[move[1]] == 0) 
		return NORMAL;

	else if (board.board[move[0]] != board.board[move[1]]) {
		if (board.board_numbers[move[1]] == 1) {
			return HIT;
		}
	}

	else if (move[1] == REDOUT || move[1] == WHITEOUT) {
		if(validateBearOff(board, player))
			return BEAROFF;
	}
	return 0;
}

void makeMove(board_t &board, int move[2], char player, int type) {
	board.board_numbers[move[0]] -= 1;
	if (board.board_numbers[move[0]] == 0)
		board.board[move[0]] = '0';
	if (type == NORMAL) {
		board.board_numbers[move[1]] += 1;
		if (board.board_numbers[move[1]] == 1)
			board.board[move[1]] = player;
	}
	if (type == HIT) {
		board.board[move[1]] = player;
		if (player == 'W')
			board.board_numbers[REDBAR] += 1;
		if(player == 'R')
			board.board_numbers[WHITEBAR] += 1;
	}
	if (type == BEAROFF) {
		if (player == 'W')
			board.board_numbers[WHITEOUT] += 1;
		if (player == 'R')
			board.board_numbers[REDOUT] += 1;
	}
}

int* validateMove(board_t &board, int move[2], char player, int dice[4]) {
	if (board.board[move[0]] != player)
		return dice;

	int type = validatePos(board, move, player);
	if(type != 0){
		for (int i = 0; i < 4; i++) {
			if (validateDice(board, move, player, dice[i])) {
				dice[i] = 0;
				makeMove(board, move, player, type);
				return dice;
			}
		}
		for (int i = 2; i <=4 ; i++) {
			if (validateDice(board, move, player, diceSum(dice, i))) {
				for (int j = 0; j < i; j++) {
					dice[j] = 0;
				}
				makeMove(board, move, player, type);
				return dice;
			}
		}
	}
	return dice;
}

void diceRolling(int *dice) {
	gotoxy(1, 23);
	cout << "Press [R] to roll the dice\n";
	cout << "Your Roll: ";
	int i = 0;
	while (i != 2) {
		if (toupper(getch()) == 'R') {
			dice[i] = diceRoll();
			cout << dice[i] << " ";
			i++;
		}
	}
	
	if (dice[0] == dice[1]) {
		dice[2] = dice[0];
		dice[3] = dice[0];
	}
}

int checkInputs(board_t board, char ch[INPUTLENGTH], char player) {
	int move = 0; 
	if (strcmp(ch, "BAR") == 0) {
		if (player == 'R')
			return REDBAR;
		else
			return WHITEBAR;
	}
	else if (strcmp(ch, "OUT") == 0) {
		if (player == 'R')
			return REDOUT;
		else
			return WHITEOUT;
	}
	return move;
}

void diceRolls(int dice[4]) {
	gotoxy(1, 23);
	cout << "Your Roll: ";
	for (int i = 0; i < 4; i++)
		cout << dice[i] << " ";
}

int checkBar(board_t board, char player, char pawn_to_move[INPUTLENGTH]) {
	if (player = 'R' && board.board_numbers[REDBAR] > 0) {
		if (strcmp(pawn_to_move, "BAR") == 0) {
			return 1;
		}
	}
	else if (player = 'W' && board.board_numbers[WHITEBAR] > 0) {
		if (strcmp(pawn_to_move, "BAR") == 0) {
			return 1;
		}
	}
	else if ((player = 'R' && board.board_numbers[REDBAR] == 0) || (player = 'W' && board.board_numbers[WHITEBAR] == 0))
		return 1;
	return 0;
}

void drawScreen(board_t board, char player, int dice[4]) {
	drawBoard(board);
	activePlayer(player);
	diceRolls(dice);
}

void game(board_t &board, char player) {
	char pawn_to_move[INPUTLENGTH], pos_to_move[INPUTLENGTH];
	int dice[] = { 0,0,0,0 };
	diceRolling(dice);
	int temp;

	do {
		drawScreen(board, player, dice);
		strcpy(pawn_to_move, moveInput("Pawn: ", 1, 24));
		strcpy(pos_to_move, moveInput("Move to: ", 1, 25));
		if (checkBar(board, player, pawn_to_move)) {
			int move[2] = { atoi(pawn_to_move) - 1, atoi(pos_to_move) - 1 };
			if (temp = checkInputs(board, pawn_to_move, player)) {
				move[0] = temp;
			}
			if (temp = checkInputs(board, pos_to_move, player)) {
				move[1] = temp;
			}
			*dice = *validateMove(board, move, player, dice);
		}
	} while (diceSum(dice, 4) > 0 );
}

int main() {
	_setcursortype(_NOCURSOR);
	srand((int)time(NULL));
	board_t board;
	char player;
	int round = 0;

	while(true){
		if (round % 2 == 1)
			player = 'R';
		else
			player = 'W';

		drawBoard(board);
		activePlayer(player);
		game(board, player);
		round++;
	}
}
