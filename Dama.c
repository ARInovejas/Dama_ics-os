#include "../../sdk/dexsdk.h"

#define standard_mode '1'
#define reversi_mode '2'
#define help 'h'
#define new_game 'n'
#define quit 'q'
#define resign 'r'
#define call_draw 'c'
#define up 'w'
#define left 'a'
#define down 's'
#define right 'd'
#define mark ' '
#define row 8
#define col 8
#define b_square 'e'
#define w_square ' '
#define b_piece 'b'
#define w_piece 'w'
#define req_piece 'r'
#define p_move 'p'
#define p1 1
#define p2 2

void setBoardUI();
void setDummyBoardUI(char dummy[row][col], char src[row][col]);
void erase(int x, int y, int w, int h); //lights
void moveCursorUp();
void moveCursorDown();
void moveCursorLeft();
void moveCursorRight();
void drawCursor(int x, int y, int color);
void redraw(int x, int y, char board[row][col]);
void writeMenu();


void initializeBoard();
void copyBoard(char dest[row][col], char src[row][col]);
void executeMove(char keypress, int marked);
void findReqMoves(char board[row][col]);
int checkCapture(char board[row][col], int rows, int cols, char opp_piece);
int findPossibleMovesP1();
int findPossibleMovesP2();
int checkWinner();

int xCoordinates[row][col];
int yCoordinates[row][col];
char board[row][col];
int cur_col, cur_row;
int gameOver;
char dummy1[row][col];
char dummy2[row][col];
char mode;
int move, next_turn;
int marked_row, marked_col;
int req_flag = 0;

int main(){
	char keypress=' ';

	do{
		set_graphics(VGA_320X200X256);
		header();

		cur_col = 0;
		cur_row = 0;
		gameOver = 0;

		keypress = (char) getch();

		if(keypress==standard_mode || keypress==reversi_mode){
			erase(1,1,640,395);
			initializeBoard();
			setBoardUI();
			writeMenu();
			mode = keypress;
			move = p1;
			do{
				
				erase(210, 20, 100, 7);
				if(move == p1){
						cur_row = 7;
						write_text("Player 1", 210, 20, YELLOW, 0);
					}else{
						cur_row = 0;
						write_text("Player 2", 210, 20, YELLOW, 0);
					}

					cur_col = 0;
					next_turn = 0;
					setBoardUI();
					do{
						copyBoard(dummy1, board);
						findReqMoves(board);
						setDummyBoardUI(dummy1, board);
						keypress = (char) getch();
						executeMove(keypress, 0);
						if(keypress == call_draw){
							erase(210, 20, 100, 7);
							write_text("Draw?", 210, 20, YELLOW, 0);
							keypress = (char) getch();
							
						}
					}while(next_turn == 0 && !(keypress == resign || keypress == call_draw));
					gameOver = checkWinner();
				}while(!(keypress == resign || keypress == call_draw) && gameOver == 0);
				
				erase(1,1,640,395);
				if(gameOver == 1)	
					write_text("Player 1 is the winner", 100, 50, WHITE, 1);
				else if (gameOver == 2)
					write_text("Player 2 is the winner", 100, 50, WHITE, 1);
				else{
					if(keypress == resign){
						if(move == 1)	write_text("Player 1 resigned", 100, 50, WHITE, 1);
						else write_text("Player 2 resigned", 100, 50, WHITE, 1);
					}else{
						write_text("Draw", 135, 50, WHITE, 1);
					}
				}
				write_text("Press any key to continue", 80, 80, WHITE, 0);
				keypress = (char) getch();
			}
		}while(keypress!=quit);


		erase(1,1,640,395);
		set_graphics(VGA_TEXT80X25X16);
		clrscr(); 
		
	

}

int checkWinner(){
	int w_count = 0, b_count = 0;
	int i, j;
	for(i = 0; i<row; i++){
		for(j = 0; j<col; j++){
			if(board[i][j]==w_piece)	w_count++;
			else if(board[i][j]==b_piece)	b_count++;
		}
	}
	if(mode==standard_mode){
		if(w_count == 0)	return 2;
		else if(b_count == 0)	return 1;
	}
	else{
		if(w_count == 0)	return 1;
		else if(b_count == 0)	return 2;
	}
	return 0;
}

