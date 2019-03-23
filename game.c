#include<msp430x14x.h>
#include "lcd.h"
#include "portyLcd.h"
#include <time.h>

#define EASY               0
#define MEDIUM             1
#define HARD               2

#define SOUND_LOW         111
#define SOUND_MEDIUM      222
#define SOUND_HIGH        444
#define SOUND_DURATION    500

#define DIODES_SOUNDS      0
#define DIODES_ONLY        1
#define SOUNDS_ONLY        2

#define DIODE_STATUS       0
#define DIODE_REL1         1
#define DIODE_REL2         2
  
#define LOSS               0
#define VICTORY            1
 
#define CORRECT_L          8
#define WRONG_L            6
#define YOU_WON_L          8
#define YOU_LOST_L         9
#define SCORE_L            7
#define GAME_TIME_L        6

int i=0;
char correct[8]= {'C','O','R','R','E','C','T','!'};
char wrong[6]={'W','R','O','N','G','!'};
char youWon[8] = {'Y','O','U',' ','W','O','N','!'};
char youLost[9] ={'Y','O','U',' ','L','O','S','T','!'};
char score[7] = {'S','c','o','r','e',':',' '};
char gameTime[6] = {'T','i','m','e',':',' '};

void delay_ms(unsigned int ms )
{
    unsigned int j;
    for (j = 0; j<= ms; j++)
       __delay_cycles(500);
}
 
void delay_us(unsigned int us )
{
    unsigned int j;
    for (j = 0; j<= us/2; j++)
       __delay_cycles(1);
}

void beep(unsigned int frequency, unsigned int duration)
{
    int j;
    long delay = (long)(10000/frequency);  
    long time = (long)((duration*100)/(delay*2)); 
    for (j=0;j<time;j++)
    {
        P4OUT |= BIT2;     
        delay_us(delay);   
        P4OUT &= ~BIT2;    
        delay_us(delay);   
    }
    delay_ms(20); 
}
     
void gameResult(short int result,int time,int playerScore){
  clearDisplay();
  if( result == LOSS) printString(youLost, YOU_LOST_L);
  else if(result == VICTORY) printString(youWon, YOU_WON_L);  
  delay_ms(500);
  clearDisplay();
  printString(score,SCORE_L);
  print2Digit(playerScore);
  gotoSecondLine();
  printString(gameTime,GAME_TIME_L);
  print2Digit(time);
  delay_ms(500);
  while( ((P4IN & BIT4) !=0) && ((P4IN & BIT5) !=0) && ((P4IN & BIT6) != 0) && ((P4IN & BIT7) !=0) ){
  }
   clearDisplay();
    
}
  void initializeAndConfigure(){
  WDTCTL=WDTPW + WDTHOLD;
  P1DIR |= BIT5 + BIT6;
  P2DIR |= BIT1;
  
  P4DIR |= BIT2 + BIT3;
  P4DIR &= ~BIT4;
  P4DIR &= ~BIT5;
  P4DIR &= ~BIT6;
  P4DIR &= ~BIT7;
      
  InitPortsLcd();                   
  InitLCD();                       
  clearDisplay(); 
  P2OUT |= BIT1;
  P1OUT &= ~BIT5;
  P1OUT &= ~BIT6;
  BCSCTL1 |= XTS;

}
void addSignal(int *sequenceArray, int sequenceSize){
    srand(time(0));
    for(i=0; i<sequenceSize; i++) sequenceArray[i] = rand() % 3;
}
 
