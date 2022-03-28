#include <Arduino.h>
#include <avr/power.h>


#define LED_tx 6
#define pin_mode 3
#define pin_stop 4
#define ADC_VREV_mV 1100.0
#define ADC_RESOLUTION 1024.0

int val = 0;
int counter = 0;
int ledState = LOW;
unsigned long previousMillis = 0;

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
  byte x[128] = {21,45,42,34,54,96,41,12,32,43
                ,23,65,43,12,65,12,32,76,43,65
                ,85,37,42,32,54,34,23,65,47,87
                ,69,12,3,4,39,10,90,98,75,42
                ,56,74,35,46,76,58,34,132,129,221
                ,232,54,121,153,178,165,186,190,165,35
                };
  sendByte(x[val]);
  digitalWrite(LED_tx,HIGH);
  delay(100);
  digitalWrite(LED_tx,LOW);
  delay(100);
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

void blinkLED(int interval){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(LED_tx, ledState);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Turn ON");
  power_twi_disable();
  power_timer2_disable();
  power_spi_disable();
  pinMode(LED_tx,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(pin_mode,INPUT_PULLUP);
  pinMode(pin_stop,INPUT_PULLUP);
  mode:
  if (counter == 0){
    blinkLED(100);
  }else if (counter == 1){
    blinkLED(500);
  }
  if(digitalRead(pin_stop)==HIGH){
    if(digitalRead(pin_mode)==LOW){
      if(counter!=1)
      counter ++;
      else counter = 0;
      count:
      if(digitalRead(pin_mode)==LOW){
        goto count;
      }
    }
    goto mode;
  }
}

void loop() {
  BER_Send();

}