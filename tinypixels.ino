/*
 * tinypixels
 * attiny85 neopixel lamp
 * two potentiomers for hue / display speed and brightness
 * one tactile switch for display mode
 * upload using Attiny85 w/internal 8MHZ Clock !!
*/
#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 4
#define MODE_BTN 1
#define POT_1 2
#define POT_2 3
#define HYSTERESIS 15
#define TOTAL_DISPLAY_MODES 3
#define DISPLAY_RATE_MIN 1
#define DISPLAY_RATE_MAX 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int num_active_pixels = strip.numPixels();
int display_mode = 0;
int display_rate = 0;
int roygbiv[][3] = {{255,5,5},{255,155,5},{255,255,5},{25,255,5},{48,59,210},{147,48,210},{169,43,196}};
int16_t red = 0;
int16_t green = 0;
int16_t blue = 0;
int16_t last_pot_one_val = -16;
int16_t last_pot_two_val = -16;

void setup() {
	pinMode(MODE_BTN, INPUT);
	strip.setBrightness(100);
	strip.begin();
	strip.show(); 
	delay(100);
}

void loop() {
	
	checkButton();
	
	checkPots();
	
	switch(display_mode) {
		case 1:
			setSolidRGB(red, green, blue);
			delay(50);
			break;
		case 2:
			randomWipes();
			break;
		case 3:
			rainbowSmear();
			break;
		default:
			setSolidRGB(red, green, blue);
			delay(50);
	}
}


/*
 * checkButton
*/
void checkButton() {

	if (digitalRead(MODE_BTN) == HIGH)  {

		//empty to show press is caught
		//emptyStrip();
		//strip.show();
		
		iterateDisplayMode();
	
	}
}

/*
 * checkPots
*/
void checkPots() {
	
	int16_t pot_one_val;
	int16_t pot_two_val;
	
	pot_one_val = analogRead(0);
	pot_two_val = analogRead(1);
	
	int16_t brightness;
	int16_t hue;
	
	//pot 1
	if (((pot_one_val - last_pot_one_val) > HYSTERESIS) || ((last_pot_one_val - pot_one_val) > HYSTERESIS)) {
		
		if(display_mode == 0) {
		
			hue = map( pot_one_val, 0, 1023, 0, 255);
		
			setSolidColor( Wheel(hue) );
	
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
	
	//setSolidWheelColor( (255 / TOTAL_DISPLAY_MODES) * display_mode );
	//setSolidRGB( roygbiv[display_mode][0], roygbiv[display_mode][1], roygbiv[display_mode][2] );
	//strip.show();

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
		delay(display_rate * 10 + 5);
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
 * set all leds to random color for "wait" long
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
		delay(display_rate * 50 + 5);
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
