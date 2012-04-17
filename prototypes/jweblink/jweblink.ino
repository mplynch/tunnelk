/* jweblink.ino - example sketch for Webduino library */

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"
//#include "aJSON.h"
#include "Streaming.h"

static uint8_t mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF7, 0x62 };
static uint8_t ip[] = { 192, 168, 2, 50 };

int StepOutputPin = 3;
int DirectionPin = 7;
static float temperature_0 = 0;
static int analog_input_0_counts = 0;
static int temperature0_LED_pin = 4;
static int temperature0_LED_state = LOW;

static float temperature_1 = 0;
static int analog_input_1_counts = 0;
static int temperature1_LED_pin = 5;
static int temperature1_LED_state = LOW;

static float humidity_0 = 0;
static int humidity0_LED_pin = 6;
static int humidity0_LED_state = LOW;

static float temperature_2 = 0;

static int rpm_0 = 0;
static int rpm0_LED_pin = 7;
static int rpm0_LED_state = LOW;

#define DATAPREFIX "/alldata"
WebServer webserver(DATAPREFIX, 80);

//#define BLINK_PIN 7
#define TEMP_PIN 0
int lightSwitch = 1;
//static aJsonObject *temp1;
//String temp_string;


//template<class T>
//inline Print &operator <<(Print &obj, T arg)
//{ obj.print(arg); return obj; }

/* This command is set as the default command for the server.  It
 * handles both GET and POST requests.  For a GET, it returns a simple
 * page with some buttons.  For a POST, it saves the value posted to
 * the lightSwitch variable, affecting the output of the light. */
void blinkCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (type == WebServer::POST)
  {
    bool repeat;
    char name[16], value[16];
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);
      Serial.println(name);
      if (strcmp(name, "lightswitch") == 0)
      {
        Serial.println("Light switch activated");
        lightSwitch = strtoul(value, NULL, 10);
        //Serial.println("Here");
        analogWrite(StepOutputPin, 127);
        if(lightSwitch == 1)
        {
          digitalWrite(DirectionPin, HIGH);
        }
        else
        {
          digitalWrite(DirectionPin, LOW);
        }
        delay(5000);
        analogWrite(StepOutputPin, 0);
      }
      else if(strcmp(name, "Temp1LED") == 0)
      {
        temperature0_LED_state = strtoul(value, NULL, 10);
        Serial.println("Temperature 0 LED State = ");
        Serial.println(temperature0_LED_state);
        digitalWrite( temperature0_LED_pin, temperature0_LED_state );      
      }
      else if(strcmp(name, "Temp2LED") == 0)
      {
        temperature1_LED_state = strtoul(value, NULL, 10);
        Serial.println("Temperature 1 LED State = ");
        Serial.println(temperature1_LED_state);
        digitalWrite( temperature1_LED_pin, temperature1_LED_state );      
      }
      else if(strcmp(name, "Humidity1LED") == 0)
      {
        humidity0_LED_state = strtoul(value, NULL, 10);
        Serial.println("Humidity 0 LED State = ");
        Serial.println(humidity0_LED_state);
        digitalWrite( humidity0_LED_pin, humidity0_LED_state );      
      }
      else if(strcmp(name, "Temp3LED") == 0)
      {
        //temperature 2 and humidity 0 are same sensor.
        humidity0_LED_state = strtoul(value, NULL, 10);
        Serial.println("Temperature 2 LED State = ");
        Serial.println(humidity0_LED_state);
        digitalWrite( humidity0_LED_pin, humidity0_LED_state );      
      }
      else if(strcmp(name, "RPM1LED") == 0)
      {
        rpm0_LED_state = strtoul(value, NULL, 10);
        Serial.println("RPM 0 LED State = ");
        Serial.println(rpm0_LED_state);
        digitalWrite( rpm0_LED_pin, rpm0_LED_state );      
      }
      
    } while (repeat);
    
    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method. 
    server.httpSeeOther(DATAPREFIX);
    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {
      Serial.println("Get command recieved.");
    
      analog_input_0_counts = analogRead(0);
      temperature_0 = analog_input_0_counts * .004882814;
      temperature_0 = (temperature_0 - .5) * 100;
      temperature_0 = (temperature_0 * 1.8) + 32;
     
      analog_input_1_counts = analogRead(1);
      temperature_1 = analog_input_1_counts * .004882814;
      temperature_1 = (temperature_1 - .5) * 100;
      temperature_1 = (temperature_1 * 1.8) + 32;
      
      P(message) = 
                   "<html><head><title>Welcome to jweblink</title>"
                   "<body>"
                   "<h1>Test the Light!</h1>"
                   "<h2>Temp 1 LED</h2>"
                   "<form action='/alldata' method='POST'>"
                   "<p><button name='Temp1LED' value='0'>Turn Off</button></p>"
                   "<p><button name='Temp1LED' value='1'>Turn On</button></p>"
                   "</form>"
                   "<h2>Temp 2 LED</h2>"
                   "<form action='/alldata' method='POST'>"
                   "<p><button name='Temp2LED' value='0'>Turn Off</button></p>"
                   "<p><button name='Temp2LED' value='1'>Turn On</button></p>"
                   "</form>"
                   "<h2>Temp 3 LED</h2>"
                   "<form action='/alldata' method='POST'>"
                   "<p><button name='Temp3LED' value='0'>Turn Off</button></p>"
                   "<p><button name='Temp3LED' value='1'>Turn On</button></p>"
                   "</form>"
                   "<h2>Humidity 1 LED</h2>"
                   "<form action='/alldata' method='POST'>"
                   "<p><button name='Humidity1LED' value='0'>Turn Off</button></p>"
                   "<p><button name='Humidity1LED' value='1'>Turn On</button></p>"
                   "</form>"
                   "<h2>RPM 1 LED</h2>"
                   "<form action='/alldata' method='POST'>"
                   "<p><button name='RPM1LED' value='0'>Turn Off</button></p>"
                   "<p><button name='RPM1LED' value='1'>Turn On</button></p>"
                   "</form>"
                   "<br><br>"
                   "<pre>";
      server.printP(message);
      P(message1) = "["
                    "{ \"initialValue\":0,\"readOnly\":true,\"name\":\"Temp1\",\"type\":\"Temperature\",\"units\":\"degF\",\"value\":";
      server.printP(message1);
      server << temperature_0;
      
      P(message2) = "},{ \"initialValue\":0,\"readOnly\":true,\"name\":\"Temp2\",\"type\":\"Temperature\",\"units\":\"degF\",\"value\":";
      server.printP(message2);
      server << temperature_1;
      
      P(message3) = "},{ \"initialValue\":0,\"readOnly\":true,\"name\":\"Temp3\",\"type\":\"Temperature\",\"units\":\"degF\",\"value\":";
      server.printP(message3);
      server << temperature_2;
      
      P(message4) = "},{ \"initialValue\":0,\"readOnly\":false,\"name\":\"Humidity1\",\"type\":\"Humidity\",\"units\":\"relative humidity\",\"value\":";
      server.printP(message4);
      server << humidity_0;
      
      P(message5) = "},{ \"initialValue\":0,\"readOnly\":false,\"name\":\"Temp1LED\",\"type\":\"LED\",\"units\":\"none\",\"value\":";
      server.printP(message5);
      server << temperature0_LED_state;
      
      P(message6) = "},{ \"initialValue\":0,\"readOnly\":false,\"name\":\"Temp2LED\",\"type\":\"LED\",\"units\":\"none\",\"value\":";
      server.printP(message6);
      server << temperature1_LED_state;
      
      P(message7) = "};{ \"initialValue\":0,\"readOnly\":false,\"name\":\"Temp3LED\",\"type\":\"LED\",\"units\":\"none\",\"value\":";
      server.printP(message7);                   
      server << humidity0_LED_state;
      
      P(message8) = "},{ \"initialValue\":0,\"readOnly\":false,\"name\":\"Humidity1LED\",\"type\":\"LED\",\"units\":\"none\",\"value\":";
      server.printP(message8);
      server << humidity0_LED_state;
      
      P(message9) = "},{ \"initialValue\":0,\"readOnly\":false,\"name\":\"RPM1LED\",\"type\":\"LED\",\"units\":\"none\",\"value\":";
      server.printP(message9);
      server << rpm0_LED_state;
      server << "}";
      
      P(message10) = "]";
      server.printP(message10);
      
      P(message11) = "</pre></body></html>";
      server.printP(message11);

      
        //server << "Temperature 0 = " << temperature_0;
        //server << ",Temperature 1 = " << temperature_1;
        //server << ",Temperature 2 = " << temperature_2;
        //server << ",Humidity 0 = " << humidity_0;
        //server << ",Fan RPM 0 = " << fan_rpm_0;
      
  }
}

void setup()
{
  pinMode(StepOutputPin, OUTPUT);
  pinMode(DirectionPin, OUTPUT);
  pinMode(temperature0_LED_pin, OUTPUT);
  pinMode(temperature1_LED_pin, OUTPUT);
  pinMode(humidity0_LED_pin, OUTPUT);
  pinMode(rpm0_LED_pin, OUTPUT);
  //pinMode(temperature0_LED_pin, OUTPUT);
  Ethernet.begin(mac, ip);
  webserver.setDefaultCommand(&blinkCmd);
  webserver.begin();
  Serial.begin(9600);
 // temp1 = aJson.createObject();
 // aJson.addNumberToObject(temp1, "initial value", 144);
 // aJson.addTrueToObject(temp1,"readOnly");
 // aJson.addStringToObject(temp1,"name","Inlet Temperature");
 // aJson.addStringToObject(temp1,"type","Temperature");
 // aJson.addStringToObject(temp1,"units","degF");
 // aJson.addNumberToObject(temp1,"value", 0);
}

void loop()
{
  webserver.processConnection(); // does this block until a request arrives??
}
