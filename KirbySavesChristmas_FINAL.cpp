#include <allegro.h>
#include <math.h>
#include <stdio.h>
	
//global variables
//position of the sprite
float my_pic_x = 50;
float my_pic_y = 50;

//prototypes
bool checkLeft(int tilePlacement[][50], int numRows, int numCols);
bool checkRight(int tilePlacement[][50], int numRows, int numCols);
bool checkUp(int tilePlacement[][50], int numRows, int numCols);
bool checkDown(int tilePlacement[][50], int numRows, int numCols);
void gravity();
void jump(int tilePlacement[][50], int numRows, int numCols, BITMAP *buffer);
bool checkDeath(int tilePlacement[][50], int numRows, int numCols);	
void spriteAnimation(float frameCounter, BITMAP *buffer);
bool checkWin(int tilePlacement[][50], int numRows, int numCols);
void respawn(int position_x, int position_y);
int mainMenu(int border[][50], int levelCompleted[]);
void winCutscene();
void borderDraw(int border[][50], int numRows, int numCols, BITMAP *buffer);
	
int main(int argc, char *argv[]) { 
    //Allegro Stuffs
	allegro_init(); 
    install_keyboard();  
    set_color_depth(desktop_color_depth()); 
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0);
	BITMAP *buffer = create_bitmap(640,480);
	BITMAP *wall = load_bitmap("wall.bmp", NULL);
	BITMAP *spike = load_bitmap("spikes.bmp", NULL);
	BITMAP *door = load_bitmap("door.bmp", NULL);
	BITMAP *LevelSelect = load_bitmap("LevelSelect.bmp", NULL);
	BITMAP *LevelSelected = load_bitmap("LevelSelected.bmp", NULL);
	int pic_x = 0;
	int pic_y = 0;

	//Standard C++ variables
	FILE *fptr;
	int levelDesign[50][50];
	int levels [15];
	int border[50][50];
	int levelCompleted[15];
    bool downlock = false;
    bool uplock = false;
    bool leftlock = false;
    bool rightlock = false;
    bool death = false;
	bool win = false;
    bool levelselect = false;
	int level = 0;
	int numberOfRows = 0;
	int numberOfColumns = 0;
	int integer = 0;
	int spawnPoint = 0;
	int respawn_x = 0;
	int respawn_y = 0;	
	int deathcounter = 0;
	float frameCounter = 0;
	int deathwater = 0;
	
	//reads in border to create it evrywhere
	fptr = fopen("menuBorder.txt",  "r");
	while(fscanf(fptr, "%d", &numberOfRows) != EOF){
		fscanf(fptr, "%d", &numberOfColumns);
		for (int i = 0; i < numberOfRows; i++){
			for (int j = 0; j < numberOfColumns; j++){
				fscanf(fptr, "%d", &integer);
				border[i][j] = integer;
			}
		}
	}
	fclose(fptr);
	
	//main loop
	while(!key[KEY_ESC]){
		//ressetting the variables for new looping
		win = false;
		deathcounter = 0;
		spawnPoint = 0;
		level = 0;
		
		//runs the main menu function until a level is selected
		while(level == 0){
			level = mainMenu(border, levelCompleted);
	    	printf("%d", level);
	    }
	    
	    //reads the level that is selected from the corresponding text file 
		if (level == 1){
			fptr = fopen("level1.txt",  "r");
		}
		else if (level == 2){
			fptr = fopen("level2.txt",  "r");
		}
		else if (level == 3){
			fptr = fopen("level3.txt",  "r");
		}
		else if (level == 4){
			fptr = fopen("level4.txt",  "r");
		}
		else if (level == 5){
			fptr = fopen("level5.txt",  "r");
		}
		else if (level == 6){
			fptr = fopen("level6.txt",  "r");
		}
		else if (level == 7){
			fptr = fopen("level7.txt",  "r");
		}
		else if (level == 8){
			fptr = fopen("level8.txt",  "r");
		}
			
	    //reads the level design from the text file to an array
	    while(fscanf(fptr, "%d", &numberOfRows) != EOF){
			fscanf(fptr, "%d", &numberOfColumns);
			for (int i = 0; i < numberOfRows; i++){
				for (int j = 0; j < numberOfColumns; j++){
					fscanf(fptr, "%d", &integer);
					levelDesign[i][j] = integer;
				}
			}
		}
		fclose(fptr);
			
		//ingame codes
		while(!key[KEY_ESC] && win == false){
			//movement code(moves the sprite with certain restrictions)	
			if(key[KEY_RIGHT] && checkLeft(levelDesign, numberOfRows, numberOfColumns) == false && death == false && win == false){ 
				if (checkDeath(levelDesign, numberOfRows, numberOfColumns) == true){
					death = true;
				}
				if (checkWin(levelDesign, numberOfRows, numberOfColumns) == true){
					win = true;
				}
				else{
					my_pic_x +=0.2;
				}
			} 		
			if(key[KEY_LEFT] && checkRight(levelDesign, numberOfRows, numberOfColumns) == false && death == false){ 
				if (checkDeath(levelDesign, numberOfRows, numberOfColumns) == true){
					death = true;
				}
				else{
					my_pic_x -=0.2;
				}	
			}		
			if(key[KEY_DOWN] && checkUp(levelDesign, numberOfRows, numberOfColumns) == false && death == false){ 
				if (checkDeath(levelDesign, numberOfRows, numberOfColumns) == true){
					death = true;
				}
				else{
					my_pic_y +=0.2;
				}	 
			}
				
			//jump code
			if(key[KEY_UP] && checkDown(levelDesign, numberOfRows, numberOfColumns) == false && checkUp(levelDesign, numberOfRows, numberOfColumns) == true && death == false){
				jump(levelDesign, numberOfRows, numberOfColumns, buffer);	
			} 
	
			//applies gravity to the sprite and checks to see if gravity kills the player
			if(checkUp(levelDesign, numberOfRows, numberOfColumns) == false){
				gravity();
				if (checkDeath(levelDesign, numberOfRows, numberOfColumns) == true){
					death = true;
				}
			}
			
			//respawns the player if he is dead
			if(death == true){
				respawn(respawn_x, respawn_y);
				death = false;
				deathcounter ++;
				printf("%d\n", deathcounter);	
			}
			
			//animates the sprite
			frameCounter += 0.5;
			if(frameCounter > 240){
				frameCounter = 0;
			}
			
			//buffers the sprite
			if(win == false){
				spriteAnimation(frameCounter, buffer);	
			}
			
			//buffers the level design
			pic_x = 0;
			pic_y = 0;
			for (int i = 0; i < numberOfRows; i++){
				pic_y = i * 20;
				pic_x = 0;
				for (int j = 0; j < numberOfColumns; j++){
					if (levelDesign[i][j] == 1){
						draw_sprite(buffer, wall, pic_x, pic_y);
						pic_x += 20;	
					}
					else if (levelDesign[i][j] == 2){
						draw_sprite(buffer, spike, pic_x, pic_y);
						pic_x += 20;
					}
					else if (levelDesign[i][j] == 3){
						draw_sprite(buffer, door, pic_x, pic_y);
						pic_x += 20;
					}
					else if(levelDesign[i][j] == 4 && spawnPoint ==0){
						respawn_x = pic_x;
						respawn_y = pic_y;
						my_pic_y = pic_y;
						my_pic_x = pic_x;
						pic_x += 20;
						spawnPoint = 1;	
					}
					else{
						pic_x += 20;
					}
				}
			}
			
			//shows the win cutscene when the player wins
			if (win == true){
				levelCompleted[level - 1] = 1;
				clear_bitmap(buffer);
				winCutscene();
			}
			
			//prints everything in buffer and then clears it to prepare for the next set 
			blit(buffer, screen, 0,0,0,0,640,480); 
			clear_bitmap(buffer); 
		}
		rest(250);
	}
	return 0;
}
END_OF_MAIN()