void generateRandomSequence(int *sequenceArray, int progressPointer, short gameMode){
     P2OUT |= BIT1;
     P1OUT &= ~BIT5;
     P1OUT &= ~BIT6;
     int j;
     
       for(i=0;i<progressPointer;i++){
       switch(sequenceArray[i]){
            
           case 0:   if(gameMode == 0 || gameMode == 1)
                     P2OUT &= ~BIT1;
                     if(gameMode == 0 || gameMode == 2)  {
                       beep(SOUND_LOW,SOUND_DURATION);
                     }if(gameMode == 2)delay_ms(500); 
                     P2OUT |= BIT1;
                     delay_ms(500); 
                     break;
           
            case 1:  if(gameMode == 0 || gameMode == 1)
                     P1OUT |= BIT5;
                     if(gameMode == 0 || gameMode == 2)  {
                       beep(SOUND_MEDIUM,SOUND_DURATION);
                     }
                     if(gameMode == 2) delay_ms(500); 
                     P1OUT &= ~BIT5;
                     delay_ms(500); 
                     break;
                     
            case 2:  if(gameMode == 0 || gameMode == 1)
                     P1OUT |= BIT6;
                     if(gameMode == 0 || gameMode == 2)  {
                      beep(SOUND_HIGH,SOUND_DURATION);
                     }
                     if(gameMode == 2)delay_ms(500); 
                     P1OUT &= ~BIT6;
                     delay_ms(500); 
                     break;
           default:
            
             for(j =0 ;j <5;j++){
                   P1OUT |= BIT6;
                   delay_ms(500);
                   P1OUT &= ~BIT6;
                   delay_ms(500); 
             }
             break;
       }
     }
}

short int playerResponse(int *sequenceArray,int progressPointer,short gameMode){
 int counter = 0;
  while(1){
    
   if( (P4IN & BIT4) ==0){
      if(gameMode == 0 || gameMode == 1)
      P2OUT &= ~BIT1;
      
      if(gameMode == 0 || gameMode == 2)  {
        beep(SOUND_LOW,SOUND_DURATION);
      }
      
      delay_ms(300);
      P2OUT |= BIT1;
      
      if(sequenceArray[counter] != DIODE_STATUS){
       printString(wrong,WRONG_L);
       delay_ms(1000);
       clearDisplay();
       return 0;
     } else counter++;
     
    }
    
    
   if( (P4IN & BIT5) ==0){
      if(gameMode == 0 || gameMode == 1)
      P1OUT |= BIT5;
      
      if(gameMode == 0 || gameMode == 2)  {
        beep(SOUND_MEDIUM,SOUND_DURATION);
       }
      delay_ms(300);
      P1OUT &= ~BIT5;
      
      if(sequenceArray[counter] != DIODE_REL1){
      printString(wrong,WRONG_L);
       delay_ms(1000);
       clearDisplay();
       return 0;
     } else counter++;
    }
   
   if( (P4IN & BIT6) ==0){
       if(gameMode == 0 || gameMode == 1)
       P1OUT |= BIT6;
       if(gameMode == 0 || gameMode == 2)  {
        beep(SOUND_HIGH,SOUND_DURATION);
        }
       delay_ms(300);
       P1OUT &= ~BIT6;
       if(sequenceArray[counter] != DIODE_REL2){
       printString(wrong,WRONG_L);
         delay_ms(1000); 
        clearDisplay();
         return 0;
     } else counter++;
    }
   
   if(counter >=progressPointer) {
      printString(correct,CORRECT_L );
      delay_ms(1000); 
      clearDisplay();
     return 1;
   }
  }
   
}

short int play(int mode, int level){
  int sequenceArray[18];
  int progressPointer=0;
  for(i=0;i<18;i++){
    sequenceArray[i] = 0;
  }
  
  addSignal(sequenceArray,18);
  progressPointer = 0;
  if(level == EASY){
   
    do{ 
        progressPointer++;
        
        if(progressPointer>10){
          
        gameResult(VICTORY,5,10);
        return 10;
        }
        
        generateRandomSequence(sequenceArray,progressPointer,mode);
       }while(playerResponse(sequenceArray, progressPointer,mode) !=0) ;
    
     gameResult(LOSS,5,progressPointer-1);
     return progressPointer-1;
  }
  else if(level == MEDIUM){
  
    do{ 
         progressPointer+=2;
       
         if(progressPointer>14){
         
         gameResult(VICTORY,5,14);
         return 14;
         }
         
        generateRandomSequence(sequenceArray,progressPointer,mode);
       }while(playerResponse(sequenceArray, progressPointer,mode) !=0);
     
     gameResult(LOSS,5,progressPointer-2);
     return progressPointer-2;
  }
  else if(level == HARD){
     
     do{ 
         progressPointer+=3;
         
         if(progressPointer>18){
           
         gameResult(VICTORY,5,18);
         return 18;
        }
        
        generateRandomSequence(sequenceArray,progressPointer,mode);
       }while(playerResponse(sequenceArray, progressPointer,mode) !=0);
    
     gameResult(LOSS,5,progressPointer-3);
     return progressPointer-3;
  }
}