void initializeBoard(){
	int index = 0, i, j;
	//creates the initial board
	for(i=0; i<row; i++){
		for (j=0; j<col; j++){
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
	for(i = 0; i<row ; i++){
		for(j = 0; j < col; j++){
			yCoordinates[i][j] = i*25;
			xCoordinates[i][j] = j*25;
		}
	}
}
void findReqMoves(char board[row][col]){
	char piece, opp_piece;
	int i, j;
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
	for(i=0; i<row; i++){
		for(j=0; j<col; j++){
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

int checkCapture(char board[row][col], int rows, int cols, char opp_piece){ //recursive function that checks if there is/are capture/captures available for a piece.
	int captureUL = 0, captureUR = 0, captureLR = 0, captureLL = 0, max = 0;
	char UL[row][col], UR[row][col], LR[row][col], LL[row][col];
	copyBoard(UL, board);
	copyBoard(UR, board);
	copyBoard(LR, board);
	copyBoard(LL, board);

	if(rows>1 && cols>1){ //check upper left capture
		if(board[rows-1][cols-1] == opp_piece && board[rows-2][cols-2] == b_square){
			//imitates the capture move to the dummy board
			UL[rows][cols] = b_square;
			UL[rows-1][cols-1] = b_square;
			captureUL = checkCapture(UL, rows-2, cols-2, opp_piece) + 1;
		}
	}
	if(rows>1 && cols<6){ //check upper right capture
		if(board[rows-1][cols+1] == opp_piece && board[rows-2][cols+2] == b_square){
			//imitates the capture move to the dummy board
			UR[rows][cols] = b_square;
			UR[rows-1][cols+1] = b_square;
			captureUR = checkCapture(UR, rows-2, cols+2, opp_piece) + 1;
		}
	}
	if(rows<6 && cols>1){ //check lower left capture
		if(board[rows+1][cols-1] == opp_piece && board[rows+2][cols-2] == b_square){
			//imitates the capture move to the dummy board
			LR[rows][cols] = b_square;
			LR[rows+1][cols-1] = b_square;
			captureLR = checkCapture(LR, rows+2, cols-2, opp_piece) + 1;
		}
	}
	if(rows<6 && cols<6){ //check lower right capture
		if(board[rows+1][cols+1] == opp_piece && board[rows+2][cols+2] == b_square){
			//imitates the capture move to the dummy board
			LL[rows][cols] = b_square;
			LL[rows+1][cols+1] = b_square;
			captureLL = checkCapture(LL, rows+2, cols+2, opp_piece) + 1;	
		}
	}
	if(captureUL > max) max = captureUL;
	if(captureUR > max) max = captureUR;
	if(captureLR > max) max = captureLR;
	if(captureLL > max) max = captureLL;

	return max;
}

int findPossibleMovesP1(){ //checks the possible moves of player 1's marked piece and marks it as 'p' in the map
	int possible = 0;
	if(checkCapture(board, cur_row, cur_col, b_piece) > 0){
		possible = 1;
		if(cur_row>1 && cur_col>1){ //check upper left capture
			if(board[cur_row-1][cur_col-1] == b_piece && board[cur_row-2][cur_col-2] == b_square) dummy1[cur_row-2][cur_col-2] = p_move;
		}
		if(cur_row>1 && cur_col<6){ //check upper right capture
			if(board[cur_row-1][cur_col+1] == b_piece && board[cur_row-2][cur_col+2] == b_square) dummy1[cur_row-2][cur_col+2] = p_move;
		}
		if(cur_row<6 && cur_col>1){ //check lower left capture
			if(board[cur_row+1][cur_col-1] == b_piece && board[cur_row+2][cur_col-2] == b_square) dummy1[cur_row+2][cur_col-2] = p_move;
		}
		if(cur_row<6 && cur_col<6){ //check lower right capture
			if(board[cur_row+1][cur_col+1] == b_piece && board[cur_row+2][cur_col+2] == b_square) dummy1[cur_row+2][cur_col+2] = p_move;
		}
	}else{
		if(cur_row>0 && cur_col>0){ //check forward left move
			if(board[cur_row-1][cur_col-1] == b_square) dummy1[cur_row-1][cur_col-1] = p_move;
			possible = 1;
		}
		if(cur_row>0 && cur_col<7){ //check forward right move
			if(board[cur_row-1][cur_col+1] == b_square) dummy1[cur_row-1][cur_col+1] = p_move;
			possible = 1;
		}
	}
	return possible;
}
int findPossibleMovesP2(){ //checks the possible moves of player 2's marked piece and marks it as 'p' in the map
	int possible = 0;
	if(checkCapture(board, cur_row, cur_col, w_piece) > 0){
		possible = 1;
		if(cur_row>1 && cur_col>1){ //check upper left capture
			if(board[cur_row-1][cur_col-1] == w_piece && board[cur_row-2][cur_col-2] == b_square) dummy1[cur_row-2][cur_col-2] = p_move;
		}
		if(cur_row>1 && cur_col<6){ //check upper right capture
			if(board[cur_row-1][cur_col+1] == w_piece && board[cur_row-2][cur_col+2] == b_square) dummy1[cur_row-2][cur_col+2] = p_move;
		}
		if(cur_row<6 && cur_col>1){ //check lower left capture
			if(board[cur_row+1][cur_col-1] == w_piece && board[cur_row+2][cur_col-2] == b_square) dummy1[cur_row+2][cur_col-2] = p_move;
		}
		if(cur_row<6 && cur_col<6){ //check lower right capture
			if(board[cur_row+1][cur_col+1] == w_piece && board[cur_row+2][cur_col+2] == b_square) dummy1[cur_row+2][cur_col+2] = p_move;
		}
	}else{
		if(cur_row<7 && cur_col>0){ //check forward left move
			if(board[cur_row+1][cur_col-1] == b_square) dummy1[cur_row+1][cur_col-1] = p_move;
			possible = 1;
		}
		if(cur_row<7 && cur_col<7){ //check forward right move
			if(board[cur_row+1][cur_col+1] == b_square) dummy1[cur_row+1][cur_col+1] = p_move;
			possible = 1;
		}
	}
	return possible;
}
void copyBoard(char dest[cur_row][col], char src[row][col]){
	int i, j;
	for(i=0; i<row; i++){
		for(j=0; j<col; j++){
			dest[i][j] = src[i][j];
		}
	}
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
					if((board[cur_row][cur_col] == w_piece && req_flag == 0) || (dummy1[cur_row][cur_col] == req_piece && req_flag == 1)){
						if(findPossibleMovesP1(cur_row, cur_col) != 0){
							marked_row = cur_row;
							marked_col = cur_col;
							dummy1[marked_row][marked_col] -= 32;
							do{
								copyBoard(dummy2, dummy1);
								setDummyBoardUI(dummy2, board);
								keypress = (char) getch();
								executeMove(keypress, 1);
							}while(keypress != mark);
							dummy1[marked_row][marked_col] += 32;
							setDummyBoardUI(dummy1, board);
						}else{}
					}else{}
				}else if(move == p2){
					if((board[cur_row][cur_col] == b_piece && req_flag == 0) || (dummy1[cur_row][cur_col] == req_piece && req_flag == 1)){
						if(findPossibleMovesP2(cur_row, cur_col) != 0){
							marked_row = cur_row;
							marked_col = cur_col;
							dummy1[marked_row][marked_col] -= 32;
							do{
								copyBoard(dummy2, dummy1);
								setDummyBoardUI(dummy2, board);
								keypress = (char) getch();
								executeMove(keypress, 1);
							}while(keypress != mark);
							dummy1[marked_row][marked_col] += 32;
							setDummyBoardUI(dummy1, board);
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
						// setBoardUI();
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
						// setBoardUI();
						move = p1;
						next_turn = 1;
					}
				}
			}
			break;
		case up: //move the cursor up
			moveCursorUp();			
			break;
		case down: //move the cursor down
			moveCursorDown();
			break;
		case left: //move the cursor left
			moveCursorLeft();
			break;
		case right: //move the cursor right
			moveCursorRight();
			break;
		default:
			break;
		
	}
}

void drawCursor(int x, int y, int color){ //draws the cursor
	int i, j;
	x+=12.5;
	y+=12.5;
    for(j=-4; j<=4; j++)
	   	for(i=-4; i<=4; i++)
	       	if(i*i + j*j <16)
	            write_pixel(x+i, y+j, color);
}

void moveCursorUp(){
	if(cur_row==0)	return;
	redraw(cur_row, cur_col, board);
	cur_row--;
	drawCursor(cur_col*25, (cur_row)*25, 22);
	
}

void moveCursorDown(){
	if(cur_row==7)	return;
	redraw(cur_row, cur_col, board);
	cur_row++;
	drawCursor(cur_col*25, cur_row*25, 22);
	
}

void moveCursorLeft(){
	if(cur_col==0)	return;
	redraw(cur_row, cur_col, board);
	cur_col--;
	drawCursor(cur_col*25, cur_row*25, 22);
	
	
}

void moveCursorRight(){
	if(cur_col==7)	return;
	redraw(cur_row, cur_col, board);
	cur_col++;
	drawCursor(cur_col*25, cur_row*25, 22);
}


void drawCircle(int x, int y, int color) {
     int i, j;
        for(j=-12; j<=12; j++)
	    	for(i=-12; i<=12; i++)
	        if(i*i + j*j < 144)
	            write_pixel(x+i, y+j, color);

}
void drawSquare(int x, int y, int color){ 
	int i,j;
	for (i=0;i<25;i++)
      for (j=0;j<25;j++)
         write_pixel(i+x,j+y, color);
}

void redraw(int x, int y, char board[row][col]){ //draws the cell
	if(board[x][y]==w_square){
		drawSquare(xCoordinates[x][y], yCoordinates[x][y], 3);	
	}else if(board[x][y]==b_square){
		drawSquare(xCoordinates[x][y], yCoordinates[x][y], 56);
	}else if(board[x][y]==w_piece-32){
		drawCircle(12.5+xCoordinates[x][y], 12.5+yCoordinates[x][y], 22);
	}else if(board[x][y]==b_piece-32){
		drawCircle(12.5+xCoordinates[x][y], 12.5+yCoordinates[x][y], 22);
	}
	else if(board[x][y]==b_piece){
		drawSquare(xCoordinates[x][y], yCoordinates[x][y], 56);
		drawCircle(12.5+xCoordinates[x][y], 12.5+yCoordinates[x][y], 0);
	}
	else if(board[x][y]==w_piece){
		drawSquare(xCoordinates[x][y], yCoordinates[x][y], 56);
		drawCircle(12.5+xCoordinates[x][y], 12.5+yCoordinates[x][y], 15);
	}
	else if(board[x][y]==req_piece){
		drawSquare(xCoordinates[x][y], yCoordinates[x][y], 56);
		drawCircle(12.5+xCoordinates[x][y], 12.5+yCoordinates[x][y], 20);
	}
	else if(board[x][y]==p_move){
		drawSquare(xCoordinates[x][y], yCoordinates[x][y], 56);
		drawCircle(12.5+xCoordinates[x][y], 12.5+yCoordinates[x][y], 25);
	}
}
void setBoardUI(){
	int i, j;

	for(i=0; i < row; i++){ //draws the board
		for(j = 0; j < col; j++){
			redraw(i, j, board);
		}
		
	}
}
void setDummyBoardUI(char dummy[row][col], char src[row][col]){
	int i, j;

	for(i=0; i < row; i++){ //draws the board
		for(j = 0; j < col; j++){
			if(dummy[i][j] != src[i][j]) redraw(i, j, dummy);
		}
		
	}
}
void erase(int x, int y, int w, int h){ //basically covers an area with a black rectangle 
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}

void writeMenu(){
	//writes the menu 
	write_text("Controls: ", 210, 30, WHITE, 0); 
	write_text("w - up",220,40,WHITE,0); 
	write_text("a - left",220,50,WHITE,0);
	write_text("s - down",220,60,WHITE,0);
	write_text("d - right",220,70,WHITE,0);   
	write_text("space- move",220,80,WHITE,0);
	write_text("r - resign",220,90,WHITE,0);  
	write_text("c - draw",220,100,WHITE,0); 
}
header(){
	write_text("FILIPINO CHECKERS",70,20,WHITE,1); //title
	write_text("(Dama)", 120, 40, WHITE, 1);
	write_text("1 - Standard Mode",40,140,WHITE,0); 
	write_text("2 - Reversi Mode",40,160,WHITE,0); 
	write_text("q - Quit",40,180,WHITE,0); 
}

