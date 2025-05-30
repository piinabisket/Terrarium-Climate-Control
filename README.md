# Terrarium-Climate-Control
A full-scope climate control system for use with terrariums and vivariums. Automatically adjusts tank temperature, humidity, and light level based preconfigured conditions. Built on an Arduino Mega2560.

**Hardware Stack:**
  1x Arduino Mega 2560
  1x ILI9410 320x240 Resistive Touchscreen Display Shield
  2-4x DHT22 Temperature & Humidity Sensors
  1x Elegoo 4-channel relay OR 1x 8-channel relay
  1x DS3231 Real-Time-Clock (RTC) Module
  2x Micro USB atomizer

# Design
The intention of this project is to build an AIO terrarium climate control system suitable for terrariums of any shape and size, with and temperature and moisture needs.
Interfacing with the system is done entirely via the onboard touchscreen display, which provides active monitoring of up to two terrarium climates.
Two sensors are utilized to properly gauge the hot and cold side temperatures. The system is intended to be modular, and compatible with any off the shelf light
and heating element.
The system is fully configurable form the touchscreen, providing the following options:
  * Temperature units
  * Temperature setpoints
  * Temperature bias 
  * Humidity setpoints
  * Daylight hours

The setpoints act as minimum thresholds for the associated property. To elaborate, if the temperature setpoint is set to 70°F and the sensors detect temperatures under 70° 
for more than 5 minutes, it will switch on the heat lamp for 10 minutes. If the temperature still has not reached the setpoint, the lamp will remain on until it does. 
Overcompensating ensures the tank reaches a stable equilibrium. Humidity also works like this, ensuring that conditions remain optimal while not 
being overly aggressive on power or water consumption. 
The temperature bias setting determines which temperature value the setpoint should refer to; hot, cold, or average.
The RTC ensures smooth operation through power cycles.

The codebase is relatively portable, different touchscreen components should be plug and play. The Arduino should also be swappable for other off the shelf MCUs' 
with minimal code adjustment. Note that the Mega 2560 was chosen for its large onboard memory and ample additional I/O ports. While the system may be possible on an UNO, 
a MEGA or better is recommended for compatibility and usability with the resource-intensive display.