//function to check if the player hits the left side of the walls 
bool checkLeft(int tilePlacement[][50], int numRows, int numCols){
	bool lock = false;
	for(int i = 0; i < numCols; i++){
		for(int j = 0; j < numCols; j++){
			if(my_pic_x+21 >=  (i)*20 && my_pic_y <= (j)*20+20 && my_pic_y+19 >= (j)*20 && my_pic_x+20 <= i*20+20 && tilePlacement[j][i] == 1 ){	
				lock = true;
				break;
			} 	
		}
	}	
	return lock;
}
END_OF_FUNCTION()

//function to check if the player hits the right side of the walls 
bool checkRight(int tilePlacement[][50], int numRows, int numCols){
	bool lock = false;
	for(int i = 0; i < numCols; i++){
		for(int j = 0; j < numCols; j++){
			if(my_pic_x <=  (i)*20+20 && my_pic_y <= (j)*20+20 && my_pic_y+19 >=(j)*20 && my_pic_x >= (i)*20 && tilePlacement[j][i]== 1){
				lock = true;
				break;
			} 	
		}
	}
	return lock;
}
END_OF_FUNCTION()

//fuction to check if the player hits the top side of the walls 
bool checkUp(int tilePlacement[][50], int numRows, int numCols){
	bool lock = false;
	for(int i = 0; i < numCols; i++){
		for(int j = 0; j < numCols; j++){
			if(my_pic_y+20 >= j*20 && my_pic_x >=  (i)*20-20  && my_pic_x <= (i)*20+19 && my_pic_y+20 <= j*20+20  && tilePlacement[j][i] == 1){
				lock = true;
				break;
			}	
		}
	}
	return lock;
}
END_OF_FUNCTION()

