int hallEffectCounter, tachCounter;
int rpmValue;
const int led = 13;
const int hallsensor = 8;

void setup() 
{ 
  pinMode(hallsensor, INPUT); 
  
  Serial.begin(9600);
  
  attachInterrupt(0, hallEffectRPM, CHANGE);

  attachInterrupt(1, tachRPM, CHANGE);
}
        
void hallEffectRPM()
{
  hallEffectCounter++;
}

void tachRPM()
{
  tachCounter++;
}

void loop () 
{
  hallEffectCounter = 0;
  
  tachCounter = 0;
  
  interrupts();
  
  delay (1000);
  
  noInterrupts();
  
  rpmValue = ((hallEffectCounter * 60) / 2);

  Serial.print ("Hall effect sensor RPM: ");
  
  Serial.print (rpmValue, DEC);
  
  Serial.print(", ");
  
  rpmValue = ((tachCounter * 60) / 4);

  Serial.print ("Fan tach RPM: ");
  
  Serial.print (rpmValue, DEC);
  
  Serial.println();
}
