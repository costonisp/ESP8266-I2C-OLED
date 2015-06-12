# ESP8266-I2C-OLED
A I2C screen to show messages received from a client-browser.

Copy the ESP_Messenger into your Arduino IDE and save as a new file.
Also copy the font.h file in the same folder.

Make sure you installed ARDUINO-ESP-IDE addon. https://github.com/sandeepmistry/esp8266-Arduino
In the Arduino IDE you choose generic ESP module as boardtype it will compile without errors.

On start-up the local-IP is shown on the OLED.
In your browser type the local-IP of the ESP server, it will open a simple page where you can submit a string of text.
This text will be shown on the oled that is connected to the ESP.

Connections Oled - ESP8266:

Oled-SDA connected to GPIO0 of ESP

Oled-SCL connected to GPIO2 of ESP
The I2C Oled board used has a SDD1306 controller. I did not (yet) test this sketch different controllers line SH1106.

