#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "conio2.h"
using namespace std;

#define WINDOWWIDTH 120
#define WINDOWHEIGHT 30
#define UP 1
#define DOWN -1
#define BARX 29
#define OUTX 59 
#define INPUTL 16
#define NORMAL 1
#define HIT 2
#define BEAROFF 3
#define REDBAR 24
#define WHITEBAR 25
#define REDOUT 26
#define WHITEOUT 27
#define END 1

struct board_t {
	char board[29] = "R0000W0W000RW000R0R0000WRWRW"; // pola 1-24 // barRW // out RW
	int board_numbers[28] = { 2,0,0,0,0,5,0,3,0,0,0,5,5,0,0,0,3,0,5,0,0,0,0,2,0,0,0,0 };
};

int diceRoll();
int diceSum(int dice[4], int n);
int diceBattle();
void activePlayer(char player);
void fillBarOut(const board_t board);
void fillCollumn(const board_t board, int x, int y, int pawn_index, int direction);
void fillBoard(const board_t board);
void drawBoard(const board_t board);
char* textInput(const char* placeholder, int x, int y);
int validateBearOff(board_t board, char player);
int validateDice(board_t board, int move[2], char player, int dice);
int validatePos(board_t board, int move[2], char player);
void makeMove(board_t& board, int move[2], char player, int type);
int validateMove(board_t& board, int move[2], char player, int dice[4], int test = 0);
void diceRolling(int* dice);
int checkInputs(char ch[INPUTL], char player);
void diceRolls(int dice[4]);
int checkBar(board_t board, char player, char pawn_to_move[INPUTL]);
int barMoveAvailable(board_t board, char player, const int cdice[4]);
int normalMoveAvailable(board_t board, char player, const int cdice[4]);
int anyMoveAvailable(board_t board, char player, const int cdice[4]);
void drawScreen(board_t board, char player, int dice[4]);
void saveMove(char* filename, char pawn_to_move[INPUTL], char pos_to_move[INPUTL], int end = 0);
int loadedGameMenu(int r);
void readMoves(char moves[100], int allmove[8], char ps);
int loadedGame(board_t& board, char ps, char moves[100], int r, int back = 0);
void moveAvailable(board_t& board, char pawn_to_move[INPUTL], char pos_to_move[INPUTL], char player, int dice[4], char* fname);
int game(board_t &board, char player, char* fname);
int gameLoop(board_t board, char p, char p1, int r, char fname[INPUTL], char p1Name[INPUTL], char p2Name[INPUTL]);
int fileCheck(FILE* file);
void previousMove(board_t &board, int r, char ps, char p1, char p2, char moves[100], FILE* file);
void skipToEnd(board_t board, int r, char ps, char p1, char p2, char moves[100], FILE* file);
void loadGameLoop(board_t &board, int r, char ps, char p1, char p2, char moves[100], FILE* file, char fname[INPUTL], char p1N[INPUTL], char p2N[INPUTL]);
void loadGame(board_t board, char fname[INPUTL], int r = 0, int back = 0, int skip = 0);
void hallOfFame();
void createFile(char fname[INPUTL]);
void newGame();
void drawTitleScreen();
void menu();

//52
int diceRoll() {
	int dice;
	dice = (rand() % 6) + 1;
	return dice;
}

//82
int diceSum(int dice[4], int n) {
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum += dice[i];
	}
	return sum;
}

//490
int diceBattle() {
	int dice[2] = {};
	int x = 25;
	int y = 19;
	do {
		gotoxy(x, y);
		cout << "Press [R] to roll the dice - Person with higher number starts\n";
	} while (toupper(getch()) != 'R');
	dice[0] = diceRoll();
	gotoxy(x, y + 1);
	cout << "Red Roll:   " << dice[0];
	do {
		gotoxy(x, y);
		cout << "Press [R] to roll the dice - Person with higher number starts\n";
	} while (toupper(getch()) != 'R');
	dice[1] = diceRoll();
	gotoxy(x, y + 2);
	cout << "White Roll: " << dice[1];

	gotoxy(x, y + 3);
	if (dice[0] < dice[1]) {
		cout << "White starts";
		getch();
		return 0;
	}
	else {
		cout << "Red starts";
		getch();
		return 1;
	}
}

