 
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
int min = -40;
int max= 125;
int initial = 30;

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.fontw( 1, "0" );
  lcd.theme( 1, 155, 152, 3, 3, 1, 0, 1, 0, 0, 0 );
  lcd.cls( BLACK, WHITE );  
  lcd.string(1,"%");  //set string 1 to % for gauge
  lcd.gauge( 1, xPos, yPos, width, height, option, initial, min, max, 1, 1 );
}

void loop()
{
  min = min + 5;
  if ( min <= max + 1 )
     lcd.wvalue( 1, min );
  delay(250);
}

