/*
Version 1.0 supports OLED display's with either SDD1306 or with SH1106 controller
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "font.h"
//#define offset 0x00    // SDD1306                      // offset=0 for SSD1306 controller
#define offset 0x02    // SH1106                       // offset=2 for SH1106 controller
#define OLED_address  0x3c                             // all the OLED's I have seen have this address
#define SSID "........"                              // insert your SSID
#define PASS "........"                              // insert your password
// ******************* String form to sent to the client-browser ************************************
String form =
  "<p>"
  "<center>"
  "<h1>Talk to me :-)</h1>"
  "<img src='http://i.imgur.com/qu8lDEu.jpg'>"
  "<form action='msg'><p>Wassup? <input type='text' name='msg' size=50 autofocus> <input type='submit' value='Submit'></form>"
  "</center>";

ESP8266WebServer server(80);                             // HTTP server will listen at port 80
 long period;

/*
  handles the messages coming from the webbrowser, restores a few special characters and 
  constructs the strings that can be sent to the oled display
*/
void handle_msg() {
  clear_display();                        // clears oled
 
  server.send(200, "text/html", form);    // Send same page so they can send another msg

  // Display msg on Oled
  String msg = server.arg("msg");
  Serial.println(msg);
  String decodedMsg = msg;
  // Restore special characters that are misformed to %char by the client browser
  decodedMsg.replace("+", " ");      
  decodedMsg.replace("%21", "!");  
  decodedMsg.replace("%22", "");  
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");  
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'");  
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");  
  decodedMsg.replace("%2C", ",");  
  decodedMsg.replace("%2F", "/");   
  decodedMsg.replace("%3A", ":");    
  decodedMsg.replace("%3B", ";");  
  decodedMsg.replace("%3C", "<");  
  decodedMsg.replace("%3D", "=");  
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");  
  decodedMsg.replace("%40", "@"); 
  //Serial.println(decodedMsg);                   // print original string to monitor
  unsigned int lengte = decodedMsg.length();      // length of received message
  for  (int i=0;i<lengte;i++)                     // prints up to 8 rows of 16 characters.
    {
      char c = decodedMsg[i];
      Serial.print(c); //decodedMsg[i]);
           if (i<16)  {sendCharXY(c,0,i);}
      else if (i<32)  {sendCharXY(c,1,i-16);}
      else if (i<48)  {sendCharXY(c,2,i-32);}
      else if (i<64)  {sendCharXY(c,3,i-48);}
      else if (i<80)  {sendCharXY(c,4,i-64);}
      else if (i<96)  {sendCharXY(c,5,i-80);}
      else if (i<112) {sendCharXY(c,6,i-96);}
      else if (i<128) {sendCharXY(c,7,i-112);}

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
  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
  server.on("/", []() {
    server.send(200, "text/html", form);
  });
  server.on("/msg", handle_msg);                  // And as regular external functions:
  server.begin();                                 // Start the server 
  clear_display();

  Serial.print("SSID : ");                        // prints SSID in monitor
  Serial.println(SSID);                           // to monitor             
  sendStrXY("SSID :" ,0,0);  sendStrXY(SSID,0,7); // prints SSID on OLED
 
  char result[16];
  sprintf(result, "%3d.%3d.%3d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  Serial.println();Serial.println(result);
  sendStrXY(result,2,0);

  Serial.println("WebServer ready!   ");
  sendStrXY("WebServer ready!",4,0);              // OLED first message 
  Serial.println(WiFi.localIP());                 // Serial monitor prints localIP
  Serial.print(analogRead(A0));
  int test = 13;
  pinMode(test,OUTPUT);
  digitalWrite(test,HIGH);
  delay(1000);
  digitalWrite(test,LOW);
}


void loop(void) {
  server.handleClient();                        // checks for incoming messages
}

//==========================================================//
// Resets display depending on the actual mode.
static void reset_display(void)
{
  displayOff();
  clear_display();
  displayOn();
}

//==========================================================//
// Turns display on.
void displayOn(void)
{
  sendcommand(0xaf);        //display on
}

//==========================================================//
// Turns display off.
void displayOff(void)
{
  sendcommand(0xae);		//display off
}

//==========================================================//
// Clears the display by sendind 0 to all the screen map.
static void clear_display(void)
{
  unsigned char i,k;
  for(k=0;k<8;k++)
  {	
    setXY(k,0);    
    {
      for(i=0;i<(128 + 2 * offset);i++)     //locate all COL
      {
        SendChar(0);         //clear all COL
        //delay(10);
      }
    }
  }
}

//==========================================================//
// Actually this sends a byte, not a char to draw in the display. 
// Display's chars uses 8 byte font the small ones and 96 bytes
// for the big number font.
static void SendChar(unsigned char data) 
{
  //if (interrupt && !doing_menu) return;   // Stop printing only if interrupt is call but not in button functions
  
  Wire.beginTransmission(OLED_address); // begin transmitting
  Wire.write(0x40);//data mode
  Wire.write(data);
  Wire.endTransmission();    // stop transmitting
}

//==========================================================//
// Prints a display char (not just a byte) in coordinates X Y,
// being multiples of 8. This means we have 16 COLS (0-15) 
// and 8 ROWS (0-7).
static void sendCharXY(unsigned char data, int X, int Y)
{
  setXY(X, Y);
  Wire.beginTransmission(OLED_address); // begin transmitting
  Wire.write(0x40);//data mode
  
  for(int i=0;i<8;i++)          
    Wire.write(pgm_read_byte(myFont[data-0x20]+i));
    
  Wire.endTransmission();    // stop transmitting
}

//==========================================================//
// Used to send commands to the display.
static void sendcommand(unsigned char com)
{
  Wire.beginTransmission(OLED_address);     //begin transmitting
  Wire.write(0x80);                          //command mode
  Wire.write(com);
  Wire.endTransmission();                    // stop transmitting
}

//==========================================================//
// Set the cursor position in a 16 COL * 8 ROW map.
static void setXY(unsigned char row,unsigned char col)
{
  sendcommand(0xb0+row);                //set page address
  sendcommand(offset+(8*col&0x0f));       //set low col address
  sendcommand(0x10+((8*col>>4)&0x0f));  //set high col address
}


//==========================================================//
// Prints a string regardless the cursor position.
static void sendStr(unsigned char *string)
{
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      SendChar(pgm_read_byte(myFont[*string-0x20]+i));
    }
    *string++;
  }
}

