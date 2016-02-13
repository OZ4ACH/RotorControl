
#include <ezLCDLib.h>

#define led 13

ezLCD3 lcd;
volatile boolean ezLCDInt = false; // flag to indicate interrupt

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.light(100,1,25);
  lcd.cls();
  lcd.font(0);
  lcd.color(WHITE);
  lcd.picture(35,0,"em.gif");
  lcd.xy(15,110);
  lcd.printString("What are you going to \\[6mMake\\[3m Today ?"); // the \\[6m will change draw color to yellow and \\[3m will chagne it back to white
 }

void loop()
{
  if(  ezLCDInt )                 //do nothing till ezLCDInt becomes true
  {
    detachInterrupt(0);
    ezLCDInt = false;             //reset ezLCDInt flag
/*


*/

    attachInterrupt(0, ezLCDevent, LOW);   
  }
}

void ezLCDevent( void ) {
  ezLCDInt = true;
}
