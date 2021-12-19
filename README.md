# E-Paper-Google-Calendar

![E-Paper Google Calendar](https://user-images.githubusercontent.com/37389453/146692164-fb12994a-7922-4ad3-be87-a993fff68c5c.jpg)

This is an E-paper calendar that connects to Google Calendar using C++. It displays the date, a total of seven agendas, when the display was last updated, and battery voltage. 
The goal was to give the user a battery ran calendar that connected to google. 

This was my college senior project. I hope to leave it here as an inspiration to others and for others to understand how projects on a deadline work. This is not a perfect device. In fact, it sometimes can't even tell you the right date. That has to do with the timer section of the code and the ESP32 itself. Ironically, I ran out of time on the project before the presentation. This can easily be rectified with an added timer. 
<br/>
<br/>

**DESIGN PROCESS**

This project was created in several stages. First an idea was hatched, materials were researched, a design plan was proposed, bill of materials was proposed (BOM), then the research and development (RnD) test phase, software test phase, a drafted schematic, built device, code development, and finally the deadline completion. Along with this I was responsible for updating my work in a journal weekly, progress reports, creating design flow charts, time management, and a presentation. 

**LIST OF MATERIALS**

  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [trigBoardv8](https://trigboard-docs.readthedocs.io/en/latest/Hardware.html) <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [e-Paper Driver HAT from Waveshare](https://www.waveshare.com/wiki/E-Paper_Driver_HAT) <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [7.5inch HD e-Paper Tricolor Display](https://www.waveshare.com/product/7.5inch-hd-e-paper-hat-b.htm) <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [TC4056A Lithium Battery Charger and Protection Module](https://www.amazon.com/gp/product/B07CZWDT8M/ref=ppx_yo_dt_b_asin_image_o02_s01?ie=UTF8&psc=1) <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [Lithium polymer battery](https://www.adafruit.com/product/2011) <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [Slide Switch](https://www.amazon.com/DIYhz-20Pcs-Position-Vertical-Switches/dp/B075RC6TFB) <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [2.0mm JST connectors](https://www.adafruit.com/product/4422) <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [Pin Headers](https://www.amazon.com/gp/product/B07NM68FXK/ref=ppx_yo_dt_b_asin_image_o02_s01?ie=UTF8&psc=1) <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [Wire](https://www.amazon.com/Electrical-colors-spools-UL1007-breadboard/dp/B083DNGSPV/ref=pd_rhf_ee_p_img_3?_encoding=UTF8&psc=1&refRID=MC23F6D6B634MKB1Y8YA) <br/>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [9cm x15cm Perf board](https://www.amazon.com/gp/product/B07XYLDTGC/ref=ppx_yo_dt_b_asin_image_o00_s01?ie=UTF8&th=1) <br/>

**SCHEMATIC**
![Schematic](https://user-images.githubusercontent.com/37389453/146691390-67704622-f39d-40d6-a083-ca6bc904ef25.jpg) 

**HARDWARE DESCRIPTION**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Starting with the charge and protection circuit. It is a standalone board that utilizes a [TC4056 charge control IC](https://www.addicore.com/TP4056-Charger-and-Protection-Module-p/ad310.htm), [DW01A Battery Protection IC](http://hmsemi.com/downfile/DW01A.PDF), and [FS8205A Dual N-Channel MOSFET](https://datasheetspdf.com/pdf-file/976676/CanShengIndustry/FS8205A/1). It has 2 LEDs, the red is used for “charging” and blue is used for “done charging.” It also has a micro-USB for easy hook up. The charge control IC is used to keep a constant current and voltage when charging the battery, where as the battery protection IC shuts off power with the MOSTFET when the battery voltage dips down too low. The output of this board is hooked up to OUT + and OUT – while the battery is hooked up to B+ and B- respectively. The battery is simply plugged into a JST connector and always connected to the charge protection circuit.<br/> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The switch is used to interrupt the connection between the positive output of the charge protection circuit to the trigBoard. The reason for this is that if someone wants to safely charge the battery using the charge protection circuit the load must be taken off as to ensure there is no damage to the load and the battery charges within a reasonable amount of time. It also doubles as an ON/OFF switch for the microcontroller to avoid wasting battery life.<br/> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The trigBoard is a breakout board for an ESP32 microcontroller with a Real Time Clock (RTC) and comes with supported firmware for things like over the air programming and the onboard RTC. The ESP32 is capable of Bluetooth and Wi-Fi connection. The trigBoard is connected to the switch through the positive battery terminal and to the protection circuit via the negative terminal. Lastly the trigBoard is connected its respective GPIO to the GPIO of the Waveshare e-Paper Driver HAT (Waveshare GPIO). <br/> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; The Waveshare e-Paper Driver HAT is powered through the trigBoard, as to power down when not used on time with said trigBoard. It receives all its signals from the GPIO header underneath the HAT from the trigBoard. The board itself is set to 4-line SPI and (B) other display, to support a 7.5” e-Paper display.  
