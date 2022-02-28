#include <Arduino.h>
#include <avr/power.h>
#include <RH_ASK.h>
#include <SPI.h>

#define LED_tx 6
RH_ASK rf_driver; //default tx pin is 12


void setup() {
  start:
  Serial.begin(9600);
  Serial.println("Turn ON");
  power_twi_disable();
  power_timer2_disable();
  pinMode(A0,INPUT);
  pinMode(LED_tx,OUTPUT);
  if(!rf_driver.init()){
    Serial.println("RF Failed");
    goto start;
  }
}

void loop() {
  // const byte x[100] ={21,23,42,34,54,23,23,12,32,43,2,65,43,12,65,12,32,76,43,65,85,37};
  const byte x = 31;
  // rf_driver.send((uint8_t *)x[1],sizeof(x[1])); 
  if(!rf_driver.send((uint8_t *)x,sizeof(x))){
    Serial.println('not sent');
  }
  rf_driver.waitPacketSent();
  digitalWrite(LED_tx,HIGH);
  delay(500);
  digitalWrite(LED_tx,LOW);
  delay(500);
  Serial.println("SENT");
}