# ESP8266-I2C-OLED
A I2C screen to show messages received from a client-browser.

Copy the ESP_Messenger into your Arduino IDE and save as a new file.

Copy the Oled display driver file into the same folder.

Also copy the font.h file in the same folder.

If you choose generic ESP module as boardtype it will compile without errors.

Find out the local IP of your ESP. 
Either look in your router what IP adress it got or watch the monitor window while the USB-Serial adapter (needed to upload the file), the localIP will be printed once on the monitor.

In your browser type the localIP of the ESP server, it will open a nice page where you can submit a string of text.
This text will show up on the oled that is connected to the ESP.

Connections Oled - ESP8266:
Oled-SDA connected to GPIO0 of ESP
Oled-SCL connected to GPIO2 of ESP

