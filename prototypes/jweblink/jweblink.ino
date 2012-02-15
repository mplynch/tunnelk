/* jweblink.ino - example sketch for Webduino library */

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

static uint8_t mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF7, 0x62 };
static uint8_t ip[] = { 192, 168, 1, 37 };

#define PREFIX "/lightswitch"
WebServer webserver(PREFIX, 80);

#define BLINK_PIN 7

int lightSwitch = 1;

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
      if (strcmp(name, "lightswitch") == 0)
      {
        lightSwitch = strtoul(value, NULL, 10);
      }
    } while (repeat);
    
    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method. 
    server.httpSeeOther(PREFIX);
    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {
    /* store the HTML in program memory using the P macro */
    P(message) = 
      "<html><head><title>Welcome to jweblink</title>"
      "<body>"
      "<h1>Test the Light!</h1>"
      "<form action='/lightswitch' method='POST'>"
      "<p><button name='lightswitch' value='0'>Turn Off</button></p>"
      "<p><button name='lightswitch' value='1'>Turn On</button></p>"
      "</form></body></html>";

    server.printP(message);
  }
}

void setup()
{
  pinMode(BLINK_PIN, OUTPUT);
  digitalWrite(BLINK_PIN, lightSwitch ? HIGH : LOW);
  Ethernet.begin(mac, ip);
  webserver.setDefaultCommand(&blinkCmd);
  webserver.begin();
}

void loop()
{
  webserver.processConnection(); // does this block until a request arrives??
  digitalWrite(BLINK_PIN, lightSwitch ? HIGH : LOW);  
}
