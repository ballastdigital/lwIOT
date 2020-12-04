#include <math.h>
#include "SeeedOLED.h"
#define DUST_SENSOR_PIN D4
#define SENSOR_READING_INTERVAL 30000
unsigned long lastInterval;
unsigned long lowpulseoccupancy = 0;
unsigned long last_lpo = 0;
unsigned long duration;

float ratio = 0;
float concentration = 0;

// setup() runs once, when the device is first turned on.
void setup() {
// Put initialization like pinMode and begin functions here.
// loop() runs over and over again, as quickly as it can execute.
Serial.begin(9600);

pinMode(DUST_SENSOR_PIN, INPUT);
lastInterval = millis();

//OLED
Wire.begin();
SeeedOled.init();
SeeedOled.clearDisplay();
SeeedOled.setNormalDisplay();
SeeedOled.setPageMode();
SeeedOled.setTextXY(2, 0);
SeeedOled.putString("SV");
SeeedOled.setTextXY(3, 0);
SeeedOled.putString("Livewire");
SeeedOled.setTextXY(4, 0);
SeeedOled.putString("Monitor");
}


void loop() {
  // The core of your code will likely live here.
	duration = pulseIn(DUST_SENSOR_PIN, LOW);
lowpulseoccupancy = lowpulseoccupancy + duration;
if ((millis() - lastInterval) > SENSOR_READING_INTERVAL)
{
  getDustSensorReadings();
  lowpulseoccupancy = 0;
  lastInterval = millis();
}
}
void getDustSensorReadings() {
    if (lowpulseoccupancy == 0)
  {
    lowpulseoccupancy = last_lpo;
  }
  else
  {
    last_lpo = lowpulseoccupancy;
  }
ratio = lowpulseoccupancy / (SENSOR_READING_INTERVAL * 10.0);
concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;
Serial.printlnf("LPO: %d", lowpulseoccupancy);
Serial.printlnf("Ratio: %f%%", ratio);
Serial.printlnf("concentration: %f pcs/L", concentration);
}

void updateDisplay(String airQuality)
{
 SeeedOled.clearDisplay();
if (concentration > 1)
 {
   SeeedOled.setTextXY(5, 0);
   SeeedOled.putString("Dust: ");
   SeeedOled.putNumber(concentration); // Cast our float to an int to make it more compact
   SeeedOled.putString(" pcs/L");
 }
}