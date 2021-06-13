/*********************************************************************************************************************************************
 * ThatPersonOverThere, APR 15-29 2021
 * Project: E-Paper-Google-Calendar
 * 
 * TimerMode fucntion:
 * Responsible for the deep sleep classifyed as hibernation. Hibernation differs from deep sleep. Link below tells you the difference
 * SEE pg 31 of: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
 * 
 * Within this function boot count ins incremented from 0, then printed on the serial monitor. Next, wake up via timer opposed to inputs like 
 * GPIO or touch (Capacitive) is enabled for 60s or 1 minute. Next several periferals are forced off, this is to avoid the ESP32 setting them
 * to AUTO, which dependint on the script could keep the co-processor on unessairly. The RTC's fast and slow memmory, IO periferals, sensors,
 * and co-processor domain is turned off. Once these are turned off, we now send the ESP32 into deep sleep hibernation. 
 *********************************************************************************************************************************************/
void TimerMode(int rtc_time) {

  //Check to see if rtc_time was 300s or 5 minutes in which case we want
  // a 5 minute reboot to check the wifi. We turn the memory off in this
  // case because it has no use here... (not event first boot flag matters)
  if (rtc_time == rtc_short_timer_in_seconds)
  {
    // Must setup the way the ESP32 will wake up first
    esp_sleep_enable_timer_wakeup(rtc_time * convert_to_microseconds);
    Serial.println("Timer wake up is initalized.");

    //Force Hiberbation mode in deep sleep
    //Hibernation mode is a defined subset of Deep sleep and is refred to as Deep sleep in hard code
  
    //Force number of power domains see pg 18 of:
    //(https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);   
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF); //Force RTC IO, sensors, and ULP co-processor off
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF); //Force RTC slow memmory off (When just this is OFF boot count fails even update!)
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF); //Force RTC fast memmory off (when just this is OFF boot count reads large value not a count)
    Serial.println("Forced everything off boi.");
  
    //Once this is called that means that we will now go to sleep
    Serial.println("Ima go to sleep now see ya in a while, may be 5min maybe 4hrs!");
    esp_deep_sleep_start();
  }//end of if rtc_time = 300
  
  //If its our first boot we want to start our program!
  if (firstBootFlag == 0) 
  {
    ++firstBootFlag; //inc for the first boot so we dont flag this on the subsequent boots
    bootCount = 0;   //make sure this is zero!

    //Print stuff on the screen so we can see!
    Serial.println("firstBootFlag: " + String(firstBootFlag));
    Serial.println("bootCount: " + String(bootCount));

    //Return to go through the program first iteration.
    return;          
  }
  
  //If the boot count has gone through 16 iderations of 30 minutes then
  // reset the boot count and start the program! Note that we need to 
  // use 17 because on the first sleep cycle boot count is incremented
  // thus 30 minutes had not passed and thus it would not be 8 hours
  // but instead 7 hours 30 minutes. 
  else if (bootCount >= numOfBootsToUpdate)
  { 
    bootCount = 0;  //reset the boot count 
    return;         //return to do the rest of the program
  }//end of if/elseif

  //No matter what when this function is called the ESP32 should go to sleep... 
  // Setup the RTC Boot Counter
  ++bootCount;
  Serial.println("Boot Count: " + String(bootCount));
  
  // Must setup the way the ESP32 will wake up first
  esp_sleep_enable_timer_wakeup(rtc_time * convert_to_microseconds);
  Serial.println("Timer wake up is initalized.");

  //Force Hiberbation mode in deep sleep
  //Hibernation mode is a defined subset of Deep sleep and is refred to as Deep slee in code
  
  //Force number of power domains see pg 18 of:
  //(https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
  esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);   
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF); //Force RTC IO, sensors, and ULP co-processor off
  //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF); //Force RTC slow memmory off (When just this is OFF boot count fails even update!)
  //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF); //Force RTC fast memmory off (when just this is OFF boot count reads large value not a count)
  Serial.println("Forced everything off boi.");
  
  //Once this is called that means that we will now go to sleep
  Serial.println("Ima go to sleep now see ya in a while, may be 5min maybe 4hrs!");
  esp_deep_sleep_start();
    
}//end of Timer Mode