//fuction to check if the player hits the bottom side of the walls 
bool checkDown(int tilePlacement[][50], int numRows, int numCols){
	bool lock = false;
	for(int i = 0; i < numCols; i++){
		for(int j = 0; j < numCols; j++){
			if(my_pic_y >= j*20 && my_pic_x >=  (i)*20-20  && my_pic_x <= (i)*20+19 && my_pic_y <= j*20+21  && tilePlacement[j][i] == 1){
				lock = true;
				break;
			}	
		}
	}
	return lock;
}
END_OF_FUNCTION()

//fuction to add gravity to the sprite
void gravity(){
	my_pic_y += 0.29;
}
END_OF_FUNCTION()

//fuction to add jump and movement while mid air to the sprite
void jump(int levelDesign[][50], int numberOfRows, int numberOfColumns, BITMAP *buffer){
	float frameCounter = 0;
	
	for(int i =0; i < 360; i++){	
		my_pic_y -= 0.25;
		frameCounter += 0.5;
		if(frameCounter > 240){
			frameCounter = 0;
		}
		spriteAnimation(frameCounter, screen);	
		if(checkDown(levelDesign, numberOfRows, numberOfColumns) == true){
			break;
		}
		if(key[KEY_RIGHT] && checkLeft(levelDesign, numberOfRows, numberOfColumns) == false){ 
			my_pic_x +=0.1;	
		} 		
		if(key[KEY_LEFT] && checkRight(levelDesign, numberOfRows, numberOfColumns) == false){ 
			my_pic_x -=0.1;	
		}		
		if(key[KEY_DOWN] && checkUp(levelDesign, numberOfRows, numberOfColumns) == false){ 	
			my_pic_y +=0.1;		 
		}	
		rest(0.5);	
	}
}
END_OF_FUNCTION()

//fuction to check if the sprite dies with spike or not
bool checkDeath(int tilePlacement[][50], int numRows, int numCols){
	bool status = false; //alive
	for(int i = 0; i < numCols; i++){
		for(int j = 0; j < numCols; j++){
			if(my_pic_y+19 >= j*20 && my_pic_x >= (i)*20-20  && my_pic_x <= (i)*20+19 && my_pic_y <= j*20+20  && tilePlacement[j][i] == 2){
				status = true; //death
				break;
			}	
		}
	}
	return status;
}
END_OF_FUNCTION()

