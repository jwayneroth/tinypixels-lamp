/*
 * tinypixels
 * attiny85 neopixel lamp
 * two potentiomers for hue / display speed and brightness
 * one tactile switch for display mode
 * upload using Attiny85 w/internal 8MHZ Clock !!
*/
#include <Adafruit_NeoPixel.h>
#include "colorFunctions.h"

#define PIXEL_PIN 4
#define MODE_BTN 1
#define LED_PIN 0
#define POT_1 1
#define POT_2 3
#define HYSTERESIS 12
#define TOTAL_DISPLAY_MODES 4
#define DISPLAY_RATE_MIN 0
#define DISPLAY_RATE_MAX 7
#define DELAY_MIN 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int num_active_pixels = strip.numPixels();
int display_mode = 0;
int display_rate = DISPLAY_RATE_MIN;
int roygbiv[][3] = {{255,5,5},{255,155,5},{255,255,5},{25,255,5},{48,59,210},{147,48,210},{169,43,196}};
int16_t wheel_hue = 0;
int16_t last_pot_one_val = -13;
int16_t last_pot_two_val = -13;
int rgbValues[3];

void setup() {
	pinMode(MODE_BTN, INPUT);
	strip.setBrightness(100);
	strip.begin();
	strip.show(); 
	//delay(100);
}

void loop() {
	
	checkButton();
	
	checkPots();
	
	switch(display_mode) {
		case 0:
			//setSolidWheelColor( wheel_hue );
			setSolidRGB( rgbValues[0], rgbValues[1], rgbValues[2] );
			strip.show();
			break;
		case 1:
			setSolidWheelColor( wheel_hue );
			strip.show();
			break;
		case 2:
			randomWipes();
			break;
		case 3:
			rainbowSmear();
			break;
		default:
			//setSolidWheelColor( wheel_hue );
			setSolidRGB( rgbValues[0], rgbValues[1], rgbValues[2] );
			strip.show();
			display_mode = 0;
	}
}


/*
 * checkButton
*/
void checkButton() {

	if (digitalRead(MODE_BTN) == HIGH)  {

		solidWhite();
		
		delay(500);
		
		iterateDisplayMode();
		
	}
}

/*
 * checkPots
*/
void checkPots() {
	
	int16_t pot_one_val;
	int16_t pot_two_val;
	
	pot_one_val = analogRead(POT_1);
	pot_two_val = analogRead(POT_2);
	
	int16_t brightness;
	uint32_t ct;
	
	//pot 1
	if (((pot_one_val - last_pot_one_val) > HYSTERESIS) || ((last_pot_one_val - pot_one_val) > HYSTERESIS)) {
				
		if(display_mode == 0) {
		
			ct = map( pot_one_val, 0, 1023, 2000, 20000);
			rgbFromTemp(ct, rgbValues);
	
		}else if (display_mode == 1) {
		
			wheel_hue = map( pot_one_val, 0, 1023, 0, 255);
		
		}else{
		
			display_rate = map( pot_one_val, 0, 1023, DISPLAY_RATE_MIN, DISPLAY_RATE_MAX);
			
		}
		
		last_pot_one_val = pot_one_val;
			
	}
	
	//pot 2
	if (((pot_two_val - last_pot_two_val) > HYSTERESIS) || ((last_pot_two_val - pot_two_val) > HYSTERESIS)) {
				
		brightness = map( pot_two_val, 0, 1023, 0, 255);
		
		strip.setBrightness(brightness);
		strip.show();
		
		last_pot_two_val = pot_two_val;
			
	}

}

/*
 * iterateDisplayMode
*/
void iterateDisplayMode() {
	
	display_mode++;
	
	if(display_mode > (TOTAL_DISPLAY_MODES - 1)) {
		display_mode = 0;
	}

}

/* 
 * colorWipe
 * Fill the dots one after the other with a color
*/
void colorWipe(uint32_t c, uint8_t wait) {
	for(uint16_t i=0; i<num_active_pixels; i++) {
		strip.setPixelColor(i, c);
		strip.show();
		delay(wait);
	}
}

/* 
 * rainbowsSmear
 * display section of rainbow (smear_increment)
 * distributed over strip
*/
void rainbowSmear() {
	uint16_t i;
	int inc = 2;//smear_increment + 1;
	for( i = 0; i < (256 / 4 ); i++) {
		strip.setPixelColor( i % num_active_pixels, Wheel( (i * (inc * 4)) & 255 ));
		strip.show();
		delay(display_rate * 10 + DELAY_MIN);
	}
}

/*
 * rainbow
 * iterate over rainbow, all leds equal
*/
void rainbow(uint8_t wait) {
	uint16_t i, j;
	for(j=0; j<256; j++) {
		for(i=0; i<num_active_pixels; i++) {
			strip.setPixelColor(i, Wheel((i+j) & 255));
		}
		strip.show();
		delay(wait);
	}
}

/* 
 * solidWhite
*/
void solidWhite()
{
	for( int i=0; i<num_active_pixels; i++ ) {
		strip.setPixelColor(i, strip.Color(255,255,255));
		strip.show();
	}
}

/* 
 * randomSolid
*/
void randomSolid()
{
	int r = random(255);
	int g = random(255);
	int b = random(255);
	for( int i=0; i<num_active_pixels; i++ ) {
		strip.setPixelColor(i, strip.Color(r, g, b));
		strip.show();
	}
}

/*
 * rainbowCycle 
 * distribute rainbow over the strip
*/
void rainbowCycle(uint8_t wait) {
	uint16_t i, j;
	for(j=0; j<256*5; j++) { // 5 display_modes of all colors on wheel
		for(i=0; i< num_active_pixels; i++) {
			strip.setPixelColor(i, Wheel(((i * 256 / num_active_pixels) + j) & 255));
		}
	strip.show();
	delay(wait);
	}
}

/*
 * Wheel
 * Input a value 0 to 255 to get a color value.
 * The colours are a transition r - g - b - back to r.
*/
uint32_t Wheel(byte WheelPos) {
	if(WheelPos < 85) {
		return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}else if(WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}else {
		WheelPos -= 170;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
}

/* 
 * emptyStrip
*/
void emptyStrip() {
	for(int i=0; i<num_active_pixels; i++) {
		strip.setPixelColor(i, strip.Color(0, 0, 0));
	}
}

/*
 * randomWipes
*/
void randomWipes() {
	for(int i=0; i<5; i++) {
		for(int j=0;j<num_active_pixels;j++) {
			int r = random(255);
			int g = random(255);
			int b = random(255);
			strip.setPixelColor(j, strip.Color(r, g, b));
		}
		strip.show();
		delay(display_rate * 50 + DELAY_MIN);
	}
}

void setSolidColor( uint32_t c ) {
	for(int j=0;j<num_active_pixels;j++) 
	{
		strip.setPixelColor(j, c);
	}
}

void setSolidRGB( int r, int g, int b ) {
	for(int j=0;j<num_active_pixels;j++) 
	{
		strip.setPixelColor(j, strip.Color(r,g,b));
	}
}

void setSolidWheelColor( byte c ) {
	uint32_t w = Wheel(c);
	for(int j=0;j<num_active_pixels;j++) 
	{
		strip.setPixelColor(j, w);
	}
}

void setLED(byte onoff) {
	if(onoff) { 
		digitalWrite(LED_PIN, HIGH);
	}else{
		digitalWrite(LED_PIN, LOW);
	}
}
