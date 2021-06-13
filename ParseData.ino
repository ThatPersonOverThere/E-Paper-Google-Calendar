/************************************************************************************************
 * ThatPersonOverThere, APR 13 2021
 * Project: E-Paper-Google-Calendar
 * 
 * ParseData function:
 * Responsible for all that shows on the serial monitor. First initiates communication with
 * the serial monitor, then declares arrays and variables. Then it uses a for loop + switch 
 * case to parse each character of GS_DATA for specific characters. When it does not find 
 * a character it adds the current character of GS_DATA into a place holder string. When it 
 * does find a specific character it sends out the array the placeholder string needs to  
 * write to and the array element number, to another function. 
 * After the for loop is complete GS_DATA is displayed and each array containing data 
 * origionally from GS_DATA is displayed
 ************************************************************************************************/
void ParseData() 
{    
  //IF the data we get ever starts with HTTP it means there is an error with Google scripts
  if(GS_DATA.startsWith("<!DOCTYPE",0))
  {
    Serial.println("There was an error with the Google Script");
  }     
  //Google scripts will tell us if there are no events so we simply pass it on
  else if(GS_DATA == "No Events Present!")
  {
    Serial.println(GS_DATA);
  }  
  //If there are events then we exicute the stuff below!
  else
  {
    //Repeat for the character lenght of the string GS_DATA
    for (int i = 0; i <= GS_DATA.length(); i++)
      {
      //If arr_count[0] is greater than or equal to 8
      // break the for loop, this ensures that only
      // 7 events will be stored in data arrays above
      if (arr_count[0] >= 8 ){break;}
      
      //Switch Case, if any character in GS_DATA matches
      //one of these cases, execute that case
      switch (GS_DATA[i])
        {
          //Case 1, if an '_' character is found in GS_DATA execute this case
          case '_': 
            //Send the array we are writing to & its arr_count specifc element number
            endTheString(arrSTmonth, 0);  
          break;
                  
          case '~': 
            endTheString(arrSTdate, 1);
          break;
                  
          case '^': 
            endTheString(arrSTtime, 2);
          break;
                  
          case '`':
            endTheString(arrSTyear, 3);
          break;
                  
          case '[': 
            endTheString(arrEDmonth, 4);
          break;
                  
          case ']':
            endTheString(arrEDdate, 5);
          break;
                  
          case '{':
            endTheString(arrEDtime, 6);
          break;
                  
          case '}':
            endTheString(arrEDyear, 7);
          break;
                  
          case '\n':
            endTheString(arrEventName, 8);
            ++ numOfEventsParsed;
          break;
                  
          default:
            //When none of the above cases are found, add the current character
            // of GS_DATA to the string called placeHolderString 
            placeHolderString += GS_DATA[i];
           break;
                  
         }//end of switch case
     }//end of for loop
    
  //Print our GS_DATA to the serial monitor
  Serial.println(GS_DATA);
      
  //Print all of the arrays with newly stored data from GS_DATA!
  printOnScreen(arrSTmonth);
  printOnScreen(arrSTdate);
  printOnScreen(arrSTtime);
  printOnScreen(arrSTyear);
  printOnScreen(arrEDmonth);
  printOnScreen(arrEDdate);
  printOnScreen(arrEDtime);
  printOnScreen(arrEDyear);
  printOnScreen(arrEventName);
        }//end of if else statement
}//end of ParseData
    
/****************************FUNCTION VOID END THE STRING********************************************
 * This function stream lines taking the sent array and placing the value in placeHolderString into  
 * one of the elements of the array sent to this function. 
 *****************************************************************************************************/
void endTheString(String arrBeingWrittenTo[], int specificArrayCountELEMENT)
{
  //GS_DATA data was stored in this string until it was sent to this function so we end the string's
  // value with \0 (which a null character symbolizing the end of a value string)
  placeHolderString += '\0';
        
  //Writes the value of placeHolderString to the array being written to
  //For example: arrBeingWrittenTo[arr_count[0]] on the first case, of the switch case
  //Means: The array arrSTmonth[] will have the placeHolderString value placed into its first 
  //       element 
  arrBeingWrittenTo[arr_count[specificArrayCountELEMENT]] = placeHolderString;
        
  placeHolderString = ""; //reset the current word
    
  //Increment the array count in for the specific array chosen
  //ie: following the first run of switch case '_' which uses arrSTmonth[]
  //    arr_count = 1,0,0,0,0,0,0,0,0
  //ie: On the first run through of switch case '~' which uses arrSTdate[]
  //    arr_count = 1,1,0,0,0,0,0,0,0
  arr_count[specificArrayCountELEMENT]++;
}//end of void endTheString
    
/****************************FUNCTION VOID PRINT ON SCREEN********************************************
 * This function simply takes an array's name and displays it with a number infront of it starting at 
 * 1 rather than 0. 
 *****************************************************************************************************/
void printOnScreen(String arrToDisplay[])
{
  Serial.println(); //New line for each array displayed to show separation
  int EventNum = 0; //Used for number displayed b4 the array
    
  //Loop that displays the sent array
  for (int j = 0; arrToDisplay[j] != ""; j++)
  {
    EventNum++; //Make it a 1 instead of 0
    //Output will be like "1: Apr" 
    Serial.print(EventNum);Serial.print(": ");Serial.println(arrToDisplay[j]);
  }//end of for loop
        
}//end of printOnScreen 
