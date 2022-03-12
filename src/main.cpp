#include <Arduino.h>
#include <avr/power.h>



#define LED_tx 6
#define pin_mode 3
#define pin_stop 4
#define ADC_VREV_mV 3300.0
#define ADC_RESOLUTION 1024.0

int val = 0;

void sendByte(byte input){
  int i;
  
  for(i=0; i<20; i++){
    digitalWrite(8, HIGH);
    delayMicroseconds(500);
    digitalWrite(8, LOW);
    delayMicroseconds(500);
  }

  digitalWrite(8, HIGH);
  delayMicroseconds(3000);
  digitalWrite(8, LOW);
  delayMicroseconds(500);
  
    
  for(i=0; i<8; i++){
    if(bitRead(input,i)==1)
      digitalWrite(8, HIGH);
    else
      digitalWrite(8, LOW);
    delayMicroseconds(500);
  
    if(bitRead(input,i)==1)
      digitalWrite(8, LOW);
    else
      digitalWrite(8, HIGH);
    delayMicroseconds(500);
  }//i

  
  digitalWrite(8, LOW);
}

void BER_Send(){
  //BER calculation
  byte x[128] ={21,45,42,34,54,96,41,12,32,43
                        ,23,65,43,12,65,12,32,76,43,65
                        ,85,37,42,32,54,34,23,65,47,87
                        ,69,12,3,4,39,10,90,98,75,42
                        ,56,74,35,46,76,58,34,132,129,221
                        ,232,54,121,153,178,165,186,190,165,35
                        };
  sendByte(x[val]);
  digitalWrite(LED_tx,HIGH);
  delay(500);
  digitalWrite(LED_tx,LOW);
  delay(500);
  (val>=127)?val=0:val++;
  Serial.println("SENT");
}

float calculate_temp(){
  float value = analogRead(A0);
  float millivolt = value*(ADC_VREV_mV/ADC_RESOLUTION);
  float tempC = (millivolt/10);
  return tempC;
}

void SendTemp(){
  char temp[16];
  float C = calculate_temp();

}
void setup() {
  start:
  Serial.begin(9600);
  Serial.println("Turn ON");
  power_twi_disable();
  power_timer2_disable();
  pinMode(LED_tx,OUTPUT);
  pinMode(pin_mode,INPUT);
  pinMode(pin_stop,INPUT);
}

void loop() {
  BER_Send();
}