#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
//constants
#define rows 8
#define cols 8
#define b_square 'e'
#define w_square ' '
#define b_piece 'b'
#define w_piece 'w'
#define req_piece 'r'
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
void printBoard(char board[rows][cols]);
void findReqMoves(char board[rows][cols]);
int checkCapture(char board[rows][cols], int row, int col, char opp_piece);
void copyBoard(char dest[rows][cols], char src[rows][cols]);
void findCursor(char dummy[rows][cols]);
void findPossibleMoves();
void executeMove(char keypress, int marked);
void startGame();

//global variables
char board[rows][cols];
char dummy1[rows][cols];
char dummy2[rows][cols];
int mode;
int move, next_turn;
int win_flag = 0;
int cur_row, cur_col;
int marked_row, marked_col;
int req_flag = 0;

int main()
{

	initializeBoard();

	startGame();

	return 0;
}

void initializeBoard(){
	int index = 0;
	//creates the initial board
	for(int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
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
void printBoard(char board[rows][cols]){
	printf("\n");
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	printf("Player %d: ", move);
}
void findReqMoves(char board[rows][cols]){
	char piece, opp_piece;
	if(move == p1) {
		piece = w_piece;
		opp_piece = b_piece;
	}
	else {
		piece = b_piece;
		opp_piece = w_piece;
	}

	req_flag = 0;
	int captures = 0;
	int max_captures = 1;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			if(board[i][j] == piece){
				captures = checkCapture(board, i, j, opp_piece);

				if(captures > max_captures){ //resets all the r because there is a more priority move (more captures than the other)
					
					copyBoard(dummy1, board);
					max_captures = captures;
				}
				if(captures == max_captures){
					
					dummy1[i][j] = req_piece;
					req_flag = 1;
					
				}
			}
		}
	}

}
int checkCapture(char board[rows][cols], int row, int col, char opp_piece){ //recursive function that checks if there is/are capture/captures available for a piece.
	int captureUL = 0, captureUR = 0, captureLR = 0, captureLL = 0, max = 0;
	char UL[rows][cols], UR[rows][cols], LR[rows][cols], LL[rows][cols];
	copyBoard(UL, board);
	copyBoard(UR, board);
	copyBoard(LR, board);
	copyBoard(LL, board);

	if(row>1 && col>1){ //check upper left capture
		if(board[row-1][col-1] == opp_piece && board[row-2][col-2] == b_square){
			//imitates the capture move to the dummy board
			UL[row][col] = b_square;
			UL[row-1][col-1] = b_square;
			captureUL = checkCapture(UL, row-2, col-2, opp_piece) + 1;
		}
	}
	if(row>1 && col<6){ //check upper right capture
		if(board[row-1][col+1] == opp_piece && board[row-2][col+2] == b_square){
			//imitates the capture move to the dummy board
			UR[row][col] = b_square;
			UR[row-1][col+1] = b_square;
			captureUR = checkCapture(UR, row-2, col+2, opp_piece) + 1;
		}
	}
	if(row<6 && col>1){ //check lower left capture
		if(board[row+1][col-1] == opp_piece && board[row+2][col-2] == b_square){
			//imitates the capture move to the dummy board
			LR[row][col] = b_square;
			LR[row+1][col-1] = b_square;
			captureLR = checkCapture(LR, row+2, col-2, opp_piece) + 1;
		}
	}
	if(row<6 && col<6){ //check lower right capture
		if(board[row+1][col+1] == opp_piece && board[row+2][col+2] == b_square){
			//imitates the capture move to the dummy board
			LL[row][col] = b_square;
			LL[row+1][col+1] = b_square;
			captureLL = checkCapture(LL, row+2, col+2, opp_piece) + 1;	
		}
	}
	if(captureUL > max) max = captureUL;
	if(captureUR > max) max = captureUR;
	if(captureLR > max) max = captureLR;
	if(captureLL > max) max = captureLL;

	return max;
}
void copyBoard(char dest[rows][cols], char src[rows][cols]){
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			dest[i][j] = src[i][j];
		}
	}
}
void findCursor(char dummy[rows][cols]){ //changes the style where the cursor is currently
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
			case req_piece:
				dummy[cur_row][cur_col] = 'R';
				break;
			default:
				printf("\n");
		}
}
int findPossibleMovesP1(){ //checks the possible moves of player 1's marked piece and marks it as 'p' in the map
	int possible = 0;
	int row = cur_row;
	int col = cur_col;
	if(checkCapture(board, row, col, b_piece) > 0){
		possible = 1;
		if(row>1 && col>1){ //check upper left capture
			if(board[row-1][col-1] == b_piece && board[row-2][col-2] == b_square) dummy1[row-2][col-2] = p_move;
		}
		if(row>1 && col<6){ //check upper right capture
			if(board[row-1][col+1] == b_piece && board[row-2][col+2] == b_square) dummy1[row-2][col+2] = p_move;
		}
		if(row<6 && col>1){ //check lower left capture
			if(board[row+1][col-1] == b_piece && board[row+2][col-2] == b_square) dummy1[row+2][col-2] = p_move;
		}
		if(row<6 && col<6){ //check lower right capture
			if(board[row+1][col+1] == b_piece && board[row+2][col+2] == b_square) dummy1[row+2][col+2] = p_move;
		}
	}else{
		if(row>0 && col>0){ //check forward left move
			if(board[row-1][col-1] == b_square) dummy1[row-1][col-1] = p_move;
			possible = 1;
		}
		if(row>0 && col<7){ //check forward right move
			if(board[row-1][col+1] == b_square) dummy1[row-1][col+1] = p_move;
			possible = 1;
		}
	}
	return possible;
}
int findPossibleMovesP2(){ //checks the possible moves of player 2's marked piece and marks it as 'p' in the map
	int possible = 0;
	int row = cur_row;
	int col = cur_col;
	if(checkCapture(board, row, col, w_piece) > 0){
		possible = 1;
		if(row>1 && col>1){ //check upper left capture
			if(board[row-1][col-1] == w_piece && board[row-2][col-2] == b_square) dummy1[row-2][col-2] = p_move;
		}
		if(row>1 && col<6){ //check upper right capture
			if(board[row-1][col+1] == w_piece && board[row-2][col+2] == b_square) dummy1[row-2][col+2] = p_move;
		}
		if(row<6 && col>1){ //check lower left capture
			if(board[row+1][col-1] == w_piece && board[row+2][col-2] == b_square) dummy1[row+2][col-2] = p_move;
		}
		if(row<6 && col<6){ //check lower right capture
			if(board[row+1][col+1] == w_piece && board[row+2][col+2] == b_square) dummy1[row+2][col+2] = p_move;
		}
	}else{
		if(row<7 && col>0){ //check forward left move
			if(board[row+1][col-1] == b_square) dummy1[row+1][col-1] = p_move;
			possible = 1;
		}
		if(row<7 && col<7){ //check forward right move
			if(board[row+1][col+1] == b_square) dummy1[row+1][col+1] = p_move;
			possible = 1;
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
					if((board[cur_row][cur_col] == w_piece && req_flag == 0) || (dummy1[cur_row][cur_col] == 'R' && req_flag == 1)){
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
					if((board[cur_row][cur_col] == b_piece && req_flag == 0) || (dummy1[cur_row][cur_col] == 'R' && req_flag == 1)){
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
		initializeBoard();
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
				do{
					copyBoard(dummy1, board);
					findReqMoves(board);
					findCursor(dummy1);
					printBoard(dummy1);
					keypress = (char) getchar();
					getchar();
					executeMove(keypress, 0);
					if(keypress == call_draw){
						printf("Opponent's answer: ");
						keypress = (char) getchar();
						getchar();
						if(keypress == call_draw){
							printf("Draw!\n");
							keypress = resign;
						}
					}
				}while(next_turn == 0 && keypress != resign);
			}while(keypress != resign);
		}
	}while(keypress != quit);
}