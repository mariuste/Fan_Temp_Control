// Fan_Temp_Control.ino ##############################################
/* This code was written to control the temperature inside a server cabinet
by measuring the temperature with a DS18b20 Temperature probe and
outputting a PWM signal with the Arduino Nano to control a 4-Pin fan.
Unlike the cheap control boards from Amazon or Ebay this code switches
the fan off when the temperature is low enough. */


/*

The following constants should be changed according to the use case:

constant (default value) - description

tempLow (35) - Below this temperature (minus half hysteresis) the fan shuts off.
               It shuts on again at this temperature plus half hysteresis

tempHigh (50) - At and above this temperature the fan is at maximum speed

hyteresis (5) - Hysteresis to prevent frequent on/off switching at the threshold

minDuty (10) - Minimum fan speed to prevent stalling
maxDuty (100) - Maximum fan speed to limit noise

 */


#include <OneWire.h>
#include <DallasTemperature.h>

// Digital pin of temperature sensor
#define ONE_WIRE_BUS 2

// Setup a oneWire instance
OneWire oneWire(ONE_WIRE_BUS);  

// Setup temperature sensor library
DallasTemperature sensors(&oneWire);

// PWM output pin
const byte OC1B_PIN = 10;

// how frequently the main loop runs
const int tempSetInterval = 5000;


// temperatur settings
const float tempLow = 35;
const float tempHigh = 50;
const float hyteresis = 5;
const int minDuty = 10;
const int maxDuty = 100;

// state on/off of Fan
bool fanState = HIGH;

// current duty cycle
byte duty = 100;

// new duty cycle
byte newDuty = 100;




void setup() {
	//enable output for Timer 1
	pinMode(OC1B_PIN,OUTPUT);
	setupTimer1();


	// start serial port 
	Serial.begin(9600); 
	
	// Start up the temperature library 
	sensors.begin();
	sensors.requestTemperatures();


	// welcome message
	Serial.println("## Start of Program ##");
	Serial.println();

	Serial.println("# Connections #");

	Serial.println(" Temperature Sensor (VCC, Data, GND)");
	Serial.print(  "            Arduino: 3V3, D");
	Serial.print(ONE_WIRE_BUS);
	Serial.println("  , GND");
	Serial.println("            *additionally 4k7 pullup between VCC and Data");
	Serial.println();

	Serial.println(" 4-Pin Fan (GND, VCC, Sense, Control)");
	Serial.print(  "   Arduino: GND, 12V, n/C  , D");
	Serial.println(OC1B_PIN);
	Serial.println();

	Serial.println("# Settings #");
	Serial.println(" Below this temperature (minus half hysteresis) the fan");
	Serial.println(" shuts off. It enables again at this temperature plus half hysteresis:");
	Serial.print("  tempLow: "); Serial.print(tempLow); Serial.println("°C");

	
	Serial.println(" At and above this temperature the fan is at maximum speed: ");
	Serial.print("  tempHigh: "); Serial.print(tempHigh); Serial.println("°C");
	Serial.println();
	
	Serial.println(" Between these two temperatures the fan is regulated from");
	Serial.println(" the minimum fan speed to maximum fan speed");
	Serial.println();
	
	Serial.println(" Hysteresis to prevent frequent on/off switching at the threshold");
	Serial.print("  hyteresis: "); Serial.print(hyteresis); Serial.println("°C");
	Serial.println();
	
	Serial.println(" Minimum fan speed to prevent stalling");
	Serial.print("  minDuty: "); Serial.print(minDuty); Serial.println(" %");
	Serial.println();

	Serial.println(" Maximum fan speed to limit noise");
	Serial.print("  maxDuty: "); Serial.print(maxDuty); Serial.println(" %");
	Serial.println();

	Serial.println(" The fan speed is adjusted at the following interval:");
	Serial.print("  tempSetInterval: "); Serial.print(tempSetInterval); Serial.println(" ms");


	Serial.println(); delay(100);
	Serial.println(); delay(100);
	Serial.println(); delay(100);
	Serial.println(); delay(100);
	Serial.println(); delay(100);
	Serial.println(); delay(100);
	Serial.println(); delay(100);

	Serial.println("# Main Loop");
	Serial.println("(temperature, state, Duty Cycle, Fan On/Off)");
	Serial.println();

}


// main loop ##############################################
void loop() {
	// measure temperature, calculate Duty cycle, set PWM
	tempToPwmDuty();
	// wait for a bit
	delay(tempSetInterval);
}




// setting PWM ############################################
void setPwmDuty() {
	if (duty == 0) {
		fanState = LOW;
	} else if (duty > 0) {
		fanState = HIGH;
	}

	setFan(duty);

}


// calculate new PWM ######################################
void tempToPwmDuty() {

	sensors.requestTemperatures();

	float temp = sensors.getTempCByIndex(0);

	Serial.print(temp);
	Serial.print("°C, ");

	if (temp < tempLow) {
		// distinguish two cases to consider hyteresis
		if (fanState == HIGH) {
			if (temp < tempLow - (hyteresis / 2) ) {
				// fan is on, temp below threshold minus hysteresis -> switch off
				Serial.print("a, ");
				newDuty = 0;
			} else {
				// fan is on, temp not below threshold minus hysteresis -> keep minimum speed
				Serial.print("b, ");
				newDuty = minDuty;
			}
		} else if (fanState == LOW) {
			// fan is off, temp below threshold -> keep off
			Serial.print("c, ");
			newDuty = 0;
		}

	} else if (temp < tempHigh) {
		// distinguish two cases to consider hyteresis
		if (fanState == HIGH) {
			// fan is on, temp above threshold > control fan speed
			Serial.print("d, ");
			newDuty = map(temp, tempLow, tempHigh, minDuty, maxDuty);
		} else if (fanState == LOW) {
			if (temp > tempLow + (hyteresis / 2) ) {
				// fan is off, temp above threshold plus hysteresis -> switch on
				Serial.print("e, ");
				newDuty = minDuty;
			} else {
				// fan is on, temp not above threshold plus hysteresis -> keep off
				Serial.print("f, ");
				newDuty = 0;
			}
		}
	} else if (temp >= tempHigh) {
		// fan is on, temp above maximum temperature -> maximum speed
		Serial.print("g, ");
		newDuty = maxDuty;
	} else {
		// any other temperature -> maximum speed (this case should never occur)
		Serial.print("h, ");
		newDuty = maxDuty;
	}

 	//set new duty
 	duty = newDuty;

	Serial.print(duty);
  	Serial.print("%, ");

 	if (fanState==0) {Serial.println("OFF");} else {Serial.println("ON");}
	setPwmDuty();
}


// Setup Timer control
void setupTimer1(){
    //Set PWM frequency to about 25khz on pins 9,10 (timer 1 mode 10, no prescale, count to 320)
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << CS10) | (1 << WGM13);
    ICR1 = 320;
    OCR1A = 0;
    OCR1B = 0;
}
//equivalent of analogWrite on pin 10
void setFan(int fan){
	float f = fan;
	f = f / 100;
    f=f<0?0:f>1?1:f;
    OCR1B = (uint16_t)(320*f);
}