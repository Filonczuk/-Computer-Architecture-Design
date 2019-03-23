void initializeAndConfigure();
void generateRandomSequence(int *sequenceArray, int progressPointer,short gameMode);
void addSignal(int *sequenceArray, int sequenceSize);
void gameResult(short int result,int time,int playerScore);
short int playerResponse(int *sequenceArray,int sequenceSize,short gameMode);
short int play(int mode, int level);
void beep(unsigned int frequency, unsigned int duration);
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);