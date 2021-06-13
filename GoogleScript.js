/***************************************************************************************
Name: ThatPersonOverThere
Date: April/9-13/2021
Project Name: E-Paper-Google-Calendar

Code Description: This code outputs google calendar data, such as the date, start time, 
end time, and event name from a specific calendar as a text ouptut in the format of a 
public web app. This information then can be 'get' or requested from arduino based 
hardware/software as one large string. Then displayed on said microcontroller. 
In spcific an ESP32 trigBoardv8 is accessing this information.  

Based from this code: https://github.com/trandi/e-paper-calendar

Useful websites: 
    https://developers.google.com/apps-script/reference/calendar/calendar-app
    https://developers.google.com/apps-script/guides/web

Instructions:
1. Paste this code into a Google Apps Script project. 
2. Grab the ID of the calender you wish to use 
(https://help.crystalcommerce.com/en/articles/1389811-how-to-find-your-google-calendar-i
d)
    place that id into the variable "cal"
3. Save...
3. Run the app, then google will ask you to authorize your code, click "Review 
   Permissions" then select your google account. Next it will say "Google has not 
   varified this app" you need to click "advanced" which will then let you click
    "Go to [your project name here] (unsafe)" then click "allow"
4. Run the code again. Make sure it works otherwise you wont get anything meaniful on 
   the microcontroller later. 
5. Next you need to deploy this code as a web app. There is a blue botton on the top 
   right that says "Deploy" click this and a drop down menu will appear. Click "New
   Deployment" On the top left of a new pop up it will say "Select type" click the 
   gear next to that phrase then select "Web app" fill in the description as anything
   you want. Under "Who has access" you need to select "Anyone" other wise it will 
   not work. Then click "Deploy" on the bottom right hand corner of the pop up. 
6. Copy the Web App Url and paste that into the C++ code.
***************************************************************************************/

/***************************************************************************************
 * Function: doGet(e)
 * Responsible for running the script and sending information out as an HTML or text 
 * output. This one outputs text information.
 * Each event is a on a seperate line. Each line contains the date, start time, end 
 * time, and event name. Should the calender not be found then the script will return
 * "no access to calender" should there be no events "No Events Present!" will be sent
***************************************************************************************/
function doGet(e) {  
  //Access the calender by its specific ID                      
  var cal = CalendarApp.getCalendarById('_____@group.calendar.google.com'); 
  //would look like 'SDdaRX9xEAOPkasdfkdsa8fjshdl@group.calendar.google.com'
  
    //If the calender ID is incorrect or does not exsist it will be undefined then the 
	// script will return 'no access to calender'  
    if (cal == undefined) {
	 //Make Google's Exicution log show the output
     console.info("No access to calendar"); 
     return ContentService.createTextOutput('No access to calendar');
    }
 
  //Declare the "current" date/time and one week from the start date/time 
  var start = new Date(); 
  const oneWeek = 7*24*3600000; // One week or 7days * Hours in day * msec of 1 hour
  
  //Declare how many days the events will be fetched from the current date
  const stop = new Date(start.getTime() + oneWeek);
  var events = cal.getEvents(start, stop);
  
  //Declare the string that will become the text ouptut containing event information
  var eventInformation = ''; //MUST BE '' OTHERWISE NULL CHECK LOWER DOWN WILL NOT WORK 

  //Declare a variable to ensure that we send a max of 20 events 
  var numOfEvents = 1; //Start at 1 for easy visualization 

  //For loop: Increase the count the count(ii) for as long as count(ii) is less than the
  // total number of events spanning from 7 days and the number of events is less than 
  // or equil to 7 so our ESP32 doesnt get an infinate string just incase! (SEE OUTPUT3)

  for (var ii = 0; numOfEvents <= 7 && ii < events.length; ii++) {
    
    //Declare a place holder variable to make easier type string below VVVVVV
    var event=events[ii]; 
      
      //Put together the long string that will be our text output
      eventInformation +=
        //Get the starting times of the events as month, day, time, year
        event.getStartTime().toString().substr(4,3)   + '_' + //Month       
        event.getStartTime().toString().substr(8,2)   + '~' + //Day         
        event.getStartTime().toString().substr(16,5)  + '^' + //Time  
        event.getStartTime().toString().substr(11,4)  + '`' + //Year        
        
        //Get the end times of the events as month, day, time, year
        event.getEndTime().toString().substr(4,3)     + '[' + //            
        event.getEndTime().toString().substr(8,2)     + ']' + //            
        event.getEndTime().toString().substr(16,5)    + '{' + //    
        event.getEndTime().toString().substr(11,4)    + '}' + //            

        //Finally get that event title 
        event.getTitle() + '\n';    

        //Increment number of events 
        numOfEvents++;

  }//End of for loop
  
 //If eventInformation has no value, ie empty or null (not truthy) then make text output


 // say 'No events Present!' returns the pharase like OUTPUT 2
 if(!eventInformation){
    console.info('No Events Present!'); //Make Google's Exicution log show the output
    
	//Returns eventInfomration from get request 
	return ContentService.createTextOutput('No Events Present!'); 
  }

  //Returns the all events found like OUTPUT 1
  console.info(eventInformation); //Make Google's Exicution log show the output
  
  //Returns eventInfomration from get request 
  return ContentService.createTextOutput(eventInformation); 
}

