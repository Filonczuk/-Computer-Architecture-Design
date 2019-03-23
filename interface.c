#include <msp430x14x.h>
#include "lcd.h"
#include "portyLcd.h"
#include "interface.h"

#define UP 			0
#define DOWN 		        1
#define MAIN_MENU 	        0	//Main Menu
#define GAME_MODE_MENU 		1	//Game Mode selection Menu
#define DIFF_LEVEL_MENU         2	//Difficulty Level selection Menu
#define AUTHORS_MENU            3
#define MM_LINES 	        3	//number of LINES in Main Menu
#define GMM_LINES 	        4	//number of LINES in Game Mode selection Menu
#define DLM_LINES 	        4	//number of LINES in Difficulty Level selection Menu
#define AM_LINES 	        4

short int linesArray[4] = {MM_LINES,GMM_LINES,DLM_LINES,AM_LINES};
short int cursor = 0;		//cursor position: 0 - first line, 1 - second line
short int position = 0;		//indicates which menu lines are displayed
short int MMSel = 0;		//Main Menu selection
short int GMSel = 0;		//Game Mode selection
short int DLSel = 0;		//Difficulty Level selection
short int currentMenu = MAIN_MENU;


//	moves selection cursor up and down, scrolls menu when needed
void moveCursor(short int direction, short int currentMenu, char MMLines[4][16], char GMMLines[4][16], char DLMLines[4][16], char ALines[4][16]){
	cursor += direction;
	if(cursor <= -1) {
		position--;
		cursor = UP;
	}
	else if(cursor >= 2) {
		position++;
		cursor = DOWN;
	}
	if(position < 0) position = 0;
	else if(position >= linesArray[currentMenu]-1) position = linesArray[currentMenu]-2;
        
        if(currentMenu == MAIN_MENU) displayMenu(MMLines);
        else if(currentMenu == GAME_MODE_MENU) displayMenu(GMMLines);
        else if(currentMenu == DIFF_LEVEL_MENU) displayMenu(DLMLines);
        else if(currentMenu == AUTHORS_MENU) displayMenu(ALines);
}

//	displays a menu
void displayMenu(char menuLines[4][16]){
  short int i;
  clearDisplay();
  
    if(cursor == UP) putc(0);
    for(i=0; i<16; i++) putc(menuLines[position][i]);

    gotoSecondLine();
    if(cursor == DOWN) putc(0);
    for(i=0; i<16; i++) putc(menuLines[position+1][i]);
  
}

//	saves custom characters into CG RAM, such as a selection cursor or letters with diacritics
void initializeCustomCharactersAndMenu(char MMLines[4][16]){
  short int i;
  char selectionCursor[8] = {0x10,0x18,0x1C,0x1E,0x1C,0x18,0x10};	        //0
  
  SEND_CMD(CG_RAM_ADDR);
  for(i=0; i<8; i++) SEND_CHAR(selectionCursor[i]);
  SEND_CMD(DD_RAM_ADDR);
  
  displayMenu(MMLines);
}

//	handles pressed buttons correctly if placed in an infinite loop
int handleButtons(char MMLines[4][16], char GMMLines[4][16], char DLMLines[4][16], char AMLines[4][16]){

	if((P4IN & BIT4)==0){
	  moveCursor(-1, currentMenu, MMLines, GMMLines, DLMLines, AMLines);
          delay_ms(200);
	}
	else if((P4IN & BIT7)==0){
	  moveCursor(1, currentMenu, MMLines, GMMLines, DLMLines, AMLines);
          delay_ms(200);
	}

	switch(currentMenu){
		case MAIN_MENU:{

			if((P4IN & BIT5)==0){//SELECT
				switch(position+cursor){

					case 0: {
						currentMenu = GAME_MODE_MENU;
                                                position = 0;
						displayMenu(GMMLines);
					}break;

					case 1: {
						currentMenu = AUTHORS_MENU;
						position = 0;
						displayMenu(AMLines);
					}break;

					case 2: return 0;
				}
			}
			else if((P4IN & BIT6)==0){//BACK
				return 0;
			}

		}break;
		case GAME_MODE_MENU:{

			if((P4IN & BIT5)==0){//SELECT
				switch(position+cursor){

					case 0: {
						GMSel = 0;
						currentMenu = DIFF_LEVEL_MENU;
                                                position = 0;
						displayMenu(DLMLines);
					}break;

					case 1: {
						GMSel = 1;
						currentMenu = DIFF_LEVEL_MENU;
                                                position = 0;
						displayMenu(DLMLines);
					}break;

					case 2: {
						GMSel = 2;
						currentMenu = DIFF_LEVEL_MENU;
                                                position = 0;
						displayMenu(DLMLines);
					}break;

					case 3: {
						currentMenu = MAIN_MENU;
                                                position = 0;
						displayMenu(MMLines);
					}break;
				}
			}
			else if((P4IN & BIT6)==0){//BACK
				currentMenu = MAIN_MENU;
                                                position = 0;
				displayMenu(MMLines);
			}
		}break;
		case DIFF_LEVEL_MENU:{

			if((P4IN & BIT5)==0){//SELECT
				switch(position+cursor){

					case 0: {
						DLSel = 0;
						currentMenu = MAIN_MENU;
					}break;

					case 1: {
						DLSel = 1;
						currentMenu = MAIN_MENU;
					}break;

					case 2: {
						DLSel = 2;
						currentMenu = MAIN_MENU;
					}break;

					case 3: {
						currentMenu = GAME_MODE_MENU;
                                                position = 0;
						displayMenu(GMMLines);
					}break;
				}
				if(position+cursor != 3){
					clearDisplay();
					 play(GMSel,DLSel);
                                                position = 0;
					 displayMenu(MMLines);
                                         
				}
			}
			else if((P4IN & BIT6)==0){//BACK
				currentMenu = GAME_MODE_MENU;
                                                position = 0;
				displayMenu(GMMLines);
			}
		}break;
                case AUTHORS_MENU:{
			if((P4IN & BIT6) == 0 || (P4IN & BIT5) == 0){//BACK
				currentMenu = MAIN_MENU;
                                position = 0;
				displayMenu(MMLines);
			}
		}break;

	}
	return 1;
}