//fuction to animate the sprite
void spriteAnimation(float frameCounter, BITMAP *buffer){
	BITMAP *picture1 = load_bitmap("picture1.bmp", NULL);
	BITMAP *picture2 = load_bitmap("picture2.bmp", NULL); 
	BITMAP *picture3 = load_bitmap("picture3.bmp", NULL);
	
	if(frameCounter < 60){
		draw_sprite(buffer, picture1, my_pic_x, my_pic_y);
	}
	else if(frameCounter >= 60 && frameCounter < 120){
		draw_sprite(buffer, picture2, my_pic_x, my_pic_y);
	}
	else if(frameCounter >= 120 && frameCounter < 180){
		draw_sprite(buffer, picture1, my_pic_x, my_pic_y);
	}
	else{
		draw_sprite(buffer, picture3, my_pic_x, my_pic_y);
	}
	
}
END_OF_FUNCTION()

//function to check if the player won or not
bool checkWin(int tilePlacement[][50], int numRows, int numCols){
	bool completion = false;
	for(int i = 0; i < numCols; i++){
		for(int j = 0; j < numCols; j++){
			if(my_pic_x <=  (i)*20+20 && my_pic_y <= (j)*20+20 && my_pic_y+19 >=(j)*20 && my_pic_x >= (i)*20 && tilePlacement[j][i] == 3){
				completion = true;
				break;
			}
		}
	}
	return completion;
}
END_OF_FUNCTION()

//function to respawn the player
void respawn(int position_x, int position_y){
		my_pic_x = position_x;
		my_pic_y = position_y;
}
END_OF_FUNCTION()


