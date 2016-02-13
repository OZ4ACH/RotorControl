#include "ezLCDLib.h"

#include <SoftwareSerial.h>


ezLCD3 lcd; // create lcd object

#define GRAD2RAD (float)0.017453293


// MODE
byte mode;
#define NORMAL 1
#define SETUP 2

// encoder
#define DEKODER_1 		2
#define DEKODER_2 		3
#define ENCODER_STEP 	5
long encoder;
long encodertime;
long encoderold;

// encoder switch
#define DEKODER_SW 		A1
long switchtime;

// rotor pot
#define ROTOR_POT 		A2
#define ROTOR_POT_MIN 	25
#define ROTOR_POT_MAX	855
long rotorgrader;

// Rele
#define RELE_BREAK  	4
#define RELE_CCW    	5
#define RELE_CW 		6 

// Antenne
long antenneoffset;
#define ANTENNEOFFSET_SET 180
long antennegrader;
long antennegraderold;

// Serial
#define SERIAL_SPEED 1200
char SerialStr[42];
int SerialStrLen;
int SerialPos;

SoftwareSerial mySerial(12, 11); 


// DIV
long setgrader;

long delaymillis;

long rotor_safetime;

byte rotor;
byte rotorold;

#define CW 1
#define CCW 2
#define BREAK 3
#define DELAY_RUN 4
#define DELAY_BREAK 5
#define WAIT_DELAY_RUN 6
#define WAIT_DELAY_BREAK 7


#define BREAK_DELAY_START		500
#define BREAK_DELAY_STOP		2000

#define ROTOR_SAFETIME			85000




class cirkel_c
{
//private void dotupdate();
	
	
public:

	void init(int cx,int cy,int cr,int ct,int cp,int coc,int co1,int co2){
		int x;
		int y;
		float xf;
		float yf;

		cirkel_x = cx;
		cirkel_y = cy;
		cirkel_r = cr;
		cirkel_t = ct;
		cirkel_p = cp;
		color_c = coc;
		color_1 = co1;
		color_2 = co2;

        lcd.color(color_c);  
		for (int grader = 0; grader < 360; grader=grader+2) {
			xf = sin((float)grader * GRAD2RAD) * cirkel_r;	
			yf = cos((float)grader * GRAD2RAD) * cirkel_r;	
			x = (int)xf +  cirkel_x;	
			y = -(int)yf + 240 - cirkel_y;	
	        lcd.circle( x, y, cirkel_t, FILL );  
		}
		xo1 = -500;
		yo1 = -500;
		xo2 = -500;
		yo2 = -500;
		
		grader1 = 0;
		grader2 = 0;
		
	}



	void setdot1(int grader) {
		grader1 = grader;
		dotupdate();
	}

	void setdot2(int grader) {
		grader2 = grader;
		dotupdate();
	}


private:	
	int xo1,xo2;
	int yo1,yo2;
	
	int cirkel_x;
	int cirkel_y;
	int cirkel_t;
	int cirkel_r;
	int cirkel_p;
	
	int color_c;
	int color_1;
	int color_2;
	
	int grader1,grader2;

	void dotupdate() {

		int x;
		int y;
		float xf;
		float yf;

		xf = sin((float)grader1 * GRAD2RAD) * cirkel_r;	
		yf = cos((float)grader1 * GRAD2RAD) * cirkel_r;	
		x = (int)xf +  cirkel_x;	
		y = -(int)yf + 240 - cirkel_y;	
		if ((xo1 != x) || (yo1 != y)) {
	        lcd.color(color_c);  
	        lcd.circle( xo1, yo1, cirkel_p, FILL );  
	    }
	
        lcd.color(color_1);  
        lcd.circle( x, y, cirkel_p, FILL );  
        xo1 = x;
        yo1 = y;


		xf = sin((float)grader2 * GRAD2RAD) * cirkel_r;	
		yf = cos((float)grader2 * GRAD2RAD) * cirkel_r;	
		x = (int)xf +  cirkel_x;	
		y = -(int)yf + 240 - cirkel_y;	
		if ((xo2 != x) || (yo2 != y)) {
        	lcd.color(color_c);  
	        lcd.circle( xo2, yo2, cirkel_p, FILL );  
		}
        lcd.color(color_2);  
        lcd.circle( x, y, cirkel_p, FILL );  
        xo2 = x;
        yo2 = y;
	}
};


cirkel_c cirkel;




