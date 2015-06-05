/*
 Structure of sketch based on One Way Chat by Hari Wiguna
 https://www.youtube.com/watch?v=iRMdj3dS2hc
 
 font.h and OLED are copied from the work of Mike Rankin
 https://github.com/mike-rankin/ESP8266_Weather_Info/tree/master/
 
 My work was to combine the parts and solve all the little bugs
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "font.h"
#define OLED_address  0x3c 
#define SSID  "FRITZ"
#define PASS  "33149193jc"
// ******************* String form to sent to the client-browser ************************************
String form =
  "<p>"
  "<center>"
  "<h1>Talk to me :-)</h1>"
  "<img src='http://i.imgur.com/qu8lDEu.jpg'>"
  "<form action='msg'><p>Wassup? <input type='text' name='msg' size=50 autofocus> <input type='submit' value='Submit'></form>"
  "</center>";

ESP8266WebServer server(80);                             // HTTP server will listen at port 80


/*
  handles the messages coming from the webbrowser, restores a few special characters and 
  constructs the strings that can be sent to the oled display
*/
void handle_msg() {
  clear_display();                        // clears oled
 
  server.send(200, "text/html", form);    // Send same page so they can send another msg

  // Display msg on Oled
  String msg = server.arg("msg");
  //Serial.println(msg);
  String decodedMsg = msg;
  // Restore special characters that are misformed to %char by the client browser
  decodedMsg.replace("+", " ");      
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%3C", "<");  
  decodedMsg.replace("%3D", "=");  
  decodedMsg.replace("%3E", ">");  
  //Serial.println(decodedMsg);                   // print original string to monitor
  unsigned int lengte = decodedMsg.length();      // length of received message
  for  (int i=0;i<lengte;i++)                     // prints up to 8 rows of 16 characters.
    {
      char c = decodedMsg[i];
      Serial.print(c); //decodedMsg[i]);
           if (i<16)  sendCharXY(c,0,i);
      else if (i<32)  sendCharXY(c,1,i-16);
      else if (i<48)  sendCharXY(c,2,i-32);
      else if (i<64)  sendCharXY(c,3,i-48);
      else if (i<80)  sendCharXY(c,4,i-64);
      else if (i<96)  sendCharXY(c,5,i-80);
      else if (i<112) sendCharXY(c,6,i-96);
      else if (i<128) sendCharXY(c,7,i-112);

    }
  //Serial.println(' ');                          // new line in monitor
}

void setup(void) {
//ESP.wdtDisable();                               // used to debug, disable wachdog timer, 
  Serial.begin(115200);                           // full speed to monitor
  Wire.begin(0,2);                                // Initialize I2C and OLED Display
  init_OLED();                                    // 
  reset_display();
  WiFi.begin(SSID, PASS);                         // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {         // Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.print("SSID : ");                        // prints SSID in monitor
  Serial.println(SSID);                           // to monitor             
  sendStrXY("SSID :" ,0,0);  sendStrXY(SSID,0,7); // prints SSID on OLED

  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
  server.on("/", []() {
    server.send(200, "text/html", form);
  });
  server.on("/msg", handle_msg);                   // And as regular external functions:
  server.begin();                                  // Start the server 
  // clear_display();
  Serial.println("WebServer ready!   ");
  sendStrXY("WebServer ready!",2,0);               // OLED first message 
  Serial.println(WiFi.localIP());                  // Serial monitor prints localIP
}


void loop(void) {
    server.handleClient();                         // checks for incoming messages
}
