#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "LowPower.h"

#define DEVICE_ID "AgriWeather1"

#define DEBUG 1
#define LORA 1

#define ONE_WIRE_BUS_1 2
#define ONE_WIRE_BUS_2 3

OneWire oneWire_1(ONE_WIRE_BUS_1);
OneWire onewire_2(ONE_WIRE_BUS_2);

DallasTemperature temp_sensor_1(&oneWire_1);
DallasTemperature temp_sensor_2(&onewire_2);

SoftwareSerial lora(10, 9);

float temp_1 = 0;
float temp_2 = 0;
float moisture_1 = 0;
float moisture_2 = 0;
float voltage = 0;

void read_temp_1() {
  temp_sensor_1.requestTemperatures();
  temp_1 = temp_sensor_1.getTempCByIndex(0);

  if (temp_1 == -127) {
    temp_1 = 0;
  }
}

void read_temp_2() {
  temp_sensor_2.requestTemperatures();
  temp_2 = temp_sensor_2.getTempCByIndex(0);

  if (temp_2 == -127) {
    temp_2 = 0;
  }
}

void readMositureA0() {
  moisture_1 = analogRead(A0);
}

void readMositureA1() {
  moisture_2 = analogRead(A1);
}

void readBatteryVoltage() {
  voltage = analogRead(A7);
}

void sleepTenSeconds() {
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}

void setup() {
  Serial.begin(9600);
  lora.begin(9600);
  temp_sensor_1.begin();
  temp_sensor_2.begin();
}

void loop() {
  String string = "";
 
  read_temp_1();
  read_temp_2();
  readMositureA0();
  readMositureA1();
  readBatteryVoltage();

  string = (String)DEVICE_ID + "," + temp_1 + "," + temp_2 + "," + moisture_1 + "," + moisture_2 + "," + voltage;

  PRINTLN(string);

  sleepTenSeconds();
}

void PRINTLN(String str) {
  if (DEBUG) {
    Serial.println(str);
  }
  if (LORA) {
    lora.println(str);
  }
}
