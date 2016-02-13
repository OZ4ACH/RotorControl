
// Serial
#define SERIAL_SPEED 4800
char SerialStr[42];
int SerialStrLen;




void setup()
{

	// Serial
	Serial.begin(SERIAL_SPEED,SERIAL_8N1);
	SerialStrLen = 0;

}


void loop(){

	if(Serial.available() > 0)
	{
		SerialStr[SerialStrLen] = (char)Serial.read();
		SerialStrLen++;
		if (SerialStrLen >= 40) SerialStrLen = 39;
	}

	for (int s = 0;s < SerialStrLen; s++)
	{
		//lcd.print(SerialStr[s]);
		Serial.print(SerialStr[s]);
	}	  	
}


