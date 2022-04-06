#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
const byte interruptPin = 2;
volatile byte start_measuring = LOW;

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }

  uint32_t currentFrequency;
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  Serial.println("Power measure with  INA219 - MIC90");
  Serial.println("Configuring IRQ pin");
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), irq_f, CHANGE);
}

void irq_f(void){
  start_measuring = !start_measuring; 
}
  
void start_meas(void){
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  float iter = 1;  

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  
  Serial.print("Measuring power continuously...");
  while(start_measuring == HIGH){
    power_mW += ina219.getPower_mW();
    iter++;
    delay(10);
  }
  Serial.println("Finished");

  Serial.print("Average power:"); Serial.print(power_mW/iter); Serial.println(" mW");
  Serial.print("Iterations:"); Serial.println(iter);
}
  
void loop(void) 
{
  if (start_measuring == HIGH){
    start_meas();
  }
}
