#define BLINK_PIN 7
#define DELAY 2 // seconds

void setup() {                
  pinMode(BLINK_PIN, OUTPUT);     
}

void loop() {
  digitalWrite(BLINK_PIN, HIGH);
  delay(DELAY*1000);
  digitalWrite(BLINK_PIN, LOW);
  delay(DELAY*1000);
}
