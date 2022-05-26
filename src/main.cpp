#include <Arduino.h>
#include <avr/power.h>


#define LED_tx 6
#define pin_mode 3
#define pin_stop 4
#define TEMP_VAR 1100.0
#define ADC_RESOLUTION 1024.0

int val = 0;
int x = 0;
int counter = 0;
int ledState = LOW;
unsigned long previousMillis = 0;
uint16_t input = 0;

void sendByte(uint16_t input){
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
  
    
  for(i=0; i<13; i++){
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
  // byte x[128] = {21,45,42,35,55,96,41,12,32,43
  //               ,23,65,43,12,65,12,32,76,43,65
  //               ,85,37,42,32,54,34,23,65,47,87
  //               ,69,12,35,4,39,10,90,98,75,42
  //               ,56,74,35,46,76,58,34,132,129,221
  //               ,232,54,121,153,178,165,186,190,165,30
  //               ,122,231,176,198,185,89,63,95,129,142
  //               ,176,186,184,98,90,76,72,28,30,10
  //               ,14,29,74,80,94,187,190,211,201,207
  //               ,19,28,25,15,54,86,219,200,95,83
  //               ,18,52,34,57,87,90,92,86,43,24
  //               ,121,100,82,94,87,92,63,71,80,89
  //               ,12,32,43,67,87,90,72,74
  //               };
  // sendByte(x[val]);
  // sendByte('}');
  // digitalWrite(LED_tx,HIGH);
  // delay(100);
  // digitalWrite(LED_tx,LOW);
  // delay(100);
  // Serial.println(x[val]);
  // (val>=127)?val=0:val++;
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
  
    
  for(i=0; i<128; i++){
  if(x == 1)
  digitalWrite(8, HIGH);
  else
  digitalWrite(8, LOW);
  delayMicroseconds(500);
  
  if(x==1){
    digitalWrite(8, LOW);
    x = 0;
  }
  else{
    digitalWrite(8, HIGH);
    x = 1;
  }
  
  delayMicroseconds(500);
}//i

  
  digitalWrite(8, LOW);
  // Serial.println("SENT");
  digitalWrite(LED_tx,HIGH);
  delay(100);
  digitalWrite(LED_tx,LOW);
  delay(200);

}

float calculate_temp(){
  float value = analogRead(A0);
  float tempC = 0.4071*value-64.306;
  return tempC;
}

void SendTemp(){
  float C = calculate_temp();
  float finNum = C*100;
  int Num = finNum;
  digitalWrite(LED_tx,HIGH);
  Serial.println(Num);
  sendByte(Num);
  // int firstNum = C;
  // float decNum = abs(firstNum - C)*100;
  // int finDec = decNum;
  // Serial.println(C);
  // sendByte(firstNum);
  // // Serial.println(firstNum);
  // sendByte(finDec);
  // // Serial.println(finDec);
  // sendByte('}');
  digitalWrite(LED_tx,LOW);
  delay(500);
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
  // Serial.println("Turn ON");
  power_twi_disable();
  power_timer1_disable();
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
  if(counter == 0)
  BER_Send();
  else SendTemp();
}