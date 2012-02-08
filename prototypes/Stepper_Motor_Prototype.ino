int StepOutputPin = 10;
int DirectionPin = 7;
int counter = 0;
void setup() 
{
  // put your setup code here, to run once:
  pinMode(StepOutputPin, OUTPUT);
  pinMode(DirectionPin, OUTPUT);  
}

void loop() 
{
  // put your main code here, to run repeatedly: 
  while(1)
  {
    analogWrite(StepOutputPin, 127);
    digitalWrite(DirectionPin, LOW);
    delay(2000);
    digitalWrite(DirectionPin, HIGH);
    delay(2000);
  }
}
