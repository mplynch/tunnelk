int counter; 
int rpmValue;
const int led = 13;
const int hallSensor = 8;

void setup() 
{ 
  pinMode(hallSensor, INPUT); 
  
  Serial.begin(9600);
  
  attachInterrupt(0, rpm, CHANGE); 
}
        
void rpm ()
{
  counter++;
}

void loop () 
{
  counter = 0;
  
  interrupts();
  
  delay (1000);
  
  noInterrupts();
  
  rpmValue = ((counter * 60) / 2);
  
  Serial.print (rpmValue, DEC);
  
  Serial.print (" rpm\r\n");
}
