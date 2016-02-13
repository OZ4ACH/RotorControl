/*
 * radio.ino
 * radio buttons select the blink rate of an LED
 */

#include <ezLCDLib.h>

#define LED_PIN 13

ezLCD3 lcd;
volatile boolean ezLCDInt = false;

int xPos = 25;  // horizontal position
int yPos = 80;   // vertical position
int width = 250;
int height = 35;
int option = 5; // 0=remove, 1=draw, 2=disabled, 3=checked,
// 4=draw first unchecked, 5=draw first checked

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.cls(BLACK,WHITE);
  lcd.font("0");
  lcd.printString("    Radio Test To Toggle LED On D13");
  lcd.fontw( 1, "sans24" );
  lcd.theme( 1, 9, 3, 0, 0, 0, 8, 8, 8, 1, 1 );
  lcd.xy(20,30);
  lcd.string( 1, "Off" );   // stringId 1
  lcd.string( 2, "Slow" );  // stringId 2
  lcd.string( 3, "Fast" );  // stringId 3
  lcd.radioButton( 1, xPos, yPos, width, height, 5, 1,1 );
  lcd.radioButton( 2, xPos, yPos + 50, width, height, 1, 1, 2 );
  lcd.radioButton( 3, xPos, yPos + 100, width, height, 1, 1, 3 );

  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );
  lcd.drawLed( 12, 160, 50, BLACK, WHITE);
}

int rate = 0; // blink delay, 0 is off 

void loop()
{
    if ( lcd.wvalue(1) == 1 )
      rate = 0;
    if ( lcd.wvalue(2) == 1 )
      rate = 500;
    if ( lcd.wvalue(3) == 1 )
      rate = 200;

   if ( rate > 0 ) blink();  
   delay(100);
}

void blink()
{  
  digitalWrite( LED_PIN, HIGH ); // turn LED on
  lcd.drawLed( 12, 160, 50, LIME, WHITE);
  delay(rate);
  digitalWrite( LED_PIN, LOW );  // turn LED off
  lcd.drawLed( 12, 160, 50, BLACK, WHITE);
  delay(rate);
}



