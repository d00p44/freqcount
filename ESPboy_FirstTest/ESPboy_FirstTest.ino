#include "ESPboyInit.h"
#include <Adafruit_NeoPixel.h>
ESPboyInit myESPboy;
// Define I/O pins

	#define GAL 1 // These pins select 1 of the 4 bytes of data to access
	#define GAU 2
	#define GBL 3
	#define GBU 4
	
	// Next 8 pins have the 8 bits of count data
	// Note I can read 0 thru 5 with PINC and 7 & 8 with PINB
	
	#define Y0 8 // A0 - A5 are PC0 - PC5
	#define Y1 9
	#define Y2 10
	#define Y3 11
	#define Y4 12
	#define Y5 13
	#define Y6 14
	#define Y7 15
	
	#define CE_Not 7 // V1.1x, pin formerly called latch, now enables clear
	#define C_state 8 // V1.1x, was 'clear' now monitors clock

  unsigned long freq = 1.5687;

Adafruit_MCP23017 mcp2;
uint16_t lcdbright = 4095;
 
// the data pin for the NeoPixels
int neoPixelPin = D4;

// How many NeoPixels we will be using, charge accordingly
int numPixels = 1;

// Instatiate the NeoPixel from the ibrary
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);
  
void setup(){
  myESPboy.begin("Freq Count Test");
  strip.begin();  // initialize the strip
  strip.show();   // make sure it is visible
  strip.clear();  // Initialize all pixels to 'off'
  strip.setBrightness(25);
  /*mcp2.begin(1); // Init MCP23017 at address 0x21
  mcp2.pinMode(GAL, OUTPUT);
	mcp2.pinMode(GAU, OUTPUT);
	mcp2.pinMode(GBL, OUTPUT);
	mcp2.pinMode(GBU, OUTPUT);
	mcp2.pinMode(CE_Not, OUTPUT);
	mcp2.pinMode(C_state, INPUT); // V1.1x, monitors clock state
 	mcp2.pinMode(Y0, INPUT);
	mcp2.pinMode(Y1, INPUT);
	mcp2.pinMode(Y2, INPUT);
	mcp2.pinMode(Y3, INPUT);
	mcp2.pinMode(Y4, INPUT);
	mcp2.pinMode(Y5, INPUT);
	mcp2.pinMode(Y6, INPUT);
	mcp2.pinMode(Y7, INPUT);
	// pinMode(spkrpin, OUTPUT);
	
	mcp2.digitalWrite(GAL, HIGH); // These 4 lines are active LOW
	mcp2.digitalWrite(GAU, HIGH);
	mcp2.digitalWrite(GBL, HIGH);
	mcp2.digitalWrite(GBU, HIGH);
	
	mcp2.digitalWrite(CE_Not, HIGH);
  //while(!mcp2.digitalRead(C_state)); // assume LOW
  //while(mcp2.digitalRead(C_state)); // now went to HIGH
  //while(!mcp2.digitalRead(C_state));*/
}


