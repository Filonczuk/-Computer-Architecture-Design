/***************************************
*	interface, version 1.0.t
****************************************/

void moveCursor(short int direction, short int currentMenu, char MMLines[4][16], char GMMLines[4][16], char DLMLines[4][16], char ALines[4][16]);
void displayMenu(char menuLines[4][16]);
void initializeCustomCharactersAndMenu(char MMLines[4][16]);
int handleButtons(char MMLines[4][16], char GMMLines[4][16], char DLMLines[4][16], char AMLines[4][16]);
void delay_ms(unsigned int ms );