#include <Arduino.h>
#include <avr/power.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <EEPROM.h>

#define LED_tx 6
#define ADC_VREV_mV 3300.0
#define ADC_RESOLUTION 1024.0
RH_ASK rf_driver;//edit lib to txPin 8

int i = 0;

void BER_Send(){
  //BER calculation
  const uint8_t x[128] ={21,45,42,34,54,96,41,12,32,43
                        ,23,65,43,12,65,12,32,76,43,65
                        ,85,37,42,32,54,34,23,65,47,87
                        ,69,12,3,4,39,10,90,98,75,42
                        ,56,74,35,46,76,58,34,132,129,221
                        ,232,54,121,153,178,165,186,190,165,35
                        };
  // const uint8_t x = B101100;
  // rf_driver.send((uint8_t *)x[1],sizeof(x[1])); 
  rf_driver.send(&x[i],sizeof(x[i]));
  rf_driver.waitPacketSent();
  digitalWrite(LED_tx,HIGH);
  delay(500);
  digitalWrite(LED_tx,LOW);
  delay(500);
  (i>=127)?i=0:i++;
  Serial.println("SENT");
}

float calculate_temp(){
  float value = analogRead(A0);
  float millivolt = value*(ADC_VREV_mV/ADC_RESOLUTION);
  float tempC = (millivolt/10);
  return tempC;
}

void SendTemp(){
  // const char* temp = calculate_temp();
  // rf_driver.send(temp,sizeof(temp));
}
void setup() {
  start:
  Serial.begin(9600);
  Serial.println("Turn ON");
  power_twi_disable();
  power_timer2_disable();
  pinMode(LED_tx,OUTPUT);
  if(!rf_driver.init()){
    Serial.println("RF Failed");
    delay(1000);
    goto start;
  }
}

void loop() {
  BER_Send();
}