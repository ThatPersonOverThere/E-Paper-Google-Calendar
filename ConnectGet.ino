/*********************************************************************************************************************************************
 * ThatPersonOverThere, APR 15-22 2021
 * Project: E-Paper-Google-Calendar
 * 
 * This function is designed connect to WiFi and then to a google web-app that uses JavaScript to get google calendar event data and output 
 * that data in a text format. This program gets that text output and displays it in the serial monitor.
 * This program also gets the time from the NTP server!
 *********************************************************************************************************************************************/ 
//Based from and using code by Dan Oprescu
//From: https://github.com/trandi/e-paper-calendar/blob/main/platformio/ePaperCalendar/src/ePaperCalendar.ino
//This author's code was the only one that worked when connecting to my own Google Script Thus I decided to keep most of it because it 
//worked flawessley opposed to the 20 or so other codes using HTTPS Redirct that repeatedly failed even with constant maintenance
//Most comments were done by myself with a few left from the creator 

void ConnectGet() {
   // 1. Serial initialisation
      Serial.println("Starting...");


   // 2. Check if the known network is present
      WiFi.mode(WIFI_STA);                      //Station mode, connects to an access point vs AP which makes the ESP32 ab access point
      WiFi.disconnect();                        //Dissconects from any network just in case

      //Declare variables for checking if the network connectivity 
      int netCount{WiFi.scanNetworks()};        //Check how many networks there are 
      bool foundKnownSSID(false);               //Set false so IF in the For loop fails the ESP32 will go to sleep because there is no wifi to find... 
      
      Serial.println("WiFi networks:");

      //For less than as many scanable networks present within the area  
      for (int i = 0; i < netCount; i++) 
      {
        Serial.println(WiFi.SSID(i));
        if(WiFi.SSID(i) == SSID) foundKnownSSID = true; //If wifi SSID is found make foundKnowSSID true
      }
      
      Serial.printf("SSID %s found: %d\n\n", SSID, foundKnownSSID);

  /*IF ELSE Statment*********************************************************************************************************************************
   * After our network SSID is found the ESP32 will atempt to connect to the access point hosting the network. Once connection to the access is
   * made, the esp32 will connect to the NTP server and get day/date/month/time(military)/ lastly google text data will be retireved. 
   *********************************************************************************************************************************************/
      if(foundKnownSSID) {
        const unsigned long startTime= (millis()); //capture the start time 

           //3. WiFi
              Serial.println("Starting WiFi...");
              int ii = 100; //Declare a second count integer above 20 for later if statment
              
              //While the WiFi status is not connected and the time passed since the microcontroller minus the total boot/start time
              //  is less than the max wait of 60s or 1 minute, attempt to connnect to the access point  
              while ((WiFi.status() != WL_CONNECTED) && (millis() - startTime < MAX_WAIT_MILLIS)) {
                Serial.printf("WiFi conn status: %d\n", WiFi.status());

                //If the count ii is greater than 20 then disconnect from any network and re-attempt to connect to access point
                //This is a delay via if statment and delay() function together both produce quicker and more reliable connection
                //  rather than either one alone, because the if statement is too quick and delay causes issues alone unless the 
                //  delay is around 8 seconds which is too slow! 
                if( ii > 20) {
                  WiFi.disconnect(true); //Force a disconnect to be sure ESP32 is not connected to anything
                  WiFi.begin(SSID, PASSWORD);
                  ii = 0;
                }
                
                delay(400); //If we dont have this it will attempt to connect too fast and be stuck in a connect loop   vv(80MHz)vv
                            //THIS WILL DEPEND ON THE CHOSEN SPEED OF MICROCONTROLLER, ESP32 IS CAPIBLE OF RUNNING AT DIFFERET FREQUENCYS
                            
                ii++;//Increment the count
              }//end of while
              
              Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
                                               //Make the int a string to place in 'printf'
  /*GET TIME DATA ******************************************************************************************************************************
   * Part of If(foundKnownSSID), uses time.h, This gets the time data from the Network Time Protocol(NTP) server and displays it on the serial
   * monitor. 
   * This can help for understanding this:
   * https://randomnerdtutorials.com/esp32-date-time-ntp-client-server-arduino/
   *********************************************************************************************************************************************/
            //Intalize and get time from NTP server!
            configTime(UTC_EST_OFFSET_SECS, DAYLIGHT_OFFSET_SECS, NTP_SERVER);
            struct tm timeInfo; //Place for data

            //If we are successful in getting the data than print the data else we didnt get time from NTP
            if(getLocalTime(&timeInfo)) 
            {
              Serial.println(&timeInfo, "%A %B %d %Y %H:%M");

              //Stick our week, month, day, and year in variables
              strftime(theWeekday,10, "%A", &timeInfo);   //Full day of week
              strftime(theMonth,10, "%B", &timeInfo);  //Full Month
              strftime(theDay,3, "%d", &timeInfo);    //day
              strftime(theYear,5, "%Y", &timeInfo);   //Year

              //Stick our Time in Hours and Minutes into Variales to display on last updated screen 
              strftime(lastUpdatedHour,3, "%H", &timeInfo);
              strftime(lastUpdatedMinute,3, "%M", &timeInfo);
              
              Serial.println(&timeInfo, "%H:%M");} 
            else 
            {
            Serial.println("NO Time from NTP");
            noTimeFromNTP_Flag = true;
            }//end of if/else
        
  /*GET CALENDAR DATA **************************************************************************************************************************
   * Part of If(foundKnownSSID), uses HTTPClient.h, in this we set up a get request to a google script web app, make sure the connection is 'OK' 
   * or has failed. If its successful we go ahead and get the text output of google script. If it is not successful then we get the error code.
   *********************************************************************************************************************************************/
            struct HTTPClient http;                                              //Custom declared variable within scope of HTTPClient libray 
            int httpCode = -111;                                                 //Make http code less than 0 to ensure while starts
            
            //While httpCode is less than 0 and the time since this loop started is less than a minute
            while((httpCode <= 0) && (millis() - startTime < MAX_WAIT_MILLIS)) {
              http.setFollowRedirects(followRedirects_t::HTTPC_STRICT_FOLLOW_REDIRECTS);  //necessary as Google Script Webapp URL will redirect
              http.begin(GoogleScript_URL, ROOT_CA);                                      //Begin connection to Google scripts
              httpCode = http.GET();                                                      //Declare a holding varaible 
        
              //If the httpCode is greater than 0 (a 200 means 'OK') then get the text ouptut
              //  otherwise print out the error code 
                if(httpCode > 0) {
                  GS_DATA = http.getString();                                      //Place google script output in to the string payload 
                  Serial.printf("HTTP response code: %d\n", httpCode);             //if the code recieved is 200 it means the connection was 'OK'
                  Serial.println(GS_DATA);                                         //See what the text looks like! 
                } else {
                  Serial.printf("Error on HTTP request, code: %d\n", httpCode);    //Prints the HTTP error we recieve a bad connection code
                }//end if else
                
              http.end(); //End the http connection/disconnect from script completely
              
            }//end while
            
  }//end of if(foundKnownSSID)
  //If foundKnownSSID is false then go to sleep because we cant get wifi without it...
  else {
    //Getting here means that we did not find a network we recognize so we are gonna go to sleep
    Serial.println("No Wifi Bra"); 
    TimerMode(rtc_short_timer_in_seconds); //Go to sleep 5 minutes
    
  }//end of if(foundKnownSSID)else
  
}//end of ConnectGet
