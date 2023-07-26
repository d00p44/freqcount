#include "ESPboyInit.h"
#include <Adafruit_NeoPixel.h>
ESPboyInit myESPboy;


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
  mcp2.begin(1); // Init MCP23017 at address 0x21

    for (byte i=8; i<11; i++) {
        mcp2.pinMode(i, OUTPUT);
    }
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
*/
	myESPboy.tft.setCursor(10, 10);
	myESPboy.tft.setTextSize(2);
	char* fr = ("Frequenz: ");
  char* ks = ("kein Signal");
  myESPboy.tft.setTextColor(TFT_BLUE);
  myESPboy.tft.drawString (fr, (128-(strlen(fr)*12))/2, 4);
  String freq = Serial.readString();
  int laenge = freq.length();
   freq.trim();
   float flfreq = 0.0;
  
     	
switch (laenge) {
  case 0:
    myESPboy.tft.setTextColor(TFT_RED);
    myESPboy.tft.setTextSize(1);
    myESPboy.tft.drawString (ks, (128-(strlen(ks)*6))/2, 60);
    strip.setPixelColor(0, 255, 0, 0);
    // show all pixels  
    strip.show();
    break;
  case 5:
    //String freq1 = String("%.06f MHz",flfreq);
    flfreq = freq.toFloat()/100000.0;
    myESPboy.tft.println(flfreq,4);
    myESPboy.tft.setCursor(55, 70);
    myESPboy.tft.println("kHz");
    break;
  case 6:
    //String freq1 = String("%.06f MHz",flfreq);
    flfreq = freq.toFloat()/100000.0;
    myESPboy.tft.println(flfreq,5);
    myESPboy.tft.setCursor(55, 70);
    myESPboy.tft.println("kHz");
    break;  
  case 7:
    //String freq1 = String("%.06f MHz",flfreq);
    flfreq = freq.toFloat()/1000000.0;
    myESPboy.tft.println(flfreq,6);
    myESPboy.tft.setCursor(55, 70);
    myESPboy.tft.println("MHz");
    strip.setPixelColor(0, 0, 255, 0);
    // show all pixels  
    strip.show();
    break;
   case 8:
    //String freq1 = String("%.06f MHz",flfreq);
    flfreq = freq.toFloat()/1000000.0;
    myESPboy.tft.println(flfreq,7);
    myESPboy.tft.setCursor(55, 70);
    myESPboy.tft.println("MHz");
    strip.setPixelColor(0, 0, 0, 255);
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
for (byte i=8; i<11; i++) {
       mcp2.digitalWrite(i, HIGH);
    }
    delay(500);

    for (byte i=8; i<11; i++) {
       mcp2.digitalWrite(i, LOW);
    }

  delay(500);
 //}

 myESPboy.tft.fillScreen(TFT_BLACK);
 
}