void loop(){ 
 uint8_t keypressed;
 static uint8_t rnd = 0;
/*int yk = 0;
 for (int i=0; i <= 4; i++){
    
     myESPboy.tft.setCursor(10, yk);
 String freq = Serial.readString();
 freq.trim();
     myESPboy.tft.println(freq);
     yk = yk+30;




     count();
*/


   
   

   myESPboy.tft.setRotation(0);
	myESPboy.tft.setCursor(10, 10);
	myESPboy.tft.setTextSize(2);
	char* fr = ("Frequenz: ");
  char* ks = ("kein Signal");
  myESPboy.tft.setTextColor(TFT_BLUE);
  myESPboy.tft.drawString (fr, (128-(strlen(fr)*12))/2, 4);
  //String freq = Serial.readString();
  
   String sfreq; 
  sfreq = String(freq);
   int laenge = sfreq.length();
   float flfreq;
 
   

switch (laenge) {
  default:
    myESPboy.tft.setTextColor(TFT_RED);
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawString (ks, (128-(strlen(ks)*6))/2, 60);
    strip.setBrightness(10);
    strip.setPixelColor(0, 255, 255, 0);
    // show all pixels  
    strip.show();
    break;
    case 1:
    flfreq = freq/1.0;
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawFloat(flfreq, 1,myESPboy.tft.drawString("Hz", 70, 60)+38,60);
    strip.setPixelColor(0, 0, 255, 255);
    strip.show();
    break;
    case 2:
    flfreq = freq/1.0;
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawFloat(flfreq, 2,myESPboy.tft.drawString("Hz", 80, 60)+20,60);
    strip.setPixelColor(0, 0, 0, 255);
    strip.show();
    break;
    case 3:
    flfreq = freq/1.0;
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawFloat(flfreq, 3,myESPboy.tft.drawString("Hz", 80, 60)+19,60);
    strip.setPixelColor(0, 127, 0, 255);
    strip.show();
    break;
    case 4:
    flfreq = freq/1000.0;
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawFloat(flfreq,4,myESPboy.tft.drawString("kHz", 80, 60)+15,60);
    strip.setPixelColor(0, 30, 255, 5);
    strip.show();
    break;
  case 5:
    flfreq = freq/1000.0;
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawFloat(flfreq, 5,myESPboy.tft.drawString("kHz", 80, 60)+10,60);
    strip.setPixelColor(0, 80, 120, 255);
    strip.show();
    break;
  case 6:
    flfreq = freq/1000.0;
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawFloat(flfreq, 6,myESPboy.tft.drawString("kHz", 80, 60)+8,60);
    strip.setPixelColor(0, 55, 25, 0);
    strip.show();

    break;  
  case 7:
  flfreq = freq/1000000.0;
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawFloat(flfreq, 7,myESPboy.tft.drawString("MHz", 80, 60)+8,60);
    strip.setBrightness(10);
    strip.setPixelColor(0, 0, 255, 8);
    // show all pixels  
    strip.show();
    break;
   case 8:
   flfreq = freq/1000000.0;
   myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawFloat(flfreq,8,myESPboy.tft.drawString("MHz", 80, 60)+8,60);
    strip.setPixelColor(0, 65, 0, 55);
    // show all pixels  
    strip.show();
    break;
    case 9:
    flfreq = freq/1000000.0;
   myESPboy.tft.setTextSize(2);
    myESPboy.tft.drawFloat(flfreq, 9,myESPboy.tft.drawString("MHz", 45, 80)-25,40);
    strip.setPixelColor(0, 10, 10, 120);
    // show all pixels  
    strip.show();
    break;

}

 /*keypressed=myESPboy.getKeys();

 if (keypressed){      
  
    if (rnd==0) {
      myLED.setRGB(100, 0, 0); 
      myESPboy.tft.setTextColor(TFT_RED);}
    if (rnd==1) {
      myLED.setRGB(0, 100, 0); 
      myESPboy.tft.setTextColor(TFT_GREEN); }
    if (rnd==2) {
      myLED.setRGB(0, 0, 100); 
      myESPboy.tft.setTextColor(TFT_BLUE); }
      
    myESPboy.tft.setCursor(0, 0);
    myESPboy.tft.setTextSize(2);
 
    myESPboy.tft.println("PRESSED:");
    if (keypressed&PAD_LEFT) myESPboy.tft.println("LEFT ");
    if (keypressed&PAD_RIGHT) myESPboy.tft.println("RIGHT");
    if (keypressed&PAD_UP) myESPboy.tft.println("UP   ");
    if (keypressed&PAD_DOWN) myESPboy.tft.println("DOWN ");
    if (keypressed&PAD_ACT) myESPboy.tft.println("ACT  ");
    if (keypressed&PAD_ESC) myESPboy.tft.println("ESC  ");
    if (keypressed&PAD_RGT) myESPboy.tft.println("RGT  ");
    if (keypressed&PAD_LFT) myESPboy.tft.println("LFT  ");
    
    myESPboy.playTone(keypressed*30);

    lcdbright += 100;    
    if (lcdbright > 4095) lcdbright = 3000;
    myESPboy.dac.setVoltage(lcdbright, false);
 }
 
 else{ 
    rnd=random(0,3);
    myESPboy.noPlayTone();
    myLED.setRGB(0, 0, 0); 
    myESPboy.tft.setCursor(0, 0);
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.print("Millis: "); 
    myESPboy.tft.println(millis());
    myESPboy.tft.setCursor(0, 8);
    myESPboy.tft.print("TFT brt:"); 
    myESPboy.tft.print(lcdbright); 
    myESPboy.dac.setVoltage(4095, false);
 }
 */


  
 //}
delay(200);
 myESPboy.tft.fillScreen(TFT_BLACK);

}

uint8_t get_byte()
	  {
		  uint8_t tempbyte;
		  
		  tempbyte = mcp2.readGPIO(1);
		  return tempbyte; // now we have the full 8 bits loaded
	  }
	
	// This routine will read all four bytes of frequency data and put
	// them in the long int 'freq'
	
	void read32()
	{

		mcp2.digitalWrite(GBU, LOW); // highest byte of four
		freq = (unsigned long) get_byte();
		freq = freq << 8;
		mcp2.digitalWrite(GBU, HIGH);
		
		mcp2.digitalWrite(GBL, LOW); // next highest
		freq = freq | (unsigned long) get_byte();
		freq = freq << 8;
		mcp2.digitalWrite(GBL, HIGH);
		
		mcp2.digitalWrite(GAU, LOW); // third - bits 8 - 15
		freq = freq | (unsigned long) get_byte();
		freq = freq << 8;
		mcp2.digitalWrite(GAU, HIGH);
		
		mcp2.digitalWrite(GAL, LOW); // lowest byte, b0:7
		freq = freq | (unsigned long) get_byte();
		mcp2.digitalWrite(GAL, HIGH);
		freq = freq*10;
		
	}
  void count(){
  while(mcp2.digitalRead(C_state)); // stay until LOW
	
	// Now a counting period has ended and data being latched
	delay(1); // allow 2 ms to latch before clear
	mcp2.digitalWrite(CE_Not, LOW); // allow low clock to reach -CCLR
	
	// Now, clock will determine when to remove clear signal
	
	//read32();
  while(!mcp2.digitalRead(C_state)); // Stay until HIGH and loop back
	
	mcp2.digitalWrite(CE_Not, HIGH);
  }