long read_rotorgrader()
{
	long grader;

	// read antenne position
 	grader = analogRead(ROTOR_POT);

	if (grader < ROTOR_POT_MIN) {
		grader = ROTOR_POT_MIN;
	}
	if (grader > ROTOR_POT_MAX) {
		grader = ROTOR_POT_MAX;
	}
 	grader = 1024 - grader;   // invert read
 	grader = 102300 / grader; // 1023 * 100R
 	grader = grader - 100;    // 100R
 	grader = grader * 360;    // 360 grader
 	grader = grader / 495;    // 500R max potmeter

	if (grader < 0)
	{
		grader = 0;
	}
	if (grader > 360)
	{
		grader = 360;
	}
	return grader;
}



void setup()
{
	// encoder switch
    pinMode(DEKODER_SW, INPUT_PULLUP);
    switchtime = 0;

	// Set mode
	mode = NORMAL;
    if (digitalRead(DEKODER_SW) == 0)
    {
		delay(500);
	    if (digitalRead(DEKODER_SW) == 0)
	    {
			mode = SETUP;
	    }
    }
	//mode = SETUP; // set to debug

	// rotor
	rotorgrader = read_rotorgrader();

	// RELE
    pinMode(RELE_BREAK, OUTPUT);
    pinMode(RELE_CCW, OUTPUT);
    pinMode(RELE_CW, OUTPUT);

	digitalWrite(RELE_BREAK ,LOW);
	digitalWrite(RELE_CCW   ,LOW);
	digitalWrite(RELE_CW 	,LOW);

	// antenne
	antenneoffset = ANTENNEOFFSET_SET;
	antennegrader = rotorgrader + antenneoffset;
    if (antennegrader >= 360) { antennegrader = antennegrader - 360; }
    if (antennegrader < 0)    { antennegrader = antennegrader + 360; }

	// encoder
    pinMode(DEKODER_1, INPUT_PULLUP);
    pinMode(DEKODER_2, INPUT_PULLUP);
    attachInterrupt(0, encoder_int, CHANGE);
    encoder = antennegrader;
	encodertime	= 0;

	// Serial
	mySerial.begin(SERIAL_SPEED);
	SerialStrLen = 0;

	mySerial.println("OZ4ACH Rotor");
	
	
	

	// SETUP ************************************************************************************************************
	if (mode == SETUP)
	{
		// LCD	
		lcd.begin( EZM_BAUD_RATE );
	
		lcd.cls(); 
		lcd.light(70); 
	
	    lcd.font(0);       
		lcd.color(WHITE);
	  	lcd.xy(20,20);
		lcd.print("SETUP");

	  	lcd.xy(20,40);
		lcd.print("Rotor");
	  	lcd.xy(20,60);
		lcd.print("Antenne");
	  	lcd.xy(20,80);
		lcd.print("Encoder");
	  	lcd.xy(20,100);
		lcd.print("Serial len");
	  	//lcd.xy(20,120);
		//lcd.print("Serial");

	}

	// NORMAL ************************************************************************************************************
	if (mode == NORMAL)
	{

	    rotor = BREAK;
	    delaymillis = 0;
	    
		rotorold = BREAK;
	
		encoderold = -1000;
		antennegraderold = -1000;
	
		
		// LCD	
		lcd.begin( EZM_BAUD_RATE );
/*	
		lcd.cls(); 
		lcd.light(70); 
		lcd.picture(0,0,"QSL-1.gif"); 
	
	    lcd.font(0);       
		lcd.color(BLACK);
	  	lcd.xy(20,205);
		lcd.print("d. 9/11-2014");
	
	
		delay(2000);
	*/
		lcd.cls(); 
		lcd.light(70); 
		lcd.picture(0,0,"Map2.gif"); 
	
		cirkel.init(99,122,90,8,7,BLACK,RED,GREEN);
		
	    lcd.color(BLUE);  
		lcd.rect(242,0,78,240,FILL);
	

		// LCD
	    lcd.font(0);       
		lcd.color(WHITE);
	  	lcd.xy(250,70);
		lcd.println("Set");
	  	lcd.xy(250,20);
		lcd.println("Antenne");
	
	  	lcd.xy(270,120);
		lcd.println("CW");
	  	lcd.xy(270,150);
		lcd.println("CCW");
	  	lcd.xy(270,180);
		lcd.println("BRK");
	
	    lcd.font(0);       
		lcd.color(BLACK);
	  	lcd.xy(246,215);
		lcd.print("OZ4ACH");
	
	  	lcd.drawLed( 10, 255, 130, BLACK, WHITE);
	  	lcd.drawLed( 10, 255, 160, BLACK, WHITE);
	  	lcd.drawLed( 10, 255, 190, BLACK, WHITE);
	  	
	  	
	  	// test totch områder
		lcd.color(BLACK);
		lcd.lineType(1);

/*
		// USE 315
		lcd.xy(0,10);
		lcd.box(70,90,false);
		// Mellem amerika 275
		lcd.xy(0,100);
		lcd.box(60,40,false);
		// Sydamerika 240
		lcd.xy(0,140);
		lcd.box(60,80,false);
		// Australine op 70
		lcd.xy(180,50);
		lcd.box(65,50,false);
		// Australine ned 90
		lcd.xy(180,100);
		lcd.box(65,70,false);
		// Japan 35
		lcd.xy(130,10);
		lcd.box(50,70,false);
		// New Zelland 45
		lcd.xy(180,0);
		lcd.box(65,50,false);
		// Indien 105
		lcd.xy(140,120);
		lcd.box(50,50,false);
		// Afrika 175
		lcd.xy(70,160);
		lcd.box(70,70,false);
		// Norge og Sverige 0
		lcd.xy(70,10);
		lcd.box(60,90,false);
		// Syd europa vest 225
		lcd.xy(60,100);
		lcd.box(40,60,false);
		// Syd europa east 160
		lcd.xy(100,100);
		lcd.box(40,60,false);
		// Rusland 60
		lcd.xy(140,80);
		lcd.box(40,40,false);
		// Indiske ocean 130
		lcd.xy(140,170);
		lcd.box(60,60,false);
*/
	  	
	}
}