//==========================================================//
// Prints a string in coordinates X Y, being multiples of 8.
// This means we have 16 COLS (0-15) and 8 ROWS (0-7).
static void sendStrXY( char *string, int X, int Y)
{
  setXY(X,Y);
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      SendChar(pgm_read_byte(myFont[*string-0x20]+i));
    }
    *string++;
  }
}


//==========================================================//
// Inits oled and draws logo at startup
static void init_OLED(void)
{
  sendcommand(0xae);		//display off
  sendcommand(0xa6);            //Set Normal Display (default)
    // Adafruit Init sequence for 128x64 OLED module
    sendcommand(0xAE);             //DISPLAYOFF
    sendcommand(0xD5);            //SETDISPLAYCLOCKDIV
    sendcommand(0x80);            // the suggested ratio 0x80
    sendcommand(0xA8);            //SSD1306_SETMULTIPLEX
    sendcommand(0x3F);
    sendcommand(0xD3);            //SETDISPLAYOFFSET
    sendcommand(0x0);             //no offset
    sendcommand(0x40 | 0x0);      //SETSTARTLINE
    sendcommand(0x8D);            //CHARGEPUMP
    sendcommand(0x14);
    sendcommand(0x20);             //MEMORYMODE
    sendcommand(0x00);             //0x0 act like ks0108
    
    //sendcommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
    sendcommand(0xA0);
    
    //sendcommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
    sendcommand(0xC0);
    
    sendcommand(0xDA);            //0xDA
    sendcommand(0x12);           //COMSCANDEC
    sendcommand(0x81);           //SETCONTRAS
    sendcommand(0xCF);           //
    sendcommand(0xd9);          //SETPRECHARGE 
    sendcommand(0xF1); 
    sendcommand(0xDB);        //SETVCOMDETECT                
    sendcommand(0x40);
    sendcommand(0xA4);        //DISPLAYALLON_RESUME        
    sendcommand(0xA6);        //NORMALDISPLAY             

  clear_display();
  sendcommand(0x2e);            // stop scroll
  //----------------------------REVERSE comments----------------------------//
    sendcommand(0xa0);		//seg re-map 0->127(default)
    sendcommand(0xa1);		//seg re-map 127->0
    sendcommand(0xc8);
    delay(1000);
  //----------------------------REVERSE comments----------------------------//
  // sendcommand(0xa7);  //Set Inverse Display  
  // sendcommand(0xae);		//display off
  sendcommand(0x20);            //Set Memory Addressing Mode
  sendcommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
  //  sendcommand(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)  
}


