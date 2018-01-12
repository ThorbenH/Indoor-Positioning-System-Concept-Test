#include <nRF24L01.h>
#include <RF24.h>

#define echoPin 3
#define trigPin 4

volatile unsigned long startTime = 0;

RF24 radio (7, 8);
const uint64_t adress = 0xF0F0F0F0F0;


void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);//tmp => cus of power reasons, should be changed to high later to strengthen signal
  radio.setDataRate(RF24_1MBPS);//test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);//cus we want >1 devices to listen to this message... think about it a bit, it will make sence
  radio.disableCRC();//maybe this line is not needed, but i dunno


  radio.maskIRQ(1, 1, 0);//only interrupt in reviecing data
  attachInterrupt(0, receivedMessage, FALLING);//0 for pin 2 and 1 for pin 3 => these are the only hardware interrupt pins => there are ways to use other pins, but they only work with CHANGE and are a little more difficult
  radioListenMode();

  
  Serial.print("Reciever: ");
  Serial.print("Radio is ");
  Serial.print(radio.isPVariant());
  Serial.println(".");

  pinMode(trigPin, OUTPUT);
  attachInterrupt(1, mesure, CHANGE);
  digitalWrite(trigPin, LOW);
}

void mesure(){
  if(digitalRead(echoPin) == HIGH){
    startTime = micros();
  } else{
    long duration =micros()-startTime; 
    int distance= duration*0.034;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm.");
  }
}

void receivedMessage(){
  if(radio.available()){
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println("Signal");
    prepUS();
  } else{
    Serial.println("ERROR: Interrupt triggered, even thought there is no message out there.");
  }
}

void prepUS(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

void radioListenMode(){
  radio.openReadingPipe(0, adress);
  radio.startListening();
}

void radioWriteMode(){
  radio.stopListening();
  radio.openWritingPipe(adress);
}

void loop() {
  Serial.println("Looped.");
  delay(1000);
}
