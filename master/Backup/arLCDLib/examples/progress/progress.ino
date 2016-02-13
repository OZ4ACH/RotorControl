 
#include <ezLCDLib.h>

ezLCD3 lcd;

int xPos = 25;  // horizontal position
int yPos = 50;   // vertical position
int width = 250;
int height = 35;
int option = 1; // 1=draw horizontal, 2=horizontal disabled, 3=vertical,
                // 4=vertical disabled, 5=redraw horizontal,
                // 6=redraw horizontal disabled, 7=redraw vertical,
                // 8=redraw vertical disabled
int value = 0;
int max= 128;
int newvalue = 0;

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.fontw( 1, "2" );//medium font
  lcd.theme( 1, 110, 106, 99, 0, 0, 105, 107, 0, 0, 1 );
  lcd.cls( BLACK, WHITE );  
  lcd.string(1,"%");  //set string 1 to % for progress bar
  lcd.progressBar( 1, xPos, yPos, width, height, option, value, max, 1 ,1);
}

void loop()
{
   int value = analogRead(0)/8;
//   value = map(value, 0, 1023, 0, 100);     // scale it to use it with the servo (value between 0 and 180)
   if ( newvalue != value ) {
      lcd.wvalue(1, value);
      newvalue = value;
   }
//   value = map(value, 0, 1023, 0, 99);     // scale it to use it with the servo (value between 0 and 180)
//   lcd.wvalue(1, value);
   delay(10);
}

