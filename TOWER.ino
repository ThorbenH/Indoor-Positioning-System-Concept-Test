#include <nRF24L01.h>
#include <RF24.h>

#define echoPin 3
#define trigPin 4

RF24 radio (7, 8);
const uint64_t adress = 0xF0F0F0F0F0;


void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);//tmp => cus of power reasons, should be changed to high later to strengthen signal
  radio.setDataRate(RF24_1MBPS);//test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);//cus we want >1 devices to listen to this message... think about it a bit, it will make sence
  radio.disableCRC();//maybe this line is not needed, but i dunno

  radioWriteMode();

  Serial.print("Transmitter: ");
  Serial.print("Radio is ");
  Serial.print(radio.isPVariant());
  Serial.println(".");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
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
  const char message[] = "0";
  radio.write(&message, sizeof(message));
  
  prepUS();
  
  Serial.println("Send out.");
  delay(1000);
}
