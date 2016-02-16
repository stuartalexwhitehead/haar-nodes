# Haar Nodes
Haar is my honours year project; for more information on it, check out the [main repository](https://github.com/stuartalexwhitehead/haar). There are a number of related repositories:

- [Haar](https://github.com/stuartalexwhitehead/haar) - Project overview and WiP report
- [Haar Engine](https://github.com/stuartalexwhitehead/haar-engine) - IoT application framework
- [Haar API](https://github.com/stuartalexwhitehead/haar-api) - Implementation of Haar Engine
- [Haar Dashboard](https://github.com/stuartalexwhitehead/haar-dashboard) - Front-end UI for Haar API
- [Haar Bridge](https://github.com/stuartalexwhitehead/haar-bridge) - Translation between ZigBee WSN and the Internet
- [Haar Nodes](https://github.com/stuartalexwhitehead/haar-nodes) - Arduino sketches and companion libraries

This repository contains Arduino sketches for prototype end devices. I have developed four end devices for Haar.

## Temperature Sensor
### Main Components
- Arduino Pro Mini @ 3.3v
- XBee Series 2 w/ chip antenna
- [Texas Instruments TMP102](http://www.ti.com/product/TMP102) on SparkFun breakout board

### Functionality
This sensor will periodically wake up, measure the temperature and transmit to the XBee coordinator. The temperature is measured in degrees celsius and is accurate to 0.0652°C.

## RGB Colour Sensor
### Main Components
- Arduino Pro Mini @ 3.3v
- XBee Series 2 w/ chip antenna
- [Intersil ISL29125](http://www.intersil.com/content/dam/Intersil/documents/isl2/<isl29125 class="pdf"></isl29125>) on SparkFun breakout board

### Functionality
The ISL29125 measures the intensity of each RGB channel up to 10,000 lux. This sensor will periodically wake up and measure the intensity of each channel. Rather than transmitting these raw values, the sensor will scale the values down to a maximum of 255 to give an approximate RGB colour value.

## 3-axis Gyroscope Sensor
### Main Components
- Arduino Pro Mini @ 3.3v
- XBee Series 2 w/ chip antenna
- [STMicro L3GD20H](http://www.st.com/web/catalog/sense_power/FM89/SC1288/PF254039)

### Functionality
The L3GD20H measures the rate of change of angles in the X, Y and Z axis. The unit of measurement is degrees per second. Rather than wasting battery power, the sensor will sleep when no movement is detected. The L3GD20H will raise an interrupt on movement in any axis. The X, Y and Z values will then be transmitted to the XBee coodinator at a maximum rate of 10 packets per second. 

## RGB LED Actuator
### Main Components
- Arduino Pro Mini @ 3.3v
- XBee Series 2 w/ chip antenna
- 3x [Fairchild TIP120](https://www.fairchildsemi.com/datasheets/TI/TIP120.pdf) Darlington Transistors
- [STMicro LD1117V33](http://www.st.com/web/en/resource/technical/document/datasheet/CD00000544.pdf) 3.3v Voltage Regulator
- 40cm SMD 5050 Flexible RGB LED Strip

### Functionality
This actuator receives RGB values via the XBee. Using PWM outputs of the Arduino (switching 12v using the TIP120s), it will set the LED strip appropriately.
