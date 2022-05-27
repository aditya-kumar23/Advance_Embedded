int lightPin=A0;
int lightVal;
int del=250;
int redPin=6;

void setup() {
  // put your setup code here, to run once:
pinMode(lightPin,INPUT);
pinMode(redPin,OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
lightVal=analogRead(lightPin);
Serial.println(lightVal);
delay(del);
if(lightVal<350)
{
  digitalWrite(redPin,HIGH);
}
  
else  digitalWrite(redPin,LOW);
}