//101
void activePlayer(char player) {
	gotoxy(64, 11);
	if (player == 'W')
		cout << "White's turn";
	else
		cout << "Red's turn";
}

//523
void fillBarOut(const board_t board) {
	int y;
	gotoxy(OUTX, y = 3);
	for (int i = 0; i < board.board_numbers[REDOUT] && i < 8; i++) {
		textcolor(RED);
		cout << "R";
		gotoxy(OUTX, y += 1);
	}
	gotoxy(OUTX, y = 19);
	for (int i = 0; i < board.board_numbers[WHITEOUT] && i < 8; i++) {
		textcolor(LIGHTBLUE);
		cout << "W";
		gotoxy(OUTX, y -= 1);
	}
	gotoxy(BARX, y = 3);
	for (int i = 0; i < board.board_numbers[REDBAR] && i < 8; i++) {
		textcolor(RED);
		cout << "R";
		gotoxy(BARX, y += 1);
	}
	gotoxy(BARX, y = 19);
	for (int i = 0; i < board.board_numbers[WHITEBAR] && i < 8; i++) {
		textcolor(LIGHTBLUE);
		cout << "W";
		gotoxy(BARX, y -= 1);
	}
	textcolor(WHITE);
}

//351
void fillCollumn(const board_t board, int x, int y, int pawn_index, int direction) {
	gotoxy(x, y);
	if (board.board_numbers[pawn_index] != 0) {
		for (int i = 0; i < board.board_numbers[pawn_index] && i < 8; i++) {
			if(board.board[pawn_index]=='R')
				textcolor(RED);
			if(board.board[pawn_index]=='W')
				textcolor(LIGHTBLUE);
			cout << board.board[pawn_index];
			gotoxy(x, y -= direction);
		}
	}
	textcolor(WHITE);
}

//347
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

void drawboard1() {
	cout << "  13  14  15  16  17  18   BAR  19  20  21  22  23  24   OUT \n";
	cout << "||-----------------------||---||-----------------------||---\n";
	for (int i = 0; i < 8; i++)
		cout << "||   |   |   |   |   |   ||   ||   |   |   |   |   |   ||\n";
	cout << "============================================================\n";
}

//463
void drawBoard(const board_t board) {
	clrscr();
	gotoxy(1, 1);
	clrscr();
	drawboard1();
	for (int i = 0; i < 8; i++)
		cout <<"||   |   |   |   |   |   ||   ||   |   |   |   |   |   ||\n";
	cout << "||-----------------------||---||-----------------------||---\n";
	cout <<"  12  11  10   9   8   7   BAR   6   5   4   3   2   1   OUT \n";

	fillBoard(board);
}

//289
char* textInput(const char* placeholder, int x, int y) {
	gotoxy(x, y);
	cout << placeholder;
	_setcursortype(_NORMALCURSOR);
	int i = 0;
	char ch[INPUTL];
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
	_setcursortype(_NOCURSOR);
	return ch;
}

//273
int validateBearOff(board_t board, char player){
	if (player == 'R') {
		if (board.board_numbers[REDBAR] > 0)
			return 0;
		for (int i = 17; i >= 0; i--) {
			if (board.board[i] == player)
				return 0;
		}
	}
	else {
		if (board.board_numbers[WHITEBAR] > 0)
			return 0;
		for (int i = 6; i < 24; i++) {
			if (board.board[i] == player)
				return 0;
		}
	}
	return 1;
}

//359
int validateDice(board_t board, int move[2], char player, int dice) {
	if (player == 'R') {
		if (move[0] == REDBAR)
			if (dice == move[1]+1)
				return 1;
		if (move[1] == REDOUT)
			if (dice + move[0] > 23)
				return 1;
		if (dice == (-1) * (move[0] - move[1])) {
			return 1;
		}
	}
	else {
		if (move[0] == WHITEBAR)
			if (dice == 24 - move[1])
				return 1;
		if (move[1] == WHITEOUT)
			if (dice - move[0] > 0)
				return 1;
		if (dice == (move[0] - move[1])) {
			return 1;
		}
	}
	return 0;
}

