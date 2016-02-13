 
#include <ezLCDLib.h>

#define LED_PIN 13

ezLCD3 lcd;

int xPos = 100;  // horizontal position
int yPos = 85;   // vertical position
int radius = 75;
int option = 1; // 1=draw, 2=disabled.
int resolution = 25;
int value = 250;
int max = 500;
int id = 1;


void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.fontw( 1, "sans24" );
  lcd.theme( 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0 );
  lcd.cls(BLACK);
  lcd.color(WHITE);  
  lcd.dial( id, xPos, yPos, radius, option, resolution, value, max, 0 );

  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );
}


void loop()
{
   int value = lcd.wvalue(id);
  lcd.color(BLACK);
  lcd.xy(200,180);
  lcd.box(100,50,FILL);
  lcd.color(WHITE);
  lcd.println(value,DEC);
   blink(value);
}

void blink(int rate)
{  
  digitalWrite( LED_BUILTIN, HIGH ); // turn LED on
  delay(rate);
  digitalWrite( LED_BUILTIN, LOW );  // turn LED off
  delay(rate);
}