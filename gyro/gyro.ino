#include <avr/sleep.h>
#include <XBee.h>
#include <Wire.h>
#include <Adafruit_L3GD20.h>

// Define global variables
Adafruit_L3GD20 gyro;
const int interruptPin = 3;
const int xbeeCts = 11;
const int xbeeWake = 12;

XBee xbee = XBee();

void setup() {
  Serial.begin(57600);
  gyro.begin(gyro.L3DS20_RANGE_250DPS);

  // Set gyroscope to normal mode and enable x, y, z data 
  gyroConfig(gyro.L3GD20_REGISTER_CTRL_REG1, 0b00001111);

  // Enable the interrupt pin
  gyroConfig(gyro.L3GD20_REGISTER_CTRL_REG3, 0b10000000);

  // Enable interrupt for high values
  gyroConfig(gyro.L3GD20_REGISTER_INT1_CFG, 0b00101010);

  // Set X, Y, Z interrupt threshold to 10
  gyroConfig(gyro.L3GD20_REGISTER_TSH_XH, 0x0A);
  gyroConfig(gyro.L3GD20_REGISTER_TSH_YH, 0x0A);
  gyroConfig(gyro.L3GD20_REGISTER_TSH_ZH, 0x0A);

  xbee.setSerial(Serial);
  pinMode(xbeeCts, INPUT);
  pinMode(xbeeWake, OUTPUT);
  digitalWrite(xbeeWake, LOW);
}

void loop() {
  readGyro();
  delay(100);

  if (abs(gyro.data.x) <= 10 && abs(gyro.data.y) <= 10 && abs(gyro.data.z <= 10)) {
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);
    delay(100);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(interruptPin));
  }
}

void gyroConfig(byte reg, byte value) {
  Wire.beginTransmission(L3GD20_ADDRESS);
  Wire.write(reg);
  Wire.write(value); 
  Wire.endTransmission(); 
}

void handleInterrupt(){
}

void readGyro(){
  gyro.read();

  // Create char array of values in format [-]xxx,[-]xxx,[-]xxx
  char xyz[14];
  sprintf(xyz, "%+04d,%+04d,%+04d", (int) gyro.data.x, (int) gyro.data.y, (int) gyro.data.z);

  // Convert char array to uint8_t array
  uint8_t payload[sizeof(xyz)];
  memcpy(payload, (uint8_t*) xyz, sizeof(xyz));

  // Send payload with 
  XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);
  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));

  // Wait for XBee module
  while(digitalRead(xbeeCts) == HIGH);

  xbee.send(zbTx);
}
