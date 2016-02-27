#include <XBee.h>
#include <ArduinoJson.h>

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

  Serial.begin(57600);
  xbee.setSerial(Serial);
}

void loop() {
  xbee.readPacket();

  if(xbee.getResponse().isAvailable()){
    if(xbee.getResponse().getApiId() == ZB_RX_RESPONSE){
      xbee.getResponse().getZBRxResponse(rx);

      // Get payload
      uint8_t* payload = rx.getData(); 

      StaticJsonBuffer<80> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject((char*) payload);

      if (root.success())
      {
        int red = root["red"];
        int green = root["green"];
        int blue = root["blue"];

        analogWrite(redPin, red);
        analogWrite(greenPin, green);
        analogWrite(bluePin, blue);
      }
    }
  }
}
