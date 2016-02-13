/*
*  Robot demo 
* copy robot.gif to arLCD \ezusers\images
*
*
*/
#include <ezLCDLib.h>  

#define led 13

ezLCD3 lcd;  // create lcd object from ezLCD3

volatile boolean ezLCDInt = false; // flag to indicate interrupt
char touch = 0;
bool dir = 0;
int x = 0;
void drawLed( char dim, int x, int y, char colorLed, char colorHigh) 
{
  lcd.xy(x,y);
  lcd.color(DIMGRAY);
  lcd.circle(dim,NOFILL);
  lcd.color(WHITE);
  lcd.arc(dim+1, 145,270,NOFILL); //circle(12,NOFILL);
 
  lcd.color(colorLed);
  lcd.circle(dim-2,FILL);
  lcd.color(colorHigh);
  lcd.arc(dim-4,180,270,NOFILL);
  lcd.arc(dim-5,180,270,NOFILL);
}
void drawEye( char dim, int x, int y )
{
  lcd.xy(x,y);
  lcd.color(BLACK);
  lcd.circle(10,FILL);
  lcd.color(WHITE);
  lcd.arc(8,180,270,NOFILL);
}

void setup()
{
  lcd.begin( EZM_BAUD_RATE );    //baudrate is 115200 set in ezLCDLib.h
  lcd.light(10);               
  lcd.cls();
  lcd.font(0);
  
  pinMode(led, OUTPUT);    
  attachInterrupt(0, ezLCDevent, LOW);   
  lcd.picture("robot.gif");
  drawLed(20,120, 71, BLACK, GRAY); 
  drawLed(20,200, 71, BLACK, WHITE);  
  lcd.light(100);                //backlight full bright
 }

void loop()
{
  delay(100);  
//  drawLed(20,120, 71, LIME, WHITE);  
//  drawLed(20,200, 71, LIME, WHITE);  
//  delay(200);
  unsigned char c=random(1,200);
  drawLed(20,200, 71, c, WHITE);  
  drawEye(10,190+x,71);
  drawLed(20,120, 71, c, WHITE);  
  drawEye(10,110+x,71);
  if((dir==0) && ((x+=5) > 20)) { dir=1;}
  if((dir==1) && ((x-=5) < 1)) { dir=0;}
  if(  ezLCDInt )                 //do nothing till ezLCDInt becomes true
  {

    detachInterrupt(0);           // disable touch interrupt just to be safe
    ezLCDInt = false;             //reset ezLCDInt flag
    touch = lcd.wstack(FIFO);
/* 
*  touch will equal the widget number that has changed, button press ect.
*  there are also 3 global vars that go along with the wstack command .
*  lcd.currentWidget  current widget number stame as touch above.
*  lcd.currentInfo  will have widget state info for buttons will be 4 for pressed 1 for released 2 for cancel.
*  lcd.currentData  this is used for widgets like sliders it will have the current slider value here .
*/
    switch(touch)
    {
      case 1:
      /*
      * Add your code here for widget 1 
      */
      if(lcd.currentInfo == PRESSED)  // if widget 1 was a button this would mean the button was pressed and released
      {
      
      }  
      if(lcd.currentInfo == RELEASED)  // if widget 1 was a button this would mean the button was pressed and not released yet
      {
        
      } 
      break;
  
      case 2:
      /*
      * Add your code here for widget 2 
      */
      break;  

      default:
      break;
    }
    attachInterrupt(0, ezLCDevent, LOW);   
  }
}

void ezLCDevent( void ) {
  ezLCDInt = true;
}
