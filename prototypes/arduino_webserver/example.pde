/* "ArduServer2" by TK Boyd
version? See "ver" constant
Help? See http://Arduserver.com */

/* This code is HEAVILY based on the example "WebServer"
supplied with (at least) the Arduino environment 0022,
August 2011.

From that... BEGIN (edited) QUOTE...

A simple web server that shows the value of the analog input pins.
 using an Arduino W5100 Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 ADDED TO DEMO by TKB:

 N.B.: May need changing from instance to instance...

 IP addresses... LAN and internet URL (latter appears several places)
 Port
 Pin assignments

 ... added to demo continued...
 A light sensor on A0 looking at room's ambient light... not
      central to function of demo, but needed, if text on served web
      page to be valid
 A light sensor on A1, which an LED shines on when it is shining.
      Probably best to wrap them together with opaque tape, sort
      of a poor man's "opto-isolator"
 An LED + resistor on pin designated by value in bLEDpin
 A beeper on pin designated by bBeeperPin... not essential to demo
(Bring pin high to turn either on... "beeper" makes continual
    whine while on.)
(End "ADDED"... quote continues...)

 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe

... END QUOTE... and please remember, Mellis & Igoe's
code has been MESSED WITH.
Things that work: All credit to them.
Things that don't: Probably because TKB "broke" them!

Aspects of this are still a DOG'S DINNER... but it works!....
*/

#include <SPI.h>
#include <Ethernet.h>

const char ver[]="21 Jan 12d";//Plus Wiznet to W5100
/*THE LETTER AT THE END OF THE DATE shows not,
which would be usual in my work, a later version
of something done earlier on the same day.

Instead, there are four versions of ArduServer2...
"a", "b", "c" and "d", identified by the last letter
of the version ID.

Version "d" is best and (at this point) latest version
of ArduServer2

Changes from ver c to ver d... N.B....

MOVED: The wire turing the LED on and off was moved from
   pin 8 to pin 6
ADDED: DS1820 tture sensor on pin bTturePin (5 at time)
   and associated code.

KEEP THE VERSION ID TO 10 characters:
  The page served is nearly "too full", and
  the "ver" string is inserted into the HTML.  */


const byte bLEDpin=6;//Was on 8 in version c
const byte bBeeperPin=7;
const byte bTturePin=5;
const byte MaxArrayElement=252;
/*Defines the size of the array for holding the first
  characters in each line coming in with the http requests
  FOR SOME REASON, setting it to about 25 caused bad problems...
  I can't see why this happens. To be investigated!*/

/**********
YOU MAY HAVE TO CHANGE THE NUMBERS HERE.......

Set up a port number, MAC address and IP address for your
controller below.

Port 80 is usual for HTTP. (I needed 81 for
    special circumstances: two servers on one IP address)
The MAC address will need changing if you have two devices on
    your LAN with the same MAC address.
The IP address may need changing, depending on your LAN, and
    what is on it already. There must not be two devices with
    the same IP address on the same LAN.

While you would not want to change what is in mac[] or ip[]
    while the program is running, you cannot make them constants,
    or the call of Ethernet.begin(mac, ip); will not compile. */

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192,168,0,240};//240 7NC... shared with old, nuElec, 254 SX
const byte bPort = 81;

/* Initialize the Ethernet server library. At the
  same time, tell it the port you want to use.*/
Server server(bPort);

char c,cMstRecentCmnd='/';
/*That '/' is part of one of the crude aspects of the current code.
  If someone accesses the page without giving a command, the "command"
  the software sees is '/'... so '/' is being used as an unreliable
  rogue saying "no command given". However, if an invalid command is
  given, anything can be harvested as "the command". AND, if the user
  used...
     (URL)/?cmd=/
  ... then a "valid" '/' would arrive as "the command, but be ignored,
  as it would be though to be representing "no valic command".

  As I said... the server works, but it isn't very robust. It only
  accepts two commands, and they have to be presented properly.

  They are, and are presented, thus...

       /?cmd=0
       /?cmd=1
*/