int mainMenu(int border[][50], int levelCompleted[]){
	//Allegro Stuffs
	BITMAP *LevelSelect = load_bitmap("LevelSelect.bmp", NULL);
	BITMAP *LevelSelected = load_bitmap("LevelSelected.bmp", NULL);
	BITMAP *LevelButton = load_bitmap("levelbutton.bmp", NULL);
	BITMAP *LevelButtonSelected = load_bitmap("levelbuttonselected.bmp", NULL);
	BITMAP *InstructionButton = load_bitmap("instructionbutton.bmp", NULL);
	BITMAP *InstructionButtonSelected = load_bitmap("instructionbuttonselected.bmp", NULL);
	BITMAP *buffer = create_bitmap(640,480);
	BITMAP *InstructionPage = load_bitmap("InstructionPage.bmp", NULL);
	BITMAP *Title = load_bitmap("MainMenuTitle.bmp", NULL);
	BITMAP *Footer = load_bitmap("MainMenuFooter.bmp", NULL);
	BITMAP *CheckMark = load_bitmap("CheckMark.bmp", NULL);
	
	//Standard C++ Variables 
	int pic_x = 0;
	int checkMark_x = 0;
	int pic_y = 0;
	int numberOfRows = 32;
	int numberOfColumns = 32;
	bool levelselect = false;
	int level = 0;
	int levels [15];
	int mainMenu [2];
	FILE *fptr;
	
	//the first menu screen
	//draws in the buttons
	pic_y = 150;
	pic_x = 260;
	clear_bitmap(buffer); 
	borderDraw(border, numberOfRows, numberOfColumns, buffer);
	blit(buffer, screen, 0,0,0,0,640,480);
	for (int i =0; i<2; i++){
		if (i == 0){	
			mainMenu[i] = 1;
			draw_sprite(screen, LevelButtonSelected, pic_x, pic_y);			
		}
		else if(i == 1){
			pic_y += 100;
			mainMenu[i] = 0;
			draw_sprite(screen, InstructionButton, pic_x, pic_y);
		}
	}
	//user uses arrow key to select a button and enter to select
	//highlights and dims buttons accordingly
	draw_sprite(screen, Title, 220, 20);
	draw_sprite(screen, Footer, 420, 360);
	
	while(!key[KEY_ENTER]){
		if(key[KEY_DOWN]){
			if(mainMenu[0] == 1){
				pic_y = 150;
				pic_x = 260;
				mainMenu[0] = 0;
				
				draw_sprite(screen, LevelButton, pic_x, pic_y);
				pic_y += 100;
				
				mainMenu[1] = 1;
				draw_sprite(screen, InstructionButtonSelected, pic_x, pic_y);
			}
		}
		if(key[KEY_UP]){
			
			if(mainMenu[1] == 1){
				pic_y = 250;
				pic_x = 260;
				mainMenu[1] = 0;
				draw_sprite(screen, InstructionButton, pic_x, pic_y);
				
				pic_y -= 100;
				
				mainMenu[0] = 1;
				draw_sprite(screen, LevelButtonSelected, pic_x, pic_y);
			}
		}	
	}
	rest(250);
	clear_bitmap(buffer); 
	borderDraw(border, numberOfRows, numberOfColumns, buffer);
    blit(buffer, screen, 0,0,0,0,640,480);
    printf("%d", mainMenu[1]);
	//displays level select
	if (mainMenu[0] == 1){
		pic_x = 30;
		pic_y = 200;
		for (int i = 0; i<8; i++){	
				//prints the first set of buttons
				if(i == 0){
					levels[i] = 1;
					//prints the first button highlighted
					draw_sprite(screen, LevelSelected, pic_x, pic_y);
				}
				else{
					levels[i] = 0;
					//prints rest of the buttons dimmed
					draw_sprite(screen, LevelSelect, pic_x, pic_y);
				}			
				pic_x += 75;					
		}
		
		pic_x = 30;
		pic_y = 200;
		for (int i = 0; i<8; i++){
			if (levelCompleted[i] == 1){
				draw_sprite(screen, CheckMark, pic_x, pic_y);
				pic_x += 75;
			}
			else{
				pic_x += 75;
			}
		}
		
		pic_x = 30;
		pic_y = 200;
		//user uses arrow keys to select and enter key to select level
		//highlights and dims buttons accordingly
		while(!key[KEY_ENTER]){
			
			if(key[KEY_RIGHT] && levels[7] != 1){
				for (int i = 0; i<8; i++){
					//finds where the selected button is	
					if(levels[i] == 1 && i <= 7){	
						levels[i] =0;
						pic_x = i * 75 + 30;
						//re draws it as not selected	
						draw_sprite(screen, LevelSelect, pic_x, pic_y);
		
						pic_x += 75;
						levels[i+1] =1;
						//draws new selected item	
						draw_sprite(screen, LevelSelected, pic_x, pic_y);
						break;
					}
					else{
						pic_x += 75;
					}
				}
			}
			if(key[KEY_LEFT] && levels[0] != 1){
				for (int i = 0; i<8; i++){
					//finds where the selected button is		
					if(levels[i] == 1){
						levels[i] =0;
						pic_x = i *  75 + 30;
						//re draws it as not selected	
						draw_sprite(screen, LevelSelect, pic_x, pic_y);
		
						pic_x -= 75;
						levels[i-1] =1;
						//draws new selected item	
						draw_sprite(screen, LevelSelected, pic_x, pic_y);
						break;
					}
					else{
						pic_x -= 75;
					}
				}
			}
			checkMark_x = 30;
			for (int i = 0; i<8; i++){
				if (levelCompleted[i] == 1){
					draw_sprite(screen, CheckMark, checkMark_x, pic_y);
					checkMark_x += 75;
				}
				else{
					checkMark_x += 75;
				}
			}
			// rests to slow down movement between buttons	
			rest(100);
			//clears screen when escape is pressed
			if(key[KEY_ESC]){
				clear_bitmap(buffer); 
				borderDraw(border, numberOfRows, numberOfColumns, buffer);
    			blit(buffer, screen, 0,0,0,0,640,480);
				return 0;
			}
		}
		//determines which text file is to be opened
		level = 0;
		for(int i = 0; i<8; i++){
			if (levels[i] == 1){
				level = i+1;
			}
		}
		return(level);
	}
	
	//displays the instructions page
	while(!key[KEY_ESC]){
		if (mainMenu[1] == 1){	
			clear_bitmap(buffer); 
			draw_sprite(buffer, InstructionPage, 20, 20);				
			borderDraw(border, numberOfRows, numberOfColumns, buffer);
			blit(buffer, screen, 0,0,0,0,640,480);
		}
	}
	//clears screen
	clear_bitmap(buffer); 
	blit(buffer, screen, 0,0,0,0,640,480);
	return 0;	
}
END_OF_FUNCTION()

