/*********************************************************************************************************************************************
 * ThatPersonOverThere, APR 15-29 2021
 * Project: E-Paper-Google-Calendar
 * 
 * This is the main file that controls everything. It simply calls the nessary functions to complete tasks. 
 *********************************************************************************************************************************************/

//Nessary Includes
#include "Config.h"   //For user to configure
#include "includes.h" //For all variables and declartions


void setup() {

    //Boot count check to see if we have gon the full 8 hours after start!
    // we dont want to see any feed back for 8 hours so we dont initalize the display/serial monitor...
    TimerMode(rtc_long_timer_in_seconds);

    //Serial.begin(115200); (PLESAE UNDERSTAND THAT THIS IS NOT NESSARY WITH ESP32 DEV BOARD MANAGER V1.0.6 DUE TO AN ESP32 TIMING ERROR AND THE GXEPD2 LIBRARY)
    //If we want to see all our serial print stuff this needs to go here unfortunately due to the error mentioned above
    display.init(115200); // uses standard SPI pins, e.g. SCK(14), MISO(12), MOSI(13), SS(15) 
     
    //Connect to wifi and get Time/ Date / Google Calendar adjends
    ConnectGet();
    
    //Parse the Google data 
    ParseData();
    
    //Get battery voltage 
    Serial.print(GetBatteryVoltage(), 2); Serial.println('V');
    
    //Display the data
    DisplayData();
    
    //Turn of ESP32 and let the timer go for 8 inderations of 30mins!  
    TimerMode(rtc_long_timer_in_seconds);
    
}//end of void setup

//Unused loop
void loop() {/*WE NEVER REACH HERE DUE TO THE NATURE OF DEEP SLEEP MODE*/}