char cLineBuffer[MaxArrayElement];
byte bPlaceInBuffer;
word wClicksProcessed=0;

void setup()
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  //Serial.begin(9600);//Restore, if you want to monitor program's
       //operation via serial monitor

  pinMode(bBeeperPin,OUTPUT);
  digitalWrite(bBeeperPin,LOW);

  pinMode(bLEDpin,OUTPUT);
  LEDOff();

//New in version d... for tture sensing... (2 lines
    digitalWrite(bTturePin, LOW);
    pinMode(bTturePin, INPUT);
}

void loop()
{ int iAn0;
  Client client = server.available();//1.... these rems, with numbers, remnants from
       //development of the program, retained in case they help describe the
       //final version. Which lines have them is a quirk of fate.
  if (client) {//2
    boolean currentLineIsBlank = true;//3
    bPlaceInBuffer=0;
    char cCmnd='H';//Rogue for "no command"
       //In current CRUDE scheme, after URL, either nothing
       //... in which case an 'H' is seen (H of HTTP is picked up)
       //or a question mark (to be traditional) followed by
       //   /?cmd=
       //followed by one case sensitive, program hardcoded character.
       //See subroutine DoCmnd for what the commands are, what they do.

    cMstRecentCmnd='H';//Init with rogue

    while (client.connected()) {//4
      if (client.available()) {//5
         c = client.read();//6
         if ((c!='\n') && (c!='\r')){
           cLineBuffer[bPlaceInBuffer]=c;
           if (bPlaceInBuffer<MaxArrayElement) {bPlaceInBuffer++;};
           }
         //Serial.print(c);
         if (c == '\n' && currentLineIsBlank) {//7
            delay(250);//Give light sensor a moment to register changed light level...
               //LDR response sluggish.. but even phototransistor needs a little time
               //During "DoCmnd" an LED may have been turned on or off/
               //Can be much shorter with better sensor.

            wClicksProcessed++;//A frill
            ShortBeep();//A frill

            //Current code (ver 6 Sept 11c: 2 sensors, 2 buttons, RevolverMap)
            //VERY CLOSE to too much code.... add more, and watch closely for
            //bad and poorly reported misbehaviour.

            client.println("HTTP/1.1 200 OK");//8
            client.println("Content-Type: text/html");//9
            client.println();//10

            /*The HTML created by this program is far from
              "well crafted". But it does the job... */

            client.println("<html>");
            client.println("<head>");
            client.print("<title>ArduServer2</title>");
            client.println("</head>");
            client.println("</head>");
            client.println();
            client.println("<body>");

            client.println("<center>");
            client.print("<h2>ArduServer2, ver ");
            client.print(ver);
            client.println("</h2>");

            client.print("<h3>See <a href=\"http://ArduServer.com\"<"
                "target=\"_blank\">ArduServer.com</a></h3>");
            client.println("</center>");
            client.println();

            if ((cMstRecentCmnd=='0')||(cMstRecentCmnd=='1')){
                client.print("<p>Cmd received: <b>");
                client.print(cMstRecentCmnd);
                client.println("</b></p>");
                }//no ;
              else {
                client.print("<p>Cmnds: \?cmd=0 and \?cmd=1<br>");
                client.print("Add \"/?cmd=1\" to page URL for different "
                  "text here.</p>");
                };

            client.println("<p>- -");

            client.print("<br>Temperature, degrees C: <b>");

//Here begins small block about reading tture, and sending it to output
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

  OneWireReset(bTturePin);
  OneWireOutByte(bTturePin, 0xcc);
  OneWireOutByte(bTturePin, 0x44); // perform temperature conversion, strong pullup for one sec

  OneWireReset(bTturePin);
  OneWireOutByte(bTturePin, 0xcc);
  OneWireOutByte(bTturePin, 0xbe);

  LowByte = OneWireInByte(bTturePin);
  HighByte = OneWireInByte(bTturePin);
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;


  if (SignBit) // If its negative
  {
     client.print("-");
  }
  client.print(Whole);
 client.print(".");
  if (Fract < 10)
  {
     client.print("0");
  }

  client.print(Fract);

      client.print("\n");
  delay(1200);      // Delay.  For DS1820 at full precision, at least 1000 needed


//(here ends block about reading tture)

            client.println("</b>");

            client.println("<p>- -");

            client.print("<br>Analog input 0 reads: <b>");
            iAn0=analogRead(0);
            client.print(iAn0);
            client.println("</b>");

            client.print("<br>Connected to a "
                "<a href=\"http://www.arunet.co.uk/tkboyd/ec/ec1senslight.htm\" "
                "target=\"_blank\">"
                "phototransistor</a>.</p>");//12

            if (iAn0>320)
              {client.println("<p>Room: <b>well lit</b>.</p>");}
              else
                  {if (iAn0>30)
                  {client.println("<p>Room: <b>dim</b>.</p>");}
                  else
                  {client.println("<p><b>Room is dark.</b></p>");};};

            client.println("<p>- -");//12

            client.print("<br>Analog input 1, senses LED, reads: <b>");//12
            client.print(analogRead(1));//13
            client.println("</b>");

            /* Here's a crude, but more clear, alternative to the buttons... just use links...
              client.println("</p><p>Click here to turn LED on <a href=\"http://mon7nc.dyndns.org:81/?cmd=0\">"
                  "http://mon7nc.dyndns.org:81/?cmd=0</a><br>");
              client.println("Click here to turn LED off <a href=\"http://mon7nc.dyndns.org:81/?cmd=1\">"
                  "http://mon7nc.dyndns.org:81/?cmd=1</a></p>");
             */


             /*The following is just(?) a "prettier" way to achieve what the
               links above achieve. You can probably do more with forms than
               you can do with mere links... but we're not going far with the
               modest form code below....   */

              client.println();
              client.println("<form METHOD=get action=\"http://mon7nc.dyndns.org:81/\">");
              client.println("<input type=hidden name=\"cmd\" value=\"1\">");
              client.println("<input type=submit value=\"LED On\">");
              client.println("</form>");
              client.println();

              client.println("<form METHOD=get action=\"http://mon7nc.dyndns.org:81/\">");
              client.println("<input type=hidden name=\"cmd\" value=\"0\">");
              client.println("<input type=submit value=\"LED Off\">");
              client.println("</form>");
              client.println();

      /*No room in page for the following text at the moment....
              client.println("<p>Light sensor watching LED, a mere LDR, responds "
                 "slowly... click 'on' (or 'off') several times to "
                 "get \"final answer\".</p>");
       */

              client.print("<p>Clicks since server booted: ");
              client.print(wClicksProcessed);
              client.println("</p>");

          client.println("<br>");

          //TKB: New RevolverMap for ArduServer2... mon277..:81...
          //Done 3 Aug 11, 22:25, my third... just after hearing from Kate (on NOLS course) of Xelda's death...
          //First hooked up Uno and "official" Ethernet Shield earlier same evening.

              client.println("RevolverMap sacrificed to make room for temperature sensing.");
  //        client.println("<center>");

          /*PLEASE NOTE: This code specific for my page. You need to go to RevolverMaps and get
               your own free code, if you want a similar thing on your own page. Easy!*/

  /*        client.print("<script type=\"text/javascript\" src=\"http://jf.revolvermaps.com/r.js\">");
          client.print("</script><script type=\"text/javascript\">rm_f1st('2','260','true','false','000000','5sgqledqgna',");
          client.print("'true','00ff6c');</script><noscript><applet codebase=\"http://rf.revolvermaps.com/j ");
          client.print("code=\"core.RE\" width=\"260\" height=\"260\" archive=\"g.jar\"><param name=\"cabbase\" ");
          client.print("value=\"g.cab\" /><param name=\"r\" value=\"true\" /><param name=\"n\" value=\"false\" /");
          client.print("><param name=\"i\" value=\"5sgqledqgna\" /><param name=\"m\" value=\"2\" />");
          client.print("<param name=\"s\" value=\"260\" /><param name=\"c\" value=\"00ff6c\" /><param name=\"v\" ");
          client.print("value=\"true\" /><param name=\"b\" value=\"000000\" /><param name=\"rfc\" value=\"true\" />");
          client.println("</applet></noscript>");

          client.println("</center>");
   */
          client.println("</body>");
          client.println("</html>");

          break;//15
         }
        if (c == '\n') {//17
          currentLineIsBlank = true;//18

          //display with buffer contents;
          if (bPlaceInBuffer>0){
             for (byte c1=0; c1<bPlaceInBuffer; c1++){
             //Serial.print(cLineBuffer[c1]);
             }
         //Serial.println();
         if ((cLineBuffer[0]=='G')&&(cLineBuffer[1]=='E')&&(cLineBuffer[2]=='T'))
         /*VERY CRUDE: Always just picks up 11th char of GET line... either the H of...
           GET / HTTP...  or the character after the tradtional "?cmd=", as in...
           GET /?cmd=1 HTTP...  */

           {
             cCmnd=cLineBuffer[10];//Yes: INDEX 10 for 11th character.
             //Serial.print("Command was: ");
             //Serial.println(cCmnd);
             if (cCmnd!='/') {DoCmnd(cCmnd);};
             /*Don't do if cCmnd=='/'... that suggests there
               was no command, or a "?cmd=/" command, which
               should not be added to the range of valid commands
               if the code is revised.*/
           }
         }
         //end "deal with buffer contents"

         bPlaceInBuffer=0;//set back to 0 for next line

        } //19... things like the "19" are remnants of an earlier line ID
              // system
        else if (c != '\r') {        //20
          currentLineIsBlank = false;//21
        }//22
      }//23
    }//24
    delay(1);//25
    client.stop();//26
  }//27
}//28