void loop(){
	// Read switch
    if (digitalRead(DEKODER_SW) == 0)
    {
        switchtime++;
    } else 
    {
        switchtime = 0;
    }

	rotorgrader = read_rotorgrader();

	antennegrader = rotorgrader + antenneoffset;
    if (antennegrader >= 360) { antennegrader = antennegrader - 360; }
    if (antennegrader < 0)    { antennegrader = antennegrader + 360; }

	//if(Serial.available() > 0)
	//{
	//	SerialStr[SerialStrLen] = (char)Serial.read();
	//	SerialStrLen++;
	//	if (SerialStrLen >= 40) SerialStrLen = 39;
	//}








	// SETUP ************************************************************************************************************
	if (mode == SETUP)
	{

		lcd.color(BLUE);  
		lcd.rect(145,35,150,150,FILL);
		
	    lcd.font(0);       
		lcd.color(WHITE);

	  	lcd.xy(150,40);
		lcd.print(rotorgrader);
	  	lcd.xy(150,60);
		lcd.print(antennegrader);
	  	lcd.xy(150,80);
		lcd.print(encoder);
	  	//lcd.xy(150,100);
		//lcd.print(SerialStrLen);


	  	//lcd.xy(150,120);
	  	
		//for (int s = 0;s < SerialStrLen; s++)
		//{
		//	//lcd.print(SerialStr[s]);
		//	lcd.print("X");
		//}	  	
		
	}

	// NORMAL ************************************************************************************************************
	if (mode == NORMAL)
	{
		// encoder
	    if (encoder >= 360) { encoder = encoder - 360; }
	    if (encoder < 0)    { encoder = encoder + 360; }

		if (lcd.touchS() == 1) {
			int tx = lcd.touchX();
			int ty = lcd.touchY();
			
			// USE 315
			if ((  0 < tx) && (tx <  70) && ( 10 < ty) && (ty <  100)) {encoder = 315; }
			// Mellem amerika 275
			if ((  0 < tx) && (tx <  60) && (100 < ty) && (ty <  140)) {encoder = 275; }
			// Sydamerika 240
			if ((  0 < tx) && (tx <  60) && (140 < ty) && (ty <  220)) {encoder = 240; }
			// Australine op 70
			if (( 180 < tx) && (tx <245) && ( 50 < ty) && (ty < 100))  {encoder =  70; }
			// Australine ned 90
			if (( 180 < tx) && (tx <245) && (100 < ty) && (ty < 170))  {encoder =  90; }
			// Japan 35
			if ((130 < tx) && (tx < 180) && (10 < ty) && (ty <  80))   {encoder =  35; }
			// New Zelland 45
			if ((180 < tx) && (tx < 245) && ( 0 < ty) && (ty <  50))   {encoder =  45; }
			// Indien 105
			if ((140 < tx) && (tx < 190) && (120 < ty) && (ty <  170)) {encoder = 105; }
			// Afrika 175
			if ((70 < tx) && (tx < 140) && (160 < ty) && (ty <  230))  {encoder = 175; }
			// Norge og Sverige 0
			if ((70 < tx) && (tx < 130) && (10 < ty) && (ty <  100))   {encoder =   0; }
			// Syd europa vest 225
			if ((60 < tx) && (tx < 100) && (100 < ty) && (ty <  160))  {encoder = 225; }
			// Syd europa east 160
			if ((100 < tx) && (tx < 140) && (100 < ty) && (ty <  160)) {encoder = 160; }
			// Rusland 60
			if ((140 < tx) && (tx < 180) && (80 < ty) && (ty <  120))  {encoder =  60; }
			// Indiske ocean 130
			if ((140 < tx) && (tx < 200) && (170 < ty) && (ty <  230)) {encoder = 130; }
	
			switchtime = 2;
		
			while (lcd.touchS() == 1) {};
			delay(10);
		}
	
		// SERIAL ---------------------------
	
		if (mySerial.available())
		{
			SerialStr[SerialStrLen] = mySerial.read();
			SerialStrLen++;

			if (SerialStrLen == 4)
			{
				if (	(SerialStr[0] == 'M')
					&&  (SerialStr[1] >= 48) &&  (SerialStr[1] <= 57)
					&&  (SerialStr[2] >= 48) &&  (SerialStr[2] <= 57)
					&&  (SerialStr[3] >= 48) &&  (SerialStr[3] <= 57)
					)
				{
					SerialPos = SerialStr[3] - 48;
					SerialPos = SerialPos + ((SerialStr[2] - 48) *10);
					SerialPos = SerialPos + ((SerialStr[1] - 48) *100);
					
					if ((SerialPos < 0) || (SerialPos > 360))					
					{
						SerialPos = encoder;
					}				

					encoder = SerialPos;
					switchtime = 2;
					SerialStrLen = 0;
					mySerial.println("Mxxx");
				}
				else
				{
					SerialStrLen = 0;
					mySerial.println("Overload1");
				}
			}
			else 
			if (!(	(SerialStr[(SerialStrLen-1)] == 'M')
				|| 	((SerialStr[(SerialStrLen-1)] >= 48) &&  (SerialStr[(SerialStrLen-1)] <= 57))
				))
			{
				SerialStrLen = 0;
				mySerial.println("Overload2");
			}
		}	
	
	
		// Kontroler rotor ---------------------------
	
	
		setgrader = encoder + antenneoffset;
	    if (setgrader >= 360) { setgrader = setgrader - 360; }
	    if (setgrader < 0)    { setgrader = setgrader + 360; }

	
		switch (rotor) {
			case (CW):
			    if (switchtime == 2)
			    {
			        rotor = DELAY_BREAK;
			    }
				if (setgrader <= rotorgrader)
				{
					rotor = DELAY_BREAK;
				}
				if (rotor_safetime < millis()) {
					rotor = DELAY_BREAK;
					lcd.color(BLACK);
				  	lcd.xy(160,215);
					lcd.print("SAFETIME");
				}
				break;
			case (CCW):
			    if (switchtime == 2)
			    {
			        rotor = DELAY_BREAK;
			    }
				if (setgrader >= rotorgrader)
				{
					rotor = DELAY_BREAK;
				}
				if (rotor_safetime < millis()) {
					rotor = DELAY_BREAK;
					lcd.color(BLACK);
				  	lcd.xy(160,215);
					lcd.print("SAFETIME");
				}
				break;
			case (BREAK):
			    if (switchtime == 2)
			    {
			        rotor = DELAY_RUN;
	
					rotor_safetime = ROTOR_SAFETIME ; 
					rotor_safetime = rotor_safetime * abs(setgrader - rotorgrader);
					rotor_safetime = rotor_safetime /360;
					rotor_safetime = rotor_safetime + millis() + BREAK_DELAY_START + 2000;
			    }
				break;
			case (DELAY_RUN):
				// set break on output
				delaymillis = millis() + BREAK_DELAY_START;
				rotor = WAIT_DELAY_RUN;
				break;
			case (WAIT_DELAY_RUN):
				if (delaymillis < millis()) {
					if (setgrader > rotorgrader)
					{
						rotor = CW;
					}
					else
					{
						rotor = CCW;
					}
				}
				break;
			case (DELAY_BREAK):
				delaymillis = millis() + BREAK_DELAY_STOP;
				rotor = WAIT_DELAY_BREAK;
				break;
			case (WAIT_DELAY_BREAK):
				if (delaymillis < millis()) {
					// set break off output
					rotor = BREAK;
				}
				break;
			default:
				rotor = DELAY_BREAK;
		}
	
	
	
	
	
		if (rotor != rotorold) {
			switch (rotor) {
				case (CW):
				  	lcd.drawLed( 10, 255, 130, GREEN, WHITE);
				  	lcd.drawLed( 10, 255, 160, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 190, RED, WHITE);
					digitalWrite(RELE_BREAK ,HIGH);
					digitalWrite(RELE_CCW   ,LOW);
					digitalWrite(RELE_CW 	,HIGH);
					break;
				case (CCW):
				  	lcd.drawLed( 10, 255, 130, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 160, GREEN, WHITE);
				  	lcd.drawLed( 10, 255, 190, RED, WHITE);
					digitalWrite(RELE_BREAK ,HIGH);
					digitalWrite(RELE_CCW   ,HIGH);
					digitalWrite(RELE_CW 	,LOW);
					break;
				case (BREAK):
				  	lcd.drawLed( 10, 255, 130, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 160, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 190, BLACK, WHITE);
					digitalWrite(RELE_BREAK ,LOW);
					digitalWrite(RELE_CCW   ,LOW);
					digitalWrite(RELE_CW 	,LOW);
					break;
				case (DELAY_RUN):
				  	lcd.drawLed( 10, 255, 130, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 160, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 190, RED, WHITE);
					digitalWrite(RELE_BREAK ,HIGH);
					digitalWrite(RELE_CCW   ,LOW);
					digitalWrite(RELE_CW 	,LOW);
					break;
				case (WAIT_DELAY_RUN):
				  	lcd.drawLed( 10, 255, 130, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 160, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 190, RED, WHITE);
					digitalWrite(RELE_BREAK ,HIGH);
					digitalWrite(RELE_CCW   ,LOW);
					digitalWrite(RELE_CW 	,LOW);
					break;
				case (DELAY_BREAK):
				  	lcd.drawLed( 10, 255, 130, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 160, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 190, RED, WHITE);
					digitalWrite(RELE_BREAK ,HIGH);
					digitalWrite(RELE_CCW   ,LOW);
					digitalWrite(RELE_CW 	,LOW);
					break;
				case (WAIT_DELAY_BREAK):
				  	lcd.drawLed( 10, 255, 130, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 160, BLACK, WHITE);
				  	lcd.drawLed( 10, 255, 190, RED, WHITE);
					digitalWrite(RELE_BREAK ,HIGH);
					digitalWrite(RELE_CCW   ,LOW);
					digitalWrite(RELE_CW 	,LOW);
					break;
				default:
				  	lcd.drawLed( 10, 255, 130, BLUE, WHITE);
				  	lcd.drawLed( 10, 255, 160, BLUE, WHITE);
				  	lcd.drawLed( 10, 255, 190, BLUE, WHITE);
					digitalWrite(RELE_BREAK ,LOW);
					digitalWrite(RELE_CCW   ,LOW);
					digitalWrite(RELE_CW 	,LOW);
			}
		}
		rotorold = rotor;
	
	   
	
	 
		if (encoderold != encoder) {
	    	cirkel.setdot1(encoder);  
	
			lcd.color(BLACK);  
			lcd.rect(260,90,45,21,FILL);
		
		    lcd.font(0);       
			lcd.color(WHITE);
		  	lcd.xy(270,90);
			lcd.print(encoder);
			antennegraderold = -1000;
		}
		encoderold = encoder;
		
	    
		if (	(	(encoderold != encoder) 
				&&  (rotor != BREAK))
			||	(antennegraderold > (antennegrader + 5))
			||	(antennegraderold < (antennegrader - 5))
			)
		{
		    cirkel.setdot2(antennegrader);  
		
			lcd.color(BLACK);  
			lcd.rect(260,40,45,22,FILL);
		
		    lcd.font(0);       
			lcd.color(WHITE);
		  	lcd.xy(270,40);
			lcd.print(antennegrader);
	
			antennegraderold = antennegrader;
		}
	}
}




void encoder_int()
{
    byte e2;
    byte e3;

	if (encodertime < millis()) {
		encodertime = millis() + 35;

	    e2 = digitalRead(DEKODER_1);
	    e3 = digitalRead(DEKODER_2);
	
	    if ((e2 == 0) && (e3 == 0))
	    {
	        encoder = encoder - ENCODER_STEP;
	    } 
	
	    if ((e2 == 0) && (e3 == 1))
	    {
	        encoder = encoder + ENCODER_STEP;
	    } 
	
	    if ((e2 == 1) && (e3 == 0))
	    {
	        encoder = encoder + ENCODER_STEP;
	    } 
	
	    if ((e2 == 1) && (e3 == 1))
	    {
	        encoder = encoder - ENCODER_STEP;
	    } 
	}
}

