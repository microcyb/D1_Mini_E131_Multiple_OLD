/*
* D1_Mini_E131_Multiple.ino -
* Sketch to handle mutiple data outputs for E1.31 data on an ESP8266
* to drive WS2811/WS2812 LEDs using the FastLED Library
*
* Requires FastLED - https://github.com/FastLED/FastLED
* Requires E131 - https://github.com/forkineye/E131
*/

#define FASTLED_SHOW_CORE 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include <ESP8266WiFi.h>
#include <E131.h> // Requires E131 - https://github.com/forkineye/E131
#include <FastLED.h> //Requires FastLED - https://github.com/FastLED/FastLED
FASTLED_USING_NAMESPACE

// WIFI SETUP
const char ssid[] = "SSID";
const char passphrase[] = "PASSPHRASE";

// USED FOR E131_MULTICAST
#define UNIVERSE		1
#define	UNIVERSE_COUNT		6

// SETUP UNIVERSE/CHANNEL PER GPIO
#define UNIVERSE_D1		1
#define CHANNEL_START_D1 	1

#define UNIVERSE_D2		2
#define CHANNEL_START_D2 	510

#define UNIVERSE_D5		3
#define CHANNEL_START_D5 	1020

#define UNIVERSE_D6		4
#define CHANNEL_START_D6 	1530

#define UNIVERSE_D7		5
#define CHANNEL_START_D7 	2040

#define UNIVERSE_D8		6
#define CHANNEL_START_D8 	2550

// IMPORTANT: SET THE MAX MILLI-AMPS OF YOUR POWER SUPPLY (4A = 4000MA)
#define MILLI_AMPS  		60000
#define VOLTS			5
uint8_t BRIGHTNESSINDEX = 	20;

// SETUP THE NUMBER OF PIXELS PER GPIO
#define D1_NUM_LEDS		170
#define D2_NUM_LEDS		170
#define D5_NUM_LEDS		170
#define D6_NUM_LEDS		170
#define D7_NUM_LEDS		170
#define D8_NUM_LEDS		170

// SET UP PIXEL TYPE WS2812B/GRB OR WS2811/RGB
#define LED_TYPE_D1    		WS2812B
#define COLOR_ORDER_D1 		GRB
#define LED_TYPE_D2    		WS2811
#define COLOR_ORDER_D2 		RGB
#define LED_TYPE_D5    		WS2811
#define COLOR_ORDER_D5 		RGB
#define LED_TYPE_D6    		WS2811
#define COLOR_ORDER_D6 		RGB
#define LED_TYPE_D7    		WS2811
#define COLOR_ORDER_D7 		RGB
#define LED_TYPE_D8    		WS2811
#define COLOR_ORDER_D8 		RGB

// DEFINE CRGB LEDS PER PIN GPIO
CRGB leds_D1[D1_NUM_LEDS];
CRGB leds_D2[D2_NUM_LEDS];
CRGB leds_D5[D5_NUM_LEDS];
CRGB leds_D6[D6_NUM_LEDS];
CRGB leds_D7[D7_NUM_LEDS];
CRGB leds_D8[D8_NUM_LEDS];

// SET THE BRIGHTNESS FOR EACH GPIO
#define BRIGHTNESS_D1			20
#define BRIGHTNESS_D2			20
#define BRIGHTNESS_D5			20
#define BRIGHTNESS_D6			20
#define BRIGHTNESS_D7			20
#define BRIGHTNESS_D8			20

E131 e131;

void setup() {
	Serial.begin(115200);
	delay(1000);
	// INITALIZE DATA OUTPUTS FOR FASTLED
	FastLED.setBrightness(BRIGHTNESSINDEX);
	FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MILLI_AMPS);
	FastLED.addLeds<LED_TYPE_D1,D1,COLOR_ORDER_D1>(leds_D1, D1_NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE_D2,D2,COLOR_ORDER_D2>(leds_D2, D2_NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE_D5,D5,COLOR_ORDER_D5>(leds_D5, D5_NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE_D6,D6,COLOR_ORDER_D6>(leds_D6, D6_NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE_D7,D7,COLOR_ORDER_D7>(leds_D7, D7_NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE_D8,D8,COLOR_ORDER_D8>(leds_D8, D8_NUM_LEDS).setCorrection(TypicalLEDStrip);
	// CONNECT TO WIFI
	WiFi.mode(WIFI_STA);
	Serial.println("");
	Serial.print(F("Connecting to "));
	Serial.print(ssid);
	if (passphrase != NULL)
		WiFi.begin(ssid, passphrase);
	else
		WiFi.begin(ssid);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print(F("Connected with IP: "));
	Serial.println(WiFi.localIP());

	e131.begin(ssid, passphrase);

	// Listen via Unicast
	// if (e131.begin(E131_UNICAST))
	// Listen via Multicast
//	if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT))
//		Serial.println(F("Listening for data..."));
//	else
//		Serial.println(F("*** e131.begin failed ***"));
//	return WiFi.status();
}

void loop() {
	if(e131.parsePacket()) {
		if (e131.universe == UNIVERSE_D1) {
			for (int i = 0; i < D1_NUM_LEDS; i++) {
				int j = i * 3 + (CHANNEL_START_D1 - 1);
				leds_D1[i].setRGB(e131.data[j], e131.data[j+1], e131.data[j+2]);
			}
		FastLED[0].showLeds(BRIGHTNESS_D1);
		}

		if (e131.universe == UNIVERSE_D2) {
			for (int i = 0; i < D2_NUM_LEDS; i++) {
				int j = i * 3 + (CHANNEL_START_D2 - 1);
				leds_D2[i].setRGB(e131.data[j], e131.data[j+1], e131.data[j+2]);
			}
		FastLED[1].showLeds(BRIGHTNESS_D1);
		}

		if (e131.universe == UNIVERSE_D5) {
			for (int i = 0; i < D5_NUM_LEDS; i++) {
				int j = i * 3 + (CHANNEL_START_D5 - 1);
				leds_D5[i].setRGB(e131.data[j], e131.data[j+1], e131.data[j+2]);
			}
		FastLED[2].showLeds(BRIGHTNESS_D5);
		}

		if (e131.universe == UNIVERSE_D6) {
			for (int i = 0; i < D6_NUM_LEDS; i++) {
				int j = i * 3 + (CHANNEL_START_D6 - 1);
				leds_D6[i].setRGB(e131.data[j], e131.data[j+1], e131.data[j+2]);
			}
		FastLED[3].showLeds(BRIGHTNESS_D6);
		}

		if (e131.universe == UNIVERSE_D7) {
			for (int i = 0; i < D7_NUM_LEDS; i++) {
				int j = i * 3 + (CHANNEL_START_D7 - 1);
				leds_D7[i].setRGB(e131.data[j], e131.data[j+1], e131.data[j+2]);
			}
		FastLED[4].showLeds(BRIGHTNESS_D7);
		}

		if (e131.universe == UNIVERSE_D8) {
			for (int i = 0; i < D8_NUM_LEDS; i++) {
				int j = i * 3 + (CHANNEL_START_D8 - 1);
				leds_D8[i].setRGB(e131.data[j], e131.data[j+1], e131.data[j+2]);
			}
		FastLED[5].showLeds(BRIGHTNESS_D8);
		}
	}
}