void DoCmnd(char cLCmnd)
{
if (cLCmnd=='0'){
    //Serial.println("Cmnd was a zero!");
    cMstRecentCmnd=cLCmnd;
    LEDOff();
    };
if (cLCmnd=='1'){
    //Serial.println("Cmnd was a one!");
    cMstRecentCmnd=cLCmnd;
    LEDOn();
    };
}

void LEDOn(){
digitalWrite(bLEDpin,HIGH);
}

void LEDOff(){
digitalWrite(bLEDpin,LOW);
}

void ShortBeep(){
digitalWrite(bBeeperPin,HIGH);
delay(80);
digitalWrite(bBeeperPin,LOW);
}

//New in ver 1 jan 12d from here to "end new..."
//  This is not only new stuff.

void OneWireReset(int Pin) // reset.  Should improve to act as a presence pulse
{
     digitalWrite(Pin, LOW);
     pinMode(Pin, OUTPUT); // bring low for 500 us
     delayMicroseconds(500);
     pinMode(Pin, INPUT);
     delayMicroseconds(500);
}//end of OneWireReset

void OneWireOutByte(int Pin, byte d) // output byte d (least sig bit first).
{
   byte n;

   for(n=8; n!=0; n--)
   {
      if ((d & 0x01) == 1)  // test least sig bit
      {
         digitalWrite(Pin, LOW);
         pinMode(Pin, OUTPUT);
         delayMicroseconds(5);
         pinMode(Pin, INPUT);
         delayMicroseconds(60);
      }
      else
      {
         digitalWrite(Pin, LOW);
         pinMode(Pin, OUTPUT);
         delayMicroseconds(60);
         pinMode(Pin, INPUT);
      }
      d=d>>1; // now the next bit is in the least sig bit position.
    }
}//end of OneWireOutByte

byte OneWireInByte(int Pin) // read byte, least sig byte first
{
    byte d, n, b;

    for (n=0; n<8; n++)
    {
        digitalWrite(Pin, LOW);
        pinMode(Pin, OUTPUT);
        delayMicroseconds(5);
        pinMode(Pin, INPUT);
        delayMicroseconds(5);
        b = digitalRead(Pin);
        delayMicroseconds(50);
        d = (d >> 1) | (b<<7); // shift d to right and insert b in most sig bit position
    }
    return(d);
}//end OneWireInByte

//end block of new in ver 1 jan 12d stuff

