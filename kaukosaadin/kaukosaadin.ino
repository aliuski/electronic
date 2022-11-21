#include <IRremote.h>

const int IR_RECEIVE_PIN = 2;

const int enableBridge1 = 4;
const int MotorForward1 = 6;
const int MotorReverse1 = 5;
const int Power = 80;

const long ADDBUTTON = 16589023;
const long SUBBUTTON = 16593103;

/*
Remote controll
Infrared Remote and IR Receiver
https://www.instructables.com/Infrared-Remote-and-IR-Receiver-TSOP1738-With-Ardu/
Control a DC motor with Arduino
https://www.aranacorp.com/en/control-a-dc-motor-with-arduino/
*/

IRrecv IrReceiver(IR_RECEIVE_PIN);

decode_results result;

void setup() {
//  Serial.begin(9600);
  IrReceiver.enableIRIn();
  
  pinMode(MotorForward1,OUTPUT);
  pinMode(MotorReverse1,OUTPUT);
  pinMode(enableBridge1,OUTPUT);
}

void loop() {
    if (IrReceiver.decode(&result)) {
//        Serial.println(result.value);

        if(result.value == SUBBUTTON) {
//          Serial.println("-");
          digitalWrite(enableBridge1,HIGH);
          analogWrite(MotorForward1,0);
          analogWrite(MotorReverse1,Power);
          delay(500);
        } else if(result.value == ADDBUTTON) {
//          Serial.println("+");
          digitalWrite(enableBridge1,HIGH);
          analogWrite(MotorForward1,Power);
          analogWrite(MotorReverse1,0);
          delay(500);
        }
        digitalWrite(enableBridge1,LOW);
        IrReceiver.resume();
    }
    delay(100);
}