//OUTPUT 1
//(DATA: SMONTH, SDAY, STIME, SYEAR, EMONTH, EDAY, ETIME, EYEAR, event name )
/********************************************OUTPUT SHOWING EVENTS**********************

 * 12:10:40 AM	Notice	Execution started
 * 12:10:39 AM	Info	Apr_05~00:00^2021`Apr[17]00:00{2021}LongBoiEvent
 *                    Apr_12~15:30^2021`Apr[22]16:30{2021}Event from 12th to 22nd
 *                    Apr_13~11:20^2021`Apr[13]12:20{2021}Event on 13th
 *                    Apr_14~11:30^2021`Apr[14]12:30{2021}Event on the 14th
 *                    Apr_15~12:00^2021`Apr[15]13:00{2021}Event on the 15
 *                    Apr_16~15:48^2021`Apr[16]16:59{2021}Event on 16th
 *                    Apr_17~18:30^2021`Apr[17]19:30{2021}Event on the 17th
 *                    Apr_18~00:00^2021`Apr[19]00:00{2021}Event on the 18th
 *                    Apr_18~15:00^2021`Apr[24]16:00{2021}Event from 18th to 24
 *                    Apr_19~00:00^2021`Apr[20]00:00{2021}Event on 19th
12:10:41 AM	Notice	Execution completed
 **************************************************************************************/

//OUTPUT 2
//I simply deleted all the events for the next week out to see what would happen 
/********************************************OUTPUT WITH NO EVENTS**********************

 * 4:39:08 AM	Info	No Events Present!
 **************************************************************************************/

//OUTPUT3
//Note there was an event labled "10" however it does not show because of the 20 event 
// limit, NOTE THAT THAT EVENT WAS THE FIRST CREATED ON THAT DAY and it was excluded, 
// this is because of how Google handles all day events... it would be fine if each 
// event had a separate special time like in OUTPUT 4, I'd call this a weakness I will
// comeback to when if i have time... 
/***************************************************************************************

 * 5:02:00 AM	Notice	Execution started
 * 5:01:59 AM	Info	Apr_05~00:00^2021`Apr[17]00:00{2021}LongBoiEvent 1
 *                  Apr_13~11:20^2021`Apr[13]12:20{2021}Event on 13th 2
 *                  Apr_14~11:30^2021`Apr[14]12:30{2021}Event on the 14th 3
 *                  Apr_15~12:00^2021`Apr[15]13:00{2021}Event on the 15 4
 *                  Apr_16~15:48^2021`Apr[16]16:59{2021}Event on 16th 5
 *                  Apr_17~18:30^2021`Apr[17]19:30{2021}Event on the 17th 6
 *                  Apr_18~00:00^2021`Apr[19]00:00{2021}Event on the 18th 7
 *                  Apr_18~15:00^2021`Apr[24]16:00{2021}Event from 18th to 24 8
 *                  Apr_19~00:00^2021`Apr[20]00:00{2021}Event on 19th 9
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}21
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}20
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}19
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}18
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}17
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}16
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}15
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}14
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}13
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}12
 *                  Apr_20~00:00^2021`Apr[21]00:00{2021}11
 * 5:02:01 AM	Notice	Execution completed
***************************************************************************************/

//OUTPUT 4
//Event 21 does exsist however it wont show b/c of the 20 event limit (this was since 
// changed to 7 but still applies)
/***************************************************************************************

 * 5:15:13 AM	Notice	Execution started
 * 5:15:11 AM	Info	Apr_05~00:00^2021`Apr[17]00:00{2021}LongBoiEvent 1
 *                  Apr_13~11:20^2021`Apr[13]12:20{2021}Event on 13th 2
 *                  Apr_14~11:30^2021`Apr[14]12:30{2021}Event on the 14th 3
 *                  Apr_15~12:00^2021`Apr[15]13:00{2021}Event on the 15 4
 *                  Apr_16~15:48^2021`Apr[16]16:59{2021}Event on 16th 5
 *                  Apr_17~18:30^2021`Apr[17]19:30{2021}Event on the 17th 6
 *                  Apr_18~00:00^2021`Apr[19]00:00{2021}Event on the 18th 7
 *                  Apr_18~15:00^2021`Apr[24]16:00{2021}Event from 18th to 24 8
 *                  Apr_19~00:00^2021`Apr[20]00:00{2021}Event on 19th 9
 *                  Apr_19~01:00^2021`Apr[19]02:00{2021}10
 *                  Apr_19~02:00^2021`Apr[19]02:30{2021}11
 *                  Apr_19~03:00^2021`Apr[19]03:30{2021}12
 *                  Apr_19~04:00^2021`Apr[19]04:30{2021}13
 *                  Apr_19~05:00^2021`Apr[19]05:30{2021}14
 *                  Apr_19~06:00^2021`Apr[19]06:30{2021}15
 *                  Apr_19~07:00^2021`Apr[19]07:30{2021}16
 *                  Apr_19~08:00^2021`Apr[19]08:30{2021}17
 *                  Apr_19~09:00^2021`Apr[19]09:30{2021}18
 *                  Apr_19~10:00^2021`Apr[19]10:30{2021}19
 *                  Apr_19~11:00^2021`Apr[19]11:30{2021}20
 * 5:15:14 AM	Notice	Execution completed
***************************************************************************************/

//Notes:
  //Tasks do not show with this scripts only events show... (because tasks are under a 
  // differnt calender ID)
  
  //A person can still type "\n" into the event name thus "\n" is not a good delimiter
  
  //The delimiters chosen are least likely to be used in event names but still typeable 


  // into the event name
  
  //The delimiters will be an achilles heel if used when making an event name/title as
  //they are used to break the information in the C++ code into arrays to try and 
  //prevent this the event title was written last
  
  //getStartTime() and getEndTime() are both formatted as dates, rather than using 
  // toDateString() which has a error/ not reliable code for some reason
  
  //A character limit less than that of the full getStartTime/EndTime date, toString()
  // was used
  
  //When a 00:00 to 00:00 is seen for the times it means that that event is an all day
  // event
  
  //Events that were started in the past but are currently ongoing will show first 
  // (Shorter longboi & Long Boi event)