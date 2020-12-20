// Demo Code for ES1 H.Kikuchi

#include <M5Core2.h>
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <SparkFun_SCD30_Arduino_Library.h>
#include "SHT3X.h"
SCD30 airSensor;
SHT3X sht30;

float tmp = 0.0;
float hum = 0.0;

void m5restartcheck() {
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(0xF8A0, BLACK);
  M5.Lcd.setCursor(0, 140, 4);
  M5.Lcd.println("Restart M5?");
  M5.Lcd.setCursor(33, 212, 4);
  M5.Lcd.print("YES");
  M5.Lcd.setCursor(246, 212, 4);
  M5.Lcd.print("NO");
  bool touchstate = false;

  while(touchstate != true)
  {
    M5.update();
    if(M5.BtnA.wasPressed()) M5.shutdown(1);
    if(M5.BtnC.wasPressed()) touchstate=true;
  }
}

void sensorresetcheck() {
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(0xF8A0, BLACK);
  M5.Lcd.setCursor(0, 140, 4);
  M5.Lcd.println("Reset SCD30?");
  M5.Lcd.setCursor(33, 220, 4);
  M5.Lcd.print("YES");
  M5.Lcd.setCursor(246, 220, 4);
  M5.Lcd.print("NO");
  bool touchstate = false;

  while(touchstate != true)
  {
    M5.update();
    if(M5.BtnA.wasPressed())
    {
      M5.Lcd.setCursor(0, 140, 4);
      M5.Lcd.println("Initiating SCD30 soft reset...");
      airSensor.sendCommand(0xD304);
      touchstate=true;
      delay(500);
    }
    if(M5.BtnC.wasPressed()) 
    {
      m5restartcheck();
      touchstate=true;
    }
  }
  M5.Lcd.fillScreen(BLACK);
}

void batterycheck() {
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(0xF8A0, BLACK);
  M5.Lcd.setCursor(0, 100, 4);
  M5.Lcd.print("Battery Voltage(V):");
  M5.Lcd.println(M5.Axp.GetBatVoltage(),1);
  if(M5.Axp.isACIN()){
        M5.Lcd.println("AC IN");
    }else{
        float power = M5.Axp.GetBatPower();
        M5.Lcd.printf("Battery powered(mA): %3.1f\r\n", power);
    }
  M5.Lcd.setCursor(236, 212, 4);
  M5.Lcd.print("BACK");
  bool touchstate = false;

  while(touchstate != true)
  {
    M5.update();
    if(M5.BtnC.wasPressed()) 
    {
      touchstate=true;
    }
  }
  M5.Lcd.fillScreen(BLACK);
}

void setup() {
  M5.begin(true, true, true, true);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(0xFCA0, BLACK);
  M5.Lcd.setCursor(0, 0, 4);
  Wire.begin();
  
  M5.Lcd.println("SHT30 test");
  Serial.println(F("ENV Unit(SHT30) test..."));
  if (sht30.get() == 1)
  {
    M5.Lcd.println("Could not find a valid SHT30. Please check wiring.");
    while (1);
  }

  M5.Lcd.println("SCD30 test");
  Serial.println(F("CO2 Unit(SCD30) test..."));
  if (airSensor.begin(Wire, true) == false)
  {
    M5.Lcd.println("SCD30 not detected. Please check wiring. Freezing...");
    while (1);
  }
  delay(200);
  M5.Lcd.fillScreen(BLACK);
}

void loop() {
  if(sht30.get()==0){
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  
  M5.update();
  if(M5.BtnA.wasPressed()) sensorresetcheck();
  if(M5.BtnB.wasPressed()) batterycheck();

  if (airSensor.dataAvailable())
  {
    Serial.printf("Temp: %2.1fC  Humidity: %2.1f%%\r\n", tmp, hum);
    M5.Lcd.setCursor(0, 0, 4);
    M5.Lcd.setTextColor(0xFBA0, BLACK);
    M5.Lcd.println("[SHT30]");
    M5.Lcd.printf(" Temp(C): %2.1f\r\n Humidity(%%): %2.1f%\r\n", tmp, hum);
    M5.Lcd.println("    ");
    M5.Lcd.setCursor(0, 95, 4);
    M5.Lcd.println("[SCD30]");
    M5.Lcd.print(" CO2(ppm): ");
    M5.Lcd.print(airSensor.getCO2());
    M5.Lcd.println("    ");
    M5.Lcd.print(" Temp(C): ");
    M5.Lcd.println(airSensor.getTemperature(), 1);
    M5.Lcd.print(" Humidity(%): ");
    M5.Lcd.println(airSensor.getHumidity(), 1);
    M5.Lcd.setTextColor(0xF8A0, BLACK);
    M5.Lcd.setCursor(20, 212, 4);
    M5.Lcd.print("Reset");
    M5.Lcd.setCursor(120, 212, 4);
    M5.Lcd.print("Battery");
  }
  delay(500);
}
