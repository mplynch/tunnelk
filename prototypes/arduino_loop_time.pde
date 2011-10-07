int ledPin = 13;
int sensorPin = A0;

long loopTimes = 10000;

long counter;
long offset;
long timer;

void setup()
{ 
  counter = offset = 0;
}

void loop()
{
  int value;

  value = analogRead(A0);
  value = analogRead(A1);
  value = analogRead(A2);
  value = analogRead(A3);
  value = analogRead(A4);
  value = analogRead(A5);

  counter++;

  if(counter == loopTimes)
  {
    timer=millis() - offset;

    Serial.begin(19200);
    Serial.print(loopTimes);
    Serial.print(" loops took ");
    Serial.print(timer / 1000.0);
    Serial.print(" seconds, or ");
    Serial.print((double)timer / (double)loopTimes);
    Serial.println(" ms per loop.");

    counter = 0;

    offset = millis();
  }
}



