#include <Arduino.h>
#include <avr/power.h>


#define LED_tx 6
#define pin_mode 3
#define pin_stop 4
// #define TEMP_VAR 1100.0
// #define ADC_RESOLUTION 1024.0

int val = 0;
int x = 0;
int counter = 0;
int ledState = LOW;
unsigned long previousMillis = 0;

void sendByte(uint16_t input){
  int i;
  //Start Bit
  for(i=0; i<20; i++){
    digitalWrite(8, HIGH);
    delayMicroseconds(500);
    digitalWrite(8, LOW);
    delayMicroseconds(500);
  }
  //Confirm Bit
  digitalWrite(8, HIGH);
  delayMicroseconds(3000);
  digitalWrite(8, LOW);
  delayMicroseconds(500);
  
  //Message Bit 
  for(i=0; i<9; i++){
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
   int i;
  //Start Bit
  for(i=0; i<20; i++){
  digitalWrite(8, HIGH);
  delayMicroseconds(500);
  digitalWrite(8, LOW);
  delayMicroseconds(500);
}
  //Confirm Bit
  digitalWrite(8, HIGH);
  delayMicroseconds(3000);
  digitalWrite(8, LOW);
  delayMicroseconds(500);
  
  //Message Bit
  for(i=0; i<1024; i++){
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
  float tempC = 0.4071*value-81.814;
  return tempC;
}

void SendTemp(){
  float C = calculate_temp();
  float finNum = C*10;
  int Num = finNum;
  // if(Num > 512){
  //   Num = 511;
  // }
  digitalWrite(LED_tx,HIGH);
  // Serial.println(Num);
  sendByte(Num);
  digitalWrite(LED_tx,LOW);
  delay(200);
  // int firstNum = C;
  // float decNum = abs(firstNum - C)*100;
  // int finDec = decNum;
  // Serial.println(C);
  // sendByte(firstNum);
  // // Serial.println(firstNum);
  // sendByte(finDec);
  // // Serial.println(finDec);
  // sendByte('}');
  
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