/*********************************************************************************************************************************************
 * ThatPersonOverThere, APR 15-29 2021
 * Project: E-Paper-Google-Calendar
 * 
 * DisplayData function:
 * This function is responsible for setting up the SPI pins for the display to the ESP32 trigBoard, then clearing the display, Next it draws
 * the top of the screen rectangle and white text that displays the date inside it. Then It will print "No events present" or all the events 
 * from GS_DATA in 16 size font and under them their start and end times in 12 font, while creating red bullet points for each event. Then it
 * draws a line at the bottom for the last time updated and the battery voltage to show up in the left and right corners respectivly. Lastly
 * the display is then powered off. 
 *********************************************************************************************************************************************/
void DisplayData(){
  //Normally we would initalize the display here but because of the ESP32 we already did that...
  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
  // To override this behavior (so text will run off the right side of the display - useful for
  // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
  // with setTextWrap(true).


  //NESSARY TO WORK WITH trigBoardv8!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  SPI.end(); // release standard SPI pins, e.g. SCK(18), MISO(19), MOSI(23), SS(5)
  //SPI: void begin(int8_t sck=-1, int8_t miso=-1, int8_t mosi=-1, int8_t ss=-1);
  SPI.begin(SCK, MISO, MOSI, SS); // map and init SPI pins SCK(14), MISO(12), MOSI(13), SS(15)

  display.setFullWindow();  // Set full window mode, meaning is going to update the entire screen
  display.firstPage();  // Tell the graphics class to use paged drawing mode

  do
  {
      // Clear previous graphics to start over to print new things.
      display.fillScreen(GxEPD_WHITE); 
      
      //Make the top a black rectangle 
      display.fillRect( 0/*x0*/, 0/*y0*/, 880/*width*/, 64/*height*/, GxEPD_BLACK); 
       
      // Print variable with right alignment:
      display.setTextColor(GxEPD_WHITE);        // Set color for text
      display.setFont(&FreeSerif24pt7b);        // Set font
      display.setCursor(200, 45);               // Set the position to start printing text (x,y)
      
      //Check if we got time from NTP server if we didnt then we cant display the time obviously so we wont leav it blank tho...
      if (noTimeFromNTP_Flag == false)
      {
        display.printf("%s, %s %s %s", theWeekday,theMonth, theDay, theYear);
      }
      else{
        display.print("No Time From NTP Server");
       
      }//end of if/else

      //Check if there are events present that will determine if we need to display no events present, an error message, or the events
      if (GS_DATA == "No Events Present!")
      {
        // Print variable with right alignment:
        display.setTextColor(GxEPD_BLACK);        // Set color for text
        display.setFont(&FreeSerif18pt7b);        // Set font
        display.setCursor(36, 99);                // Set the position to start printing text (x,y)
        display.println(GS_DATA);

        //Because there are no events we want to put some pictures up to make the display pleaseing
        //That Person over there will display on the upper right corner hanging from the black bar
        //Kirby appears near the center of the display
        display.drawBitmap(700, 64, gImage_ThatPersonOverThere, 147, 186, GxEPD_BLACK); // Print ThaPersonOverThere (POSITION_X, POSITION_Y, IMAGE_NAME, IMAGE_WIDTH, IMAGE_HEIGHT, COLOR)
        display.drawBitmap(300, 200, gImage_kirby, 270, 239, GxEPD_RED); // Print kriby (POSITION_X, POSITION_Y, IMAGE_NAME, IMAGE_WIDTH, IMAGE_HEIGHT, COLOR)
        
      }
      else if (GS_DATA.startsWith("<!DOCTYPE",0))
      {
        // Print variable with right alignment:
        display.setTextColor(GxEPD_BLACK);        // Set color for text
        display.setFont(&FreeSerif18pt7b);        // Set font
        display.setCursor(36, 99);                // Set the position to start printing text (x,y)
        display.println("There was a problem getting Google's Data :'( ");

        //Display that person over there in this circumstance too! But this time we make kirby bigger and mad via differnt picture!!!
        display.drawBitmap(700, 64, gImage_ThatPersonOverThere, 147, 186, GxEPD_BLACK); // Print ThaPersonOverThere (POSITION_X, POSITION_Y, IMAGE_NAME, IMAGE_WIDTH, IMAGE_HEIGHT, COLOR)
        display.drawBitmap(250, 180, gImage_kirbyMad, 366, 321, GxEPD_RED); // Print kriby (POSITION_X, POSITION_Y, IMAGE_NAME, IMAGE_WIDTH, IMAGE_HEIGHT, COLOR)
        
        
      }//end of else if
      else {
        //adjusting y coordiantes, to travel down so event text does not 
        int y0 = 99; //For the event name
        int y1 = 125; //Start time of event
        int y2 = 88; //For our bullet points

       //For 8 events (to test stuff!), display the event name and start time and end time stored in arrays 
        for (int iii = 0; iii < numOfEventsParsed; iii++)
        {
          // Print Event Name
          display.setTextColor(GxEPD_BLACK);        // Set color for text
          display.setFont(&FreeSerif18pt7b);        // Set font smaller
          display.setCursor(36, y0);                // Set the position to start printing text (x,y)
          display.println(arrEventName[iii]);
          
          // Print Start time
          display.setFont(&FreeSerif12pt7b);        // Set font even smaller
          display.setCursor(62, y1);                // Set the position to start printing text (x,y) 
          display.printf("Start: %s %s %s ", arrSTmonth[iii], arrSTdate[iii], arrSTtime[iii]);

          //If the year changed between the event we want to display the year!!!
          if(arrSTyear[iii] != arrEDyear[iii])
          {
            display.print(arrSTyear[iii]);
          }

          // Print End time 
          display.setCursor(310, y1);                // use y1 to make the text seem as if its one long line  
          display.printf("End: %s %s %s ", arrEDmonth[iii], arrEDdate[iii], arrEDtime[iii]);
         
          //If the year changed between the event we want to display the year!!!
          if(arrEDyear[iii] != arrSTyear[iii])
          {
            display.print(arrEDyear[iii]);
          }

          //Make a circle for each event/bullet point cuz we fancy
          display.fillCircle( 16/*x*/, y2/*y*/, 10/*radius*/, GxEPD_RED);

          y0 = y0 + 62; //Increase the event Name spacing between events
          y1 = y1 + 62; //^^^^^^^^^^^^ start time and end time spacing between events
          y2 = y2 + 62;

        }//end of for loop
        

      }// end of if/else

        //Visual seperation for battery voltage
        display.drawLine( 0/*x0*/, 504/*y0*/, 880/*x1*/, 504/*y1*/, GxEPD_BLACK);

        
        //We want to display when this page was last updated so we will use the date function in the main program when we plug it in 
        display.setFont(&FreeSerif12pt7b); 
        display.setCursor(16,526); //set cursor x and y
        
        //If the ntp server flag is flase ie we have the time we can display it
        if (noTimeFromNTP_Flag == false){
          display.printf("Last updated: %s:%s %s %s %s ", lastUpdatedHour, lastUpdatedMinute, theMonth, theDay, theYear);
        }else
        {
          display.print("Last updated: ");
          display.setTextColor(GxEPD_RED); //make it red to show em that it messed up/pay attention to the error
          display.print("NTP ERROR");
        }
        
        //We want to print the battery voltage, Bewarned this will probably be a couple of points off the serial monitor as its calling the function again
        //that is if we were using the main program!! this is a test so we stick in a float variable named like it :)
        display.setCursor(650, 526); //Set cursor x and y
        display.setTextColor(GxEPD_BLACK);      //incase there was no time from NTP server
        display.print("Battery Voltage: ");
        display.setTextColor(GxEPD_RED);        // Make the voltage and "V" show red
        display.print(GetBatteryVoltage(), 2);
        display.print(" V");
        
 } //end of do loop 


  // Print everything we set previously (ABSLOUTELY NEEDS TO GO RIGHT AFTER THE DO LOOP OTHERWISE ERROR)
  while (display.nextPage()); 
  
  //Power the display off
  display.powerOff();

  //Tell us we are finished with this
  Serial.println("end of while loopdiedoo");
  
} //end of Display Data
