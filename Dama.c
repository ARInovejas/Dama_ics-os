#include "../../sdk/dexsdk.h"


#define standard_mode '1'
#define reversi_mode '2'
#define help 'h'
#define new_game 'n'
#define quit 'q'
#define resign 'r'
#define call_draw 'c'
#define up_key 'w'
#define left_key 'a'
#define down_key 's'
#define right_key 'd'
#define move '_'

void setBoardUI();
void erase(int x, int y, int w, int h); //lights
void moveCursorUp();
void moveCursorDown();
void moveCursorLeft();
void moveCursorRight();
void drawCursor(int x, int y);

int currentCoordinateX, currentCoordinateY;
int gameOver;
int main(){

	char keyPressed;
	set_graphics(VGA_320X200X256);
	header();

	currentCoordinateX = 0;
	currentCoordinateY = 0;
	gameOver = 0;

	keyPressed = (char)getch();

	if(keyPressed==standard_mode){
		erase(1,1,640,395);
		setBoardUI();
		do{
			keyPressed = (char)getch();
			if(keyPressed==up_key)	moveCursorUp();

			if(keyPressed==down_key) moveCursorDown();

			if(keyPressed==left_key)	moveCursorLeft();

			if(keyPressed==right_key)	moveCursorRight();
			if(keyPressed==quit)	break;
		}while(gameOver==0);

		erase(1,1,640,395);
		set_graphics(VGA_TEXT80X25X16);
		clrscr();
		
	}else if(keyPressed==reversi_mode){
		erase(1,1,640,395);
		setBoardUI();
		do{
			keyPressed = (char)getch();
			if(keyPressed==up_key)	moveCursorUp();

			if(keyPressed==down_key) moveCursorDown();

			if(keyPressed==left_key)	moveCursorLeft();

			if(keyPressed==right_key)	moveCursorRight();
			if(keyPressed==quit)	break;
		}while(gameOver==0 || keyPressed!=quit);

		erase(1,1,640,395);
		set_graphics(VGA_TEXT80X25X16);
		clrscr();
		
	}

}

void drawCursor(int x, int y){
	int i, j;
	x+=12.5;
	y+=12.5;
       for(j=-4; j<=4; j++)
	    	for(i=-4; i<=4; i++)
	        if(i*i + j*j <16)
	            write_pixel(x+i, y+j, 22);
}
void moveCursorUp(){
	currentCoordinateY--;
	if(currentCoordinateY==-1)	return;
	drawCursor(currentCoordinateX*25, (currentCoordinateY)*25);
	
}

void moveCursorDown(){
	currentCoordinateY++;
	if(currentCoordinateY==7)	return;
	drawCursor(currentCoordinateX*25, currentCoordinateY*25);
	
}

void moveCursorLeft(){
	currentCoordinateX--;
	if(currentCoordinateX==-1)	return;
	drawCursor(currentCoordinateX*25, currentCoordinateY*25);
	
}

void moveCursorRight(){
	currentCoordinateX++;
	if(currentCoordinateX==0)	return;
	drawCursor(currentCoordinateX*25, currentCoordinateY*25);
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

void setBoardUI(){
	int i, j;

	for(i=0; i < 200; i+=50){
		drawSquare(i, 0, 3);
		drawSquare(i, 50, 3); 
		drawSquare(i, 100, 3);
		drawSquare(i, 150, 3);
		drawSquare(i, 25, 56);
		drawSquare(i, 75, 56);
		drawSquare(i, 125, 56);
		drawSquare(i, 175, 56);
	}
			

	for(i=25; i < 200; i+=50)	{
		drawSquare(i, 25, 3);
		drawSquare(i, 75, 3);
		drawSquare(i, 125, 3);
		drawSquare(i, 175, 3);

		drawSquare(i, 0, 56);
		drawSquare(i, 50, 56);
		drawSquare(i, 100, 56);
		drawSquare(i, 150, 56);
	}	
	
	for(i = 37.5; i < 200; i +=50)	{

		drawCircle(i, 12.5, 0);
  		drawCircle(i, 62.5, 0);
		drawCircle(i, 162.5, 15);
	}
	for(i = 12.5; i < 200; i +=50)	{
		drawCircle(i, 37.5, 0);
		drawCircle(i, 137.5, 15);
		drawCircle(i, 187.5, 15);
	}
	
	write_text("Controls: ", 210, 10, WHITE, 0); 
	write_text("w - up",220,20,WHITE,0); 
	write_text("a - left",220,30,WHITE,0);
	write_text("s - down",220,40,WHITE,0);
	write_text("d - right",220,50,WHITE,0);   
	write_text("space- move",220,60,WHITE,0);
	write_text("r - resign",220,70,WHITE,0);  
	write_text("c - draw",220,80,WHITE,0); 

	
	
}
void erase(int x, int y, int w, int h){ //basically covers an area with a black rectangle 
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}

header(){
	write_text("FILIPINO CHECKERS",70,20,WHITE,1); //title
	write_text("(Dama)", 120, 40, WHITE, 1);
	write_text("1 - Standard Mode",40,160,WHITE,0); 
	write_text("2 - Reversi Mode",40,180,WHITE,0); 
}