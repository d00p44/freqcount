#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include <ESP8266WiFi.h>  
// Define I/O pins

	#define GAL 0 // These pins select 1 of the 4 bytes of data to access
	#define GAU 1
	#define GBL 2
	#define GBU 3
	
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
	
	#define CE_Not 6 // V1.1x, pin formerly called latch, now enables clear
	#define C_state 7 // V1.1x, was 'clear' now monitors clock

  unsigned long freq;
  unsigned long ref;
  unsigned long rezi;

Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;

void setup(){
  Serial.begin(9600); //serial init
  WiFi.mode(WIFI_OFF); // to safe battery power

  mcp2.begin(0); // Init MCP23017 at address 0x20
  mcp3.begin(4);
  for (int i=8;i<=15;i++){  
     mcp2.pinMode(i, INPUT);
  }
  for (int i=0;i<=6;i++){  
     mcp2.pinMode(i, OUTPUT);
     mcp2.pullUp(i, HIGH);
     }
     mcp2.pinMode(C_state, INPUT);

//MCP3 init  

  for (int i=8;i<=15;i++){  
     mcp3.pinMode(i, INPUT);
  }
  for (int i=0;i<=6;i++){  
     mcp3.pinMode(i, OUTPUT);
     mcp3.pullUp(i, HIGH);
     }
     mcp3.pinMode(C_state, INPUT);  

  delay(500);
  Serial.println("Zaehler laeuft");
  while(!mcp2.digitalRead(C_state)){yield();} 
  while(mcp2.digitalRead(C_state)){yield();} 
  while(!mcp2.digitalRead(C_state)){yield();}
  //Clock is Running
  delay(500);
  Serial.println("Zaehler laeuft");
  
}


void loop(){ 

   count();
   delay(1);
    rezi= ref*freq;
    
    Serial.println(freq);  
    Serial.println(ref); 
    
    Serial.println(rezi); 

}

uint8_t get_byte()
	  {
		  uint8_t tempbyte;
		  
		  tempbyte = mcp2.readGPIO(1);
		  return tempbyte; // now we have the full 8 bits loaded
	  }
	
	// This routine will read all four bytes of frequency data and put
	// them in the long int 'freq'
uint8_t get_byte_ref()
	  {
		  uint8_t tempbyte_ref;
		  
		  tempbyte_ref = mcp3.readGPIO(1);
		  return tempbyte_ref; // now we have the full 8 bits loaded
	  }

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
  void read32_ref()
	{

		mcp3.digitalWrite(GBU, LOW); // highest byte of four
		ref = (unsigned long) get_byte_ref();
		ref = ref << 8;
		mcp3.digitalWrite(GBU, HIGH);
		
		mcp3.digitalWrite(GBL, LOW); // next highest
		ref = ref | (unsigned long) get_byte_ref();
		ref = ref << 8;
		mcp3.digitalWrite(GBL, HIGH);
		
		mcp3.digitalWrite(GAU, LOW); // third - bits 8 - 15
		ref = ref | (unsigned long) get_byte_ref();
		ref = ref << 8;
		mcp3.digitalWrite(GAU, HIGH);
		
		mcp3.digitalWrite(GAL, LOW); // lowest byte, b0:7
		ref = ref | (unsigned long) get_byte_ref();
		mcp3.digitalWrite(GAL, HIGH);
		ref = ref*10;
		
	}
  void count(){
  while(mcp2.digitalRead(C_state)){yield();} // stay until LOW
	
	// Now a counting period has ended and data being latched
	delay(1); // allow 2 ms to latch before clear
	mcp2.digitalWrite(CE_Not, LOW); // allow low clock to reach -CCLR
  mcp3.digitalWrite(CE_Not, LOW);
	// Now, clock will determine when to remove clear signal
	
	read32();
  read32_ref();
  while(!mcp2.digitalRead(C_state)){yield();}// Stay until HIGH and loop back
	mcp2.digitalWrite(CE_Not, HIGH);
  mcp3.digitalWrite(CE_Not, HIGH);
  }