void winCutscene(){
	//Allegro stuffs
	BITMAP *doorLeft = load_bitmap("doorLeft.bmp", NULL);
	BITMAP *wall = load_bitmap("wall.bmp", NULL);
	BITMAP *buffer = create_bitmap(640,480);
	BITMAP *tree = load_bitmap("tree.bmp", NULL);
	BITMAP *treepresents = load_bitmap("treepresents.bmp", NULL);
	BITMAP *Wintitle = load_bitmap("WinScreen.bmp", NULL);
	//Standard C++ variables
	FILE *fptr;
	int levelDesign[50][50];
	int integer;
	int numberOfRows;
	int numberOfColumns;
	int pic_x = 0;
	int pic_y = 0;
	int spawnPoint = 0;
	float frameCounter = 0;
	
	//loads in level textfile and reads information in
	fptr = fopen("cutscene.txt",  "r");
	while(fscanf(fptr, "%d", &numberOfRows) != EOF){
		fscanf(fptr, "%d", &numberOfColumns);
		for (int i = 0; i < numberOfRows; i++){
			for (int j = 0; j < numberOfColumns; j++){
				fscanf(fptr, "%d", &integer);
				levelDesign[i][j] = integer;
			}
		}
	}
	fclose(fptr);
	
	while (!key[KEY_ESC]){
		pic_x = 0;
		pic_y = 0;
		//draws the level and the sprite
		for (int i = 0; i < numberOfRows; i++){
			pic_y = i * 20;
			pic_x = 0;
			for (int j = 0; j < numberOfColumns; j++){
				if (levelDesign[i][j] == 1){
					draw_sprite(buffer, wall, pic_x, pic_y);
					pic_x += 20;
				}
				else if (levelDesign[i][j] == 2){
					draw_sprite(buffer, doorLeft, pic_x, pic_y);
					pic_x += 20;
				}
				else if(levelDesign[i][j] == 4 && spawnPoint ==0){
					my_pic_y = pic_y;
					my_pic_x = pic_x;
					pic_x += 20;
					spawnPoint = 1;	
				}
				else{
					pic_x += 20;
				}
			}
		}
	
		// animates the sprite
		frameCounter += 0.05;
		if(frameCounter > 240){
			frameCounter = 0;
		}
		spriteAnimation(frameCounter, buffer);
		
		//moves the sprite until sprite reaches tree
		//changes the tree from bare to with presents when that happens
		if(my_pic_x < 450){
			my_pic_x += 0.25;	
			draw_sprite(buffer, tree, 470, 340);
		}
		else{
			draw_sprite(buffer, treepresents, 470, 340);
			draw_sprite(buffer, Wintitle, 110, 100);
		}
		
		blit(buffer, screen, 0,0,0,0,640,480); 
		clear_bitmap(buffer);
	}
	rest(10);
}
END_OF_FUNCTION()

//draws a border
void borderDraw(int border[][50], int numRows, int numCols, BITMAP *buffer){
	int pic_x;
	int pic_y;
	BITMAP *wall = load_bitmap("wall.bmp", NULL);
	
	for (int i = 0; i < numRows; i++){
		pic_y = i * 20;
		pic_x = 0;
		for (int j = 0; j < numCols; j++){
			if (border[i][j] == 1){
				draw_sprite(buffer, wall, pic_x, pic_y);
				pic_x += 20;	
			}
			else{
				pic_x += 20;
			}
		}
	}
}
END_OF_FUNCTION()
