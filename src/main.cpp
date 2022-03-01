#include <Arduino.h>
#include <avr/power.h>
#include <RH_ASK.h>
#include <SPI.h>

#define LED_tx 6
RH_ASK rf_driver; //default tx pin is 12

int i = 0;

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
    delay(1000);
    goto start;
  }
}

void loop() {
  const uint8_t x[22] ={21,23,42,34,54,23,23,12,32,43,2,65,43,12,65,12,32,76,43,65,85,37};
  // const uint8_t x = B101100;
  // rf_driver.send((uint8_t *)x[1],sizeof(x[1])); 
  rf_driver.send(&x[i],sizeof(x[i]));
  rf_driver.waitPacketSent();
  digitalWrite(LED_tx,HIGH);
  delay(500);
  digitalWrite(LED_tx,LOW);
  delay(500);
  (i>=21)?i=0:i++;
  Serial.println("SENT");

  // const char *msg = "Hello World";
  // rf_driver.send((uint8_t *)msg, strlen(msg));
  // rf_driver.waitPacketSent();
  // delay(1000);
}