#include <Adafruit_NeoPixel.h>

#define PIN 0
#define LIGHT 2 //D4=A2
#define BTN 1

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);

int brightness = 10;
int wasOff = 0;

void setup() {
	
	pinMode(BTN, INPUT);
	
	strip.setBrightness(brightness);
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void checkButton() {
	if (digitalRead(BTN) == HIGH) {
		brightness += 10;
		if(brightness > 100) {
			brightness = 0;
		}
		strip.setBrightness(brightness);
		strip.show();
		delay(200);
	}
}

void loop() {
	if(analogRead(LIGHT) > 650) {
		
		if(wasOff == 1) {
			strip.setBrightness(brightness);
			strip.show();
			wasOff = 0;
		}
		
		checkButton();		
		
		//colorWipe(strip.Color(255, 0, 0), 50); // Red
		
		//checkButton();
		
		//colorWipe(strip.Color(0, 255, 0), 50); // Green
		
		//checkButton();
		
		//colorWipe(strip.Color(0, 0, 255), 50); // Blue
		
		//checkButton();
		
		//rainbow(20);
		
		//checkButton();
		
		rainbowCycle(3);
	
	}else{
	
		wasOff = 1;
		strip.setBrightness(0);
		strip.show();
		delay(300);
	
	}
	
	delay(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

