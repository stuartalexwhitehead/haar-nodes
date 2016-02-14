#include <SparkFunISL29125.h>
#include <avr/sleep.h>
#include <XBee.h>
#include <Wire.h>

// Define global variables
SFE_ISL29125 rgbSensor;
const int xbeeCts = 11;
const int xbeeWake = 12;
const int sleepTotal = 8;
volatile int sleepCount = 0;
const int ledPin = 13;

XBee xbee = XBee();

void setup() {
  configureWatchdog();
  rgbSensor.init();
  Serial.begin(9600);
  xbee.setSerial(Serial);
  pinMode(xbeeCts, INPUT);
  pinMode(xbeeWake, OUTPUT);
  digitalWrite(xbeeWake, LOW);
}

/**
 * Measure the RGB channel values
 * 
 * The SparkFunISL29125 library returns a 16-bit integer.
 * Each channel is measured from 0 - 10000 lux.
 * We calculate RGB values based on their ratios.
 */
void loop() {
  goToSleep();

  if(sleepCount == sleepTotal){
    readColour();
    xbee.readPacket(10000);
    sleepCount = 0;
  }
}

/**
 * Put the MCU into a deep sleep
 */
void goToSleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

/**
 * Configure the Watchdog Timer
 * 
 * The Watchdog Timer triggers an interrupt capable of waking the Arduino from a deep sleep.
 * For more information on the Watchdog Control Register, see the ATmega datasheet.
 * 
 * WDTCSR = WDIF | WDIE | WDP3 | WDCE | WDE | WDP2 | WDP1 | WDP0
 */
void configureWatchdog(){
  MCUSR &= ~(1 << WDRF);                // Clear watchdog reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE);   // Allow config changes for 4 clock cycles
  WDTCSR = (1<< WDP0) | (1 << WDP3);    // Prescaler = 1024k
  WDTCSR |= (1 << WDIE);                // Enable Watchdog interrupt mode
}

/**
 * Handle the Watchdog interupt
 */
ISR(WDT_vect) {
  sleepCount++;
}

/**
 * XBee Transmission Request
 * 
 * ZigBee coordinators always have the 64 byte address 0x0000000000000000
 */
void readColour(){
  float redLux = (float) rgbSensor.readRed();
  float greenLux = (float) rgbSensor.readGreen();
  float blueLux = (float) rgbSensor.readBlue();

  // Calculate 0 - 255 value based on ratios
  float maxValue = 0;
  if (redLux > maxValue) {
    maxValue = redLux;
  }

  if (greenLux > maxValue) {
    maxValue = greenLux;
  }

  if (blueLux > maxValue) {
    maxValue = blueLux;
  }

  unsigned int red = (int) ((redLux / maxValue) * 255);
  unsigned int green = (int) ((greenLux / maxValue) * 255);
  unsigned int blue = (int) ((blueLux / maxValue) * 255);

  // Create char array of values in format xxx,xxx,xxx
  char rgb[11];
  sprintf(rgb, "%03d,%03d,%03d", red, green, blue);
  
  // Convert char array to uint8_t array
  uint8_t payload[sizeof(rgb)];
  memcpy(payload, (uint8_t*) rgb, sizeof(rgb));
  
  XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);
  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));

  // Wait for XBee module
  while(digitalRead(xbeeCts) == HIGH);

  xbee.send(zbTx);
}
