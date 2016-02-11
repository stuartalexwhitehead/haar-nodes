#include <XBee.h>

const int redPin = 5;
const int greenPin = 11;
const int bluePin = 6;

XBee xbee = XBee();
XBeeResponse response = XBeeResponse(); 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop() {
  xbee.readPacket();

  if(xbee.getResponse().isAvailable()){
    if(xbee.getResponse().getApiId() == ZB_RX_RESPONSE){
      xbee.getResponse().getZBRxResponse(rx);

      // Get payload
      uint8_t* payload = rx.getData(); 

      // Get red value
      char redDigits[4];
      redDigits[0] = payload[0];
      redDigits[1] = payload[1];
      redDigits[2] = payload[2];
      redDigits[3] = '\0';
      int red = atoi(redDigits);

      // Get green value
      char greenDigits[4];
      greenDigits[0] = payload[4];
      greenDigits[1] = payload[5];
      greenDigits[2] = payload[6];
      greenDigits[3] = '\0';
      int green = atoi(greenDigits);

      // Get blue value
      char blueDigits[4];
      blueDigits[0] = payload[8];
      blueDigits[1] = payload[9];
      blueDigits[2] = payload[10];
      blueDigits[3] = '\0';
      int blue = atoi(blueDigits);

      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);
    }
  }
}