//381
int validatePos(board_t board, int move[2], char player) {
	if ((move[1] < 0) || (move[1] > 27))
		return 0;
	if (board.board[move[0]] == board.board[move[1]] || board.board_numbers[move[1]] == 0) {
		if ((move[1] == REDOUT) || (move[1] == WHITEOUT)) {
			if (validateBearOff(board, player))
				return BEAROFF;
		}
		return NORMAL;
	}

	else if (board.board[move[0]] != board.board[move[1]]) {
		if (board.board_numbers[move[1]] == 1) {
			return HIT;
		}
	}

	return 0;
}

//530
void makeMove(board_t &board, int move[2], char player, int type) {
	board.board_numbers[move[0]] -= 1;
	if ((board.board_numbers[move[0]] == 0) && (move[0]<24))
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

//461
int validateMove(board_t &board, int move[2], char player, int dice[4], int test) {
	if (board.board[move[0]] != player)
		return 0;

	int type = validatePos(board, move, player);
	if(type != 0){
		for (int i = 0; i < 4; i++) {

			if (validateDice(board, move, player, dice[i])) {
				dice[i] = 0;
				if(test == 0)
					makeMove(board, move, player, type);
				return 1;
			}
		}
		for (int i = 2; i <=4 ; i++) {
			if (validateDice(board, move, player, diceSum(dice, i))) {
				for (int j = 0; j < i; j++) {
					dice[j] = 0;
				}
				if (test == 0)
					makeMove(board, move, player, type);
				return 1;
			}
		}
	}
	return 0;
}

//232
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

//212
int checkInputs(char ch[INPUTL], char player) {
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

//95
void diceRolls(int dice[4]) {
	gotoxy(1, 23);
	cout << "Your Roll: ";
	for (int i = 0; i < 4; i++)
		cout << dice[i] << " ";
}

//369
int checkBar(board_t board, char player, char pawn_to_move[INPUTL]) {
	if (player == 'R' && board.board_numbers[REDBAR] > 0) {
		if (strcmp(pawn_to_move, "BAR") == 0) {
			return 1;
		}
	}
	else if (player == 'W' && board.board_numbers[WHITEBAR] > 0) {
		if (strcmp(pawn_to_move, "BAR") == 0) {
			return 1;
		}
	}
	else if ((player == 'R' && board.board_numbers[REDBAR] == 0) || (player == 'W' && board.board_numbers[WHITEBAR] == 0))
		return 1;
	return 0;
}

//340
int barMoveAvailable(board_t board, char player, const int cdice[4]) {
	int move[2];
	int dice[4];
	int d = (player == 'W') ? -1 : 1;
	move[0] = (player == 'W') ? WHITEBAR : REDBAR;
	
	for (int i = 0; i < 4; i++)
		dice[i] = cdice[i];

	for (int i = 0; i < 4; i++) {
		if (dice[i] > 0) {
			if (player == 'R')
				move[1] = dice[i]-1;
			else
				move[1] = 24 + d * dice[i];
			if (validateMove(board, move, player, dice, 1) == 1) {
				return 1;
			}
		}
	}
	return 0;
}

//372
int normalMoveAvailable(board_t board, char player, const int cdice[4]) {
	int move[2];
	int dice[4];
	int d = (player == 'W') ? -1 : 1;
	move[0] = (player == 'W') ? WHITEBAR : REDBAR;

	for (int i = 0; i < 4; i++)
		dice[i] = cdice[i];

	for (int i = 0; i < 24; i++) {
		move[0] = i;
		if (player == board.board[i]) {
			for (int j = 0; j < 4; j++) {
				if (dice[j] != 0) {
					move[1] = move[0] + d * dice[j];
					if (validateMove(board, move, player, dice, 1) == 1) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

//377
int anyMoveAvailable(board_t board, char player, const int cdice[4]) {
	int move = (player == 'W') ? WHITEBAR : REDBAR;
	
	if (board.board_numbers[move] > 0) {
		if (barMoveAvailable(board,player,cdice))
			return 1;
	}
	else {
		if (normalMoveAvailable(board, player, cdice))
			return 1;
	}
	
	if (validateBearOff(board, player) && (board.board_numbers[26] != 15 || board.board_numbers[27] != 15))
		return 1;
	cout << "No legal moves";
	getch();
	return 0;
}

//178
void drawScreen(board_t board, char player, int dice[4]) {
	drawBoard(board);
	if (player == 'R')
		textcolor(RED);
	else
		textcolor(LIGHTBLUE);
	activePlayer(player);
	textcolor(WHITE);
	diceRolls(dice);
}

//296
void saveMove(char* filename , char pawn_to_move[INPUTL], char pos_to_move[INPUTL], int end) {
	FILE* file;
	file = fopen(filename, "a");

	if (file == NULL) {
		cout << "Error opening file";
		return;
	}

	if (end) {
		fputs(";\n", file);
	}
	else {
		fputs(pawn_to_move, file);
		fputs("-", file);
		fputs(pos_to_move, file);
		fputs(" ", file);
	}

	fclose(file);
}

//373
int loadedGameMenu(int r) {
	int x = 2;
	gotoxy(x, 25);
	cout << "[B]ack";
	gotoxy(x += 9, 25);
	cout << "[N]ext";
	gotoxy(x += 9, 25);
	cout << "[C]ontinue";
	gotoxy(x += 13, 25);
	cout << "[R]estart";
	gotoxy(x += 12, 25);
	cout << "[S]kip";
	gotoxy(x += 9, 25);
	cout << "[Q]uit";

	while (1) {
		switch (toupper(getch())) {
		case 'B':
			if (r > 0)
				return 1;
			break;
		case 'N':
			break;
		case 'C':
			return 2;
		case 'R':
			return 3;
		case 'S':
			return 4;
		case 'Q':
			return 5;
		}
		break;
	}
	return 0;
}

//456
void readMoves(char moves[100], int allmove[8], char ps) {
	int i = 0;
	char ch[5];
	int moveIndex = 0;
	int temp;
	while (moves[i] != ';') {
		int j = i;
		strcpy(ch, "   ");
		int chIndex = 0;
		while ((moves[j] != '-') && (moves[j] != ' ')) {
			ch[chIndex] = moves[j];
			j++;
			chIndex++;
		}
		allmove[moveIndex] = atoi(ch) - 1;
		if (temp = checkInputs(ch, ps))
			allmove[moveIndex] = temp;
		moveIndex += 1;
		i = j + 1;
		strcpy(ch, "   ");
	}
}

//473
int loadedGame(board_t &board, char ps, char moves[100], int r, int back) {
	int allmove[8];
	for (int j = 0; j < 8; j++)
		allmove[j] = -1;
	int move[2];

	drawBoard(board);
	activePlayer(ps);

	int x;
	if (!back) {
		if ((x = loadedGameMenu(r))!=0)
			return x;
	}
	
	if (moves[0] == ';')
		return 0;

	readMoves(moves, allmove, ps);

	for (int a = 0; a < 8; a += 2) {
		if (allmove[a] != -1 || allmove[a + 1] != -1) {
			move[0] = allmove[a];
			move[1] = allmove[a + 1];

			int type = validatePos(board, move, ps);
			makeMove(board, move, ps, type);
		}
		move[0] = allmove[a];
		move[1] = allmove[a + 1];
	}
	return 0;
}

//506
void moveAvailable(board_t &board, char pawn_to_move[INPUTL], char pos_to_move[INPUTL], char player, int dice[4], char* fname) {
	int temp;
	strcpy(pawn_to_move, textInput("Move from: ", 1, 24));
	strcpy(pos_to_move, textInput("Move to:   ", 1, 25));
	if (checkBar(board, player, pawn_to_move)) {
		int move[2] = { atoi(pawn_to_move) - 1, atoi(pos_to_move) - 1 };
		if (temp = checkInputs(pawn_to_move, player)) {
			move[0] = temp;
		}
		if (temp = checkInputs(pos_to_move, player)) {
			move[1] = temp;
		}
		if (validateMove(board, move, player, dice))
			saveMove(fname, pawn_to_move, pos_to_move);
	}
}

//482
int game(board_t &board, char player, char* fname) { 
	char pawn_to_move[INPUTL], pos_to_move[INPUTL];
	int dice[] = { 0,0,0,0 };
	const int* cdice = dice;
	drawScreen(board, player, dice);
	diceRolling(dice);

	do {
		drawScreen(board, player, dice);
		if (board.board_numbers[REDOUT] >= 15 || board.board_numbers[WHITEOUT] >= 15)
			return END;
		if (anyMoveAvailable(board, player, cdice) == 0)
			break;
		moveAvailable(board, pawn_to_move, pos_to_move, player, dice, fname);
	} while (diceSum(dice, 4) > 0 );

	saveMove(fname, pawn_to_move, pos_to_move, 1);
	return 0;
}

//353
int gameLoop(board_t board, char p, char p1, int r, char fname[INPUTL], char p1Name[INPUTL], char p2Name[INPUTL]) {
	char win[INPUTL];
	while (true) {
		if (r % 2 == 1)
			p = 'R';
		else
			p = 'W';

		char x;
		drawBoard(board);
		gotoxy(26, 25);
		cout << "[M]ove [Q]uit";
		while (((x = toupper(getch())) != 'M') && (x != 'Q'));
		if (x == 'Q')
			return 0;

		if (game(board, p, fname) == END) {
			clrscr();
			if (p == p1)
				strcpy(win, p1Name);
			else
				strcpy(win, p2Name);
			gotoxy(50, 15);
			cout << "GAME OVER - WINNER: " << win;
			getch();
			break;
		}
		r++;

		drawBoard(board);

	}
	return 1;
}

//109
int fileCheck(FILE* file) {
	if (file == NULL) {
		clrscr();
		gotoxy(40, 15);
		cout << "File not found";
		getch();
		return 1;
	}
	return 0;
}

//185
void previousMove(board_t &board, int r, char ps, char p1, char p2, char moves[100], FILE* file) {
	for (int i = 0; (i < r); i++) {
		if (i % 2 == 0)
			ps = p1;
		else
			ps = p2;
		fgets(moves, 100, file);
		loadedGame(board, ps, moves, r, 1);
	}
}

//196
void skipToEnd(board_t board, int r, char ps, char p1, char p2, char moves[100], FILE* file) {
	while (fgets(moves, 100, file)) {
		if (r % 2 == 0)
			ps = p1;
		else
			ps = p2;
		loadedGame(board, ps, moves, r, 1);
		r++;
	}
	drawBoard(board);
	getch();
}

//532
void loadGameLoop(board_t &board, int r, char ps, char p1, char p2, char moves[100], FILE* file, char fname[INPUTL], char p1N[INPUTL], char p2N[INPUTL]) {
	board_t Nboard;
	int x;
	while (fgets(moves, 100, file)) {
		if (r % 2 == 0)
			ps = p1;
		else
			ps = p2;
		if (x = loadedGame(board, ps, moves, r, 0)) {
			switch (x) {
			case 1: //back
				fclose(file);
				board = Nboard;
				loadGame(board, fname, r - 1, 1, 0);
				return;
			case 2: //continue
				while (true) {
					gameLoop(board, ps, p1, r, fname, p1N, p2N);
					return;
				}
				return;
			case 3: //restart
				loadGame(Nboard, fname);
				return;
			case 4: //skip
				loadGame(Nboard, fname, 0, 0, 1);
				return;
			case 5: //quit
				return;
			}
		}
		r++;
	}
	drawBoard(board);
	gotoxy(26, 23);
	cout << "THE END";
	getch();
}

//526 bez parametrów
void loadGame(board_t board, char fname[INPUTL], int r, int back, int skip) {
	clrscr();
	FILE* file;
	file = fopen(fname, "r");

	if (fileCheck(file)) 
		return;

	char movs[100];
	char ps = fgets(movs, 100, file)[0]; 
	int p1 = (ps == 'W') ? 'W' : 'R';
	int p2 = (ps == 'W') ? 'R' : 'W';

	fgets(movs, 100, file);
	char p1N[INPUTL];
	char p2N[INPUTL];
	strcpy(p1N, "               ");
	strcpy(p2N, "               ");

	int i = 0, j;
	for (j=0; movs[j] != ';'; j++) {
		p1N[i++] = movs[j];
	}
	i = 0;
	for (++j; movs[j] != ';'; j++) {
		p2N[i++] = movs[j];
	}

	if (back && r!=0) 
		previousMove(board, r, ps, p1, p2, movs, file);

	if (skip) {
		skipToEnd(board, r, ps, p1, p2, movs, file);
		return;
	}

	loadGameLoop(board, r, ps, p1, p2, movs, file, fname, p1N, p2N);
	return;
}

void hallOfFame() {
	cout << "work in progress";
	getch();
}

//140
void createFile(char fname[INPUTL]) {
	clrscr();
	strcpy(fname, textInput("Savefile Name: ", 38, 14));
	FILE* file = fopen(fname, "w");
	fclose(file);
	clrscr();
}

//453
void newGame() { 
	board_t board;
	char p1Name[INPUTL], p2Name[INPUTL], win[INPUTL], fname[INPUTL], p;
	createFile(fname);
	strcpy(p1Name, textInput("P1 Name (RED):   ",38,14));
	strcpy(p2Name, textInput("P2 Name (WHITE): ",38,15));
	int r = 0;
	
	FILE* file = fopen(fname, "w");
	if (diceBattle()) {
		r = 1;
		fputs("R\n", file);
	}
	else
		fputs("W\n", file);

	fputs(p1Name, file);
	fputs(";", file);
	fputs(p2Name, file);
	fputs(";\n", file);
	fclose(file);
	
	if (r % 2 == 1)
		p = 'R';
	else
		p = 'W';

	gameLoop(board, p, p, r, fname, p1Name, p2Name);

}

void drawTitle(int x, int &y) {
	gotoxy(x, y++);
	cout << " ______ _______ ______ __  __ _______ _______ _______ _______ _______ _______";
	gotoxy(x, y++);
	cout << "|   __ \\   _   |      |  |/  |     __|   _   |   |   |   |   |       |    |  |";
	gotoxy(x, y++);
	cout << "|   __ <       |   ---|     <|    |  |       |       |       |   -   |       |";
	gotoxy(x, y++);
	cout << "|______/___|___|______|__|\\__|_______|___|___|__|_|__|__|_|__|_______|__|____|";
}

//530
void drawTitleScreen() {
	clrscr();
	int y = 7;
	int x = 21;
	gotoxy(x, y++);
	drawTitle(x, y);
	gotoxy((WINDOWWIDTH / 2) - 18, y+1);
	cout << "MADE BY: MATEUSZ GRZONKA S198023";
	gotoxy(x, y += 10);
	cout << "[N]ew Game";
	gotoxy(x += 23,y);
	cout << "[L]oad Game";
	gotoxy(x += 23, y);
	cout << "[H]all of Fame";
	gotoxy(x += 23, y);
	cout << "[Q]uit";
}

//255
void menu() {
	board_t board;
	while (1){
		drawTitleScreen();
		switch (toupper(getch())) {
			case 'N':
				newGame();
				break;
			case 'L':
				char fname[INPUTL];
				clrscr();
				strcpy(fname, textInput("Savefile name: ", 40, 15));
				loadGame(board, fname);
				break;
			case 'H':
				hallOfFame();
				break;
			case 'Q':
				return;
		}
	}
}

//119
int main() {
	_setcursortype(_NOCURSOR);
	srand((int)time(NULL));
	settitle("Backgammon Project - Mateusz Grzonka s198023");
	menu();
}
