//
//  Example Audrino Tunnel K Code
//  Author:  Chris Davis
//  Author Email:  chrisandsusan@mail.com
//

//Include Libraries
#include <SPI.h>           //Provides communication from the audrino to the ethernet shield
#include <Ethernet.h>      //Provides the functions to communicate from the ethernet shield to the Internet
#include <EthernetDNS.h>   //Provides DNS lookup to the audrino
#include <EthernetDHCP.h>  //Provides functions to activate DHCP functionality on the audrino
#include <DHT22.h>         //Provides the functions needed to read the temperature / humidity sensor
#include <Twitter.h>       //Provides the functions to send tweets from the audrino

//Global Defines
#define DHT22_PIN      7   //Define the DHT22 pin as DIO pin 7

//Global Variables
DHT22 myDHT22(DHT22_PIN);               //Create a DHT22 object and instantiate it for use in the program.
Twitter twitter("369294957-XNvuc1tXUpJwUvH3pUAPeC3c4wzAKyUR6meTHysp"); //the Tunnel K OAuth string for the web app that will tweet for us.
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
const byte* myIP;                       //The IP address that I am assigning to  the audrino on my home network
const byte* myGateway;                  //The DHCP gateway address for the audrino
const byte* dnsAddress;                 //The DNS lookup address for the audrino
byte pachubeServer[] = {173,203,98,29}; //IP address of pachube's data server
Client client(pachubeServer, 80);       //Create a http client that will communicate with pachube's data server
char tweet[140];                        //a static character string is needed to pass the function that will tweet for us.
unsigned long counter;                  //a counter variable to count from 0 up to 2^32 so we know which sequence of the loop to run
boolean lastConnected = false;          //a flag to let us know if we were connected the last time the loop was run.


void setup()
{
  counter = 0;              //Initalize the counter variable to 0.
  Serial.begin(9600);       //Initalize the serial port with standard settings and 9600 Baud transmission rate

  EthernetDHCP.begin(mac);  //Initalize the Ethernet Shield with the mac address and a request to obtain a DHCP IP address
 
  //if we make it this far, we have a DHCP provided ip address
  myIP = EthernetDHCP.ipAddress();              //Get our DHCP IP Address
  myGateway = EthernetDHCP.gatewayIpAddress();  //Get our DHCP Gateway Address
  dnsAddress = EthernetDHCP.dnsIpAddress();     //Get our DNS lookup Address
}

void loop()
{
  //Local Loop Variables
  DHT22_ERROR_t  DHT22ErrorCode;                //DHT22 Error Code
  double         DHT22Humidity;                 //Humidity reading
  double         DHT22Temperature;              //Temperature reading
  char           SerialPrintString[250];        //Temporary string to facilitate printing messages to the Audrino serial port.
  String         DataString;                    //Temporary string to facilitate publishing messages to pachube.com
  char           tempString[15];                //Temporary string used to hold the conversion of temperature from a floating point number to a string
  char           humidityString[15];            //Temporary string used to hold the conversion of humidity from a floating point number to a string
  
  delay(1000);  //run this loop code 1/second
  
  counter++;    //advance the counter by one each time
  
  //If the web client is connected disconnect it
  if (client.connected())
  {
    client.stop();
  }
  
  //Every three seconds, run this code
  if((counter % 3) == 0)
  {
    //Read the DHT22 sensor every three seconds
    DHT22ErrorCode = myDHT22.readData();
    DHT22Humidity = myDHT22.getHumidity();
    DHT22Temperature = myDHT22.getTemperatureC();
    sprintf(SerialPrintString, "DHT22 Temp = %s deg C, DHT22 Humidity = %s %% Relative Humidity", ftoa(tempString, DHT22Temperature, 2), ftoa(humidityString, DHT22Humidity, 2));
    Serial.println(SerialPrintString);
  }
  
  //Every 20 seconds, run this code.
  if((counter % 20) == 0)
  {
    //Publish data to pachube.com every 20 seconds.
    Serial.println("Publishing data to pachube.com");
    DataString = "";  //clear the DataString from last time
    DataString = DataString + ftoa(tempString, DHT22Temperature, 2) + "," + ftoa(humidityString, DHT22Humidity, 2);
    Serial.println(DataString);
    sendData(DataString);
  }
  
  //Once per hour, run this code.
  if((counter % 3600) == 0)
  {
    //Tweet the current Temperature and Humidity every hour
    Serial.println("Tweeting current temperature and humidity information");
    sprintf(tweet, "Temperature = %s deg C, Humidity = %s %% Relative Humidity", ftoa(tempString, DHT22Temperature, 2), ftoa(humidityString, DHT22Humidity, 2));
    //open up a web client session
    client.connect();
    //post our tweet message
    if(twitter.post(tweet))
    {
      //if the tweet is successful, print a message to the audrino serial port
      int status = twitter.wait(&Serial);
      if(status == 200)
      {
        Serial.println("Tweeted OK.");
      }
      //if the tweet is unsuccessful, post the error code.
      else
      {
        Serial.println("failed : code ");
        Serial.println(status);
      }
    }
  }
}

// this method makes a HTTP connection to the server:
void sendData(String thisData) 
{
  // if there's a successful connection:
  if (client.connect()) 
  {
    //Serial.println("connecting...");
    // send the HTTP PUT request. 
    // fill in your feed address here:
    client.print("PUT /api/36705.csv HTTP/1.1\n");
    client.print("Host: www.pachube.com\n");
    // fill in your Pachube API key here:
    client.print("X-PachubeApiKey: USLFAeQavXODvrKyz3mV-cJV36N098Wi_tM_929vtak\n");
    client.print("Content-Length: ");

    // calculate the length of the sensor reading in bytes:
    //int thisLength = getLength(thisData);
    client.println(thisData.length(), DEC);

    // last pieces of the HTTP PUT request:
    client.print("Content-Type: text/csv\n");
    client.println("Connection: close\n");

    // here's the actual content of the PUT request:
    client.println(thisData);

    // note the time that the connection was made:
    //lastConnectionTime = millis();
  } 
  else 
  {
    // if you couldn't make a connection:
    Serial.println("Pachube connection failed");
  }
}

char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

