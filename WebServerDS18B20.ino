/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */


#include <SPI.h>
#include <Ethernet.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <RTClib.h>
#include <Wire.h>


RTC_DS1307 RTC;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 10, 150);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);



// Il terminale data del sensore è connesso
// alla porta 8 di Arduino
#define ONE_WIRE_BUS 8

// Imposta la comunicazione oneWire per comunicare
// con un dispositivo compatibile
OneWire oneWire(ONE_WIRE_BUS);

// Passaggio oneWire reference alla Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// temperatura letta da sonda digitale DS18B20
double tempDi = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  //start RTC
  Wire.begin();
  RTC.begin();
 
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  RTC.adjust(DateTime(__DATE__, __TIME__));
  //
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
   
     
  
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 2");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<title>Arduino Thermometer - by pierfani.it</title>");
         /*
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          */
                    
          //ORARIO
          DateTime now = RTC.now();          
                    
          //gestione temperatura da DS18B20
          sensors.requestTemperatures(); // Invia il comando di lettura delle temperatura
          tempDi = sensors.getTempCByIndex(0);
          client.print("Temperatura di: ");
          client.print(tempDi);
          client.print(" C");
          client.println("<br />");
          client.println("<br />");
          client.print(now.day(), DEC);
          client.print('/');
          client.print(now.month(), DEC);
          client.print('/');
          client.print(now.year(), DEC);
          client.print(' ');
          client.print(now.hour(), DEC);
          client.print(':');
          client.print(now.minute(), DEC);
          client.print(':');
          client.print(now.second(), DEC);
          client.println("<br />");
          client.println("Arduino Uno - Ethernet Shield - Sensore temperatura DS18B20");
          client.println("<br />");
          client.println("Realizzato da Pierantonio Fanigliulo");
          client.println("<br />");
          client.println("<a href=\"https://github.com/pierfani/Arduino-Web-Server-Temperature\"> Sketch on GitHub</a>");
        
         client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

