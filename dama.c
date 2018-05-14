#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
//constants
#define row 8
#define col 8
#define b_square 'e'
#define w_square ' '
#define b_piece 'b'
#define w_piece 'w'
#define p_move 'p'
#define p1 1
#define p2 2

#define up 'w'
#define down 's'
#define left 'a'
#define right 'd'
#define mark ' '
#define resign 'r'
#define call_draw 'c'
#define quit 'q'
#define new_game 'n'
#define s_mode '1'
#define r_mode '2'

//fxns
void initializeBoard();
void printMenu();
void printBoard(char board[row][col]);
void findReqMoves(char board[row][col]);
void copyBoard(char dest[row][col], char src[row][col]);
void findCursor(char dummy[row][col]);
void findPossibleMoves();
void executeMove(char keypress, int marked);
void startGame();

//global variables
char board[row][col];
char dummy1[row][col];
char dummy2[row][col];
int mode;
int move, next_turn;
int win_flag = 0;
int cur_row, cur_col;
int marked_row, marked_col;
int req_flag = 0;
int req_row, req_col;

int main()
{

	initializeBoard();

	startGame();

	return 0;
}

void initializeBoard(){
	int index = 0;
	//creates the initial board
	for(int i=0; i<row; i++){
		for (int j=0; j<col; j++){
			if(i<=2){
				if(index%2 == 1) board[i][j] = b_piece;
				else board[i][j] = w_square;	
			}else if(i>=5){
				if(index%2 == 1) board[i][j] = w_piece;
				else board[i][j] = w_square;	
			}else{
				if(index%2 == 1) board[i][j] = b_square;
				else board[i][j] = w_square;
			}
			index++;
		}
		index++;
	}
}
void printMenu(){
	printf("[1] Standard Mode\n[2] Reversi Mode\n[q] Quit\n--:");
}
void printBoard(char board[row][col]){
	printf("\n");
	for (int i=0; i<row; i++){
		for (int j=0; j<col; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	printf("Player %d: ", move);
}
void findReqMoves(char board[row][col]){

}
void copyBoard(char dest[row][col], char src[row][col]){
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			dest[i][j] = src[i][j];
		}
	}
}
void findCursor(char dummy[row][col]){ //changes the style where the cursor is currently
	switch(dummy[cur_row][cur_col]){
			case b_square:
				dummy[cur_row][cur_col] = 'E';
				break;
			case w_square:
				dummy[cur_row][cur_col] = '.';
				break;
			case b_piece:
				dummy[cur_row][cur_col] = 'B';
				break;
			case w_piece:
				dummy[cur_row][cur_col] = 'W';
				break;
			case p_move:
				dummy[cur_row][cur_col] = 'P';
				break;
			default:
				printf("\n");
		}
}
int findPossibleMovesP1(){ //checks the possible moves of player 1's marked piece and marks it as 'p' in the map
	int possible = 0;
	if(cur_row>=0 && cur_col>0){
		switch(board[cur_row-1][cur_col-1]){
			case b_square: //forward move
				dummy1[cur_row-1][cur_col-1] = p_move;
				possible = -1;
				break;
			case b_piece: //forward capture
				if(board[cur_row-2][cur_col-2] == b_square){
					dummy1[cur_row-2][cur_col-2] = p_move;
					possible = 2;
				}
				break;
			default:
				break;
		}
		switch(board[cur_row+1][cur_col-1]){ //backward capture
			case b_piece:
				if(board[cur_row+2][cur_col-2] == b_square){
					dummy1[cur_row+2][cur_col-2] = p_move;
					if(possible = -1) dummy1[cur_row-1][cur_col-1] = 'e';
					possible = 2;
				}
				break;
			default:
				break;
		}
	}
	if(cur_row>=0 && cur_col<7){
		switch(board[cur_row-1][cur_col+1]){
			case b_square: //forward move
				if(possible < 2){
					dummy1[cur_row-1][cur_col+1] = p_move;
					if(possible == -1) possible = -3;
					else possible = -2;
				}
				break;
			case b_piece: //forward capture
				if(board[cur_row-2][cur_col+2] == b_square) {
					dummy1[cur_row-2][cur_col+2] = p_move;
					if(possible = -1) dummy1[cur_row-1][cur_col-1] = 'e';
					possible = 2;
				}
				break;
			default:
				break;
		}
		switch(board[cur_row+1][cur_col+1]){ //backward capture
			case b_piece:
				if(board[cur_row+2][cur_col+2] == b_square){
					dummy1[cur_row+2][cur_col+2] = p_move;
					if(possible = -1) dummy1[cur_row-1][cur_col-1] = 'e';
					else if(possible = -2) dummy1[cur_row-1][cur_col+1] = 'e';
					else if(possible = -3) {
						dummy1[cur_row-1][cur_col-1] = 'e';
						dummy1[cur_row-1][cur_col+1] = 'e';
					}
					possible = 2;
				}
			default:
				break;
		}
	}
	return possible;
}
int findPossibleMovesP2(){ //checks the possible moves of player 2's marked piece and marks it as 'p' in the map
	int possible = 0;
	if(cur_row>=0 && cur_col>0){
		switch(board[cur_row+1][cur_col-1]){
			case b_square: //forward move
				dummy1[cur_row+1][cur_col-1] = p_move;
				possible = -1;
				break;
			case w_piece: //forward capture
				if(board[cur_row+2][cur_col-2] == b_square){
					dummy1[cur_row+2][cur_col-2] = p_move;
					possible = 2;
				}
				break;
			default:
				break;
		}
		switch(board[cur_row-1][cur_col-1]){ //backward capture
			case w_piece:
				if(board[cur_row-2][cur_col-2] == b_square){
					dummy1[cur_row-2][cur_col-2] = p_move;
					if(possible = -1) dummy1[cur_row+1][cur_col-1] = 'e';
					possible = 2;
				}
				break;
			default:
				break;
		}
	}
	if(cur_row>=0 && cur_col<7){
		switch(board[cur_row+1][cur_col+1]){
			case b_square: //forward move
				if(possible < 2){
					dummy1[cur_row+1][cur_col+1] = p_move;
					if(possible == -1) possible = -3;
					else possible = -2;
				}
				break;
			case w_piece: //forward capture
				if(board[cur_row+2][cur_col+2] == b_square){
					dummy1[cur_row+2][cur_col+2] = p_move;
					if(possible = -1) dummy1[cur_row+1][cur_col-1] = 'e';
					possible = 2;
				}
			default:
				break;
		}
		switch(board[cur_row-1][cur_col+1]){ //backward capture
			case w_piece:
				if(board[cur_row-2][cur_col+2] == b_square){
					dummy1[cur_row-2][cur_col+2] = p_move;
					if(possible = -1) dummy1[cur_row+1][cur_col-1] = 'e';
					else if(possible = -2) dummy1[cur_row+1][cur_col+1] = 'e';
					else if(possible = -3) {
						dummy1[cur_row+1][cur_col-1] = 'e';
						dummy1[cur_row+1][cur_col+1] = 'e';
					}
					possible = 2;
				}
			default:
				break;
		}
	}
	return possible;
}
void executeMove(char keypress, int marked){ // executes the move given by the keypress of the player
	switch(keypress){
		case mark: 
			if(marked == 0){ 
				/*
					If the cursor is a piece of the current player, it will be marked:
						- findPossibleMoves() checks if there is an available move for the current marked piece
						- if there is any, the map will be updated and it will show the places your piece can move to
							where you can mark it again to execute the final move, then the turn will switch to the other player
						- else, the piece will be unmarked and the player can move again
					else, nothing will happen
				*/
				if(move == p1){ 
					if(board[cur_row][cur_col] == w_piece){
						if(findPossibleMovesP1(cur_row, cur_col) != 0){
							marked_row = cur_row;
							marked_col = cur_col;
							do{
								copyBoard(dummy2, dummy1);
								findCursor(dummy2);
								printBoard(dummy2);
								keypress = (char) getchar();
								getchar();
								executeMove(keypress, 1);
							}while(keypress != mark);
						}else{}
					}else{}
				}else if(move == p2){
					if(board[cur_row][cur_col] == b_piece){
						if(findPossibleMovesP2(cur_row, cur_col) != 0){
							marked_row = cur_row;
							marked_col = cur_col;
							do{
								copyBoard(dummy2, dummy1);
								findCursor(dummy2);
								printBoard(dummy2);
								keypress = (char) getchar();
								getchar();
								executeMove(keypress, 1);
							}while(keypress != mark);
						}else{}
					}else{}
				}else{}
			}else if(marked == 1){
				/*
					If the move is valid, the move will be executed and the board will be updated
					else, the currently marked piece will be unmarked
				*/
				if(dummy1[cur_row][cur_col] == p_move){
					if(move == p1){
						board[cur_row][cur_col] = w_piece;
						board[marked_row][marked_col] = b_square;
						//check if capture is made
						if(cur_row == (marked_row-2) && cur_col == (marked_col-2)) board[marked_row-1][marked_col-1] = b_square;
						else if(cur_row == (marked_row-2) && cur_col == (marked_col+2)) board[marked_row-1][marked_col+1] = b_square;
						else if(cur_row == (marked_row+2) && cur_col == (marked_col-2)) board[marked_row+1][marked_col-1] = b_square;
						else if(cur_row == (marked_row+2) && cur_col == (marked_col+2)) board[marked_row+1][marked_col+1] = b_square;
						move = p2;
						next_turn = 1;
					}else{
						board[cur_row][cur_col] = b_piece;
						board[marked_row][marked_col] = b_square;
						//check if capture is made
						if(cur_row == (marked_row+2) && cur_col == (marked_col-2)) board[marked_row+1][marked_col-1] = b_square;
						else if(cur_row == (marked_row+2) && cur_col == (marked_col+2)) board[marked_row+1][marked_col+1] = b_square;
						else if(cur_row == (marked_row-2) && cur_col == (marked_col-2)) board[marked_row-1][marked_col-1] = b_square;
						else if(cur_row == (marked_row-2) && cur_col == (marked_col+2)) board[marked_row-1][marked_col+1] = b_square;
						move = p1;
						next_turn = 1;
					}
				}
			}
			break;
		case up: //move the cursor up
			if(cur_row > 0) cur_row-=1;
			break;
		case down: //move the cursor down
			if(cur_row < 7) cur_row+=1;
			break;
		case left: //move the cursor left
			if(cur_col > 0) cur_col-=1;
			break;
		case right: //move the cursor right
			if(cur_col < 7) cur_col+=1;
			break;
		case resign: //resign
			printf("Player %d resigned\n", move);
			break;
		case call_draw: //calls draw
			printf("Player %d called draw.\n", move);
			break;
		default:
			printf("\n");
	}
}
void startGame(){
	char keypress;
	do{
		printMenu();
		move = p1;
		keypress = (char) getchar();
		getchar();
		if(keypress == s_mode || keypress == r_mode){
			mode = keypress;
			do{
				if(move == p1){
					cur_row = 7;
				}else{
					cur_row = 0;
				}
				cur_col = 0;
				next_turn = 0;
				req_flag = 0;
				findReqMoves(dummy1);
				do{
					copyBoard(dummy1, board);
					findCursor(dummy1);
					printBoard(dummy1);
					keypress = (char) getchar();
					getchar();
					executeMove(keypress, 0);
				}while(next_turn == 0 && keypress != resign);
			}while(keypress != resign);
		}
	}while(keypress != quit);
}