/*
 * UpdateTime - Reads the RTC and set multiple varibles for use in other parts of the code
 *  pinout +5V-SDA-SCL-N/C-GND I think the N/C is an alarm pin...?
 *  //  SCL                 --> A04 Tied HIGH W/ 10k Ohm resistor
 *  //  SDA                 --> A05 Tied HIGH W/ 10k Ohm resistor  
 * 
 * DAP - 09-17-2021 Initial rev
 * DAP - 09-28-2021 added more vars 
 * DAP - 11-11-2022 updated setupRTC() 
 * DAP - 11-11-2022 Added DST SW and SunRise/SunSet
 * DAP - 03-14-2023 Added RTC_UPDATE_PIN
 */

void SetupRTC()
{
  //initialize RTC
  rtc.begin();

  //if RTC is not running OR RTC_UPDATE_PIN is pulled LOW try to set it 
  if (! rtc.isrunning() || digitalRead(RTC_UPDATE_PIN) == LOW) 
  {
    char t[32];
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
    DateTime now = rtc.now();
    sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
    Serial.print(F("Date/Time: "));
    Serial.println(t);
  }

  if (rtc.isrunning())  //if RTC is running, send out time
  {
    Serial.println("RTC has begun");
    UpdateRTC();
    //GetSunriseSunset();  //removed 
  }
  
}

void UpdateRTC()
{
    DateTime now = rtc.now();
    char buf[100];          //buffer for the RTC TimeDate Stamp
    strncpy(buf,"DD.MM.YYYY  hh:mm:ss\0",100);
    Serial.println(now.format(buf));
    delay(1000);
}

void UpdateTime()
{
  DateTime now = rtc.now();
//  strncpy(TimeBuffer,"DD.MM.YY  hh:mm:ss\0",100); //this formats the TimeBuffer array
//  TimeReadout = (now.format(TimeBuffer));
//  Serial.println(strncpy(TimeReadout, TimeBuffer, 20);
  //Serial.println(now.format(TimeBuffer));
      
    seconds = now.second();
    minutes = now.minute();
    hours   = now.hour();
    days    = now.day();
    months  = now.month();
    years   = (now.year()-2000);  //remove the first 2 digits
    
    //     char buf[100];          //buffer for the RTC TimeDate Stamp
    // strncpy(buf,"DD.MM.YYYY  hh:mm:ss\0",100);
    // Serial.println(now.format(buf));
    if (DSTSwitch_mode == 0)
    {
      hours -= 1;
      utc_offset = -5;
      Serial.println(hours);
      Serial.println(utc_offset);
    }
    else if (DSTSwitch_mode == 1)
    {
      utc_offset = -4;
      Serial.print("UTC_Offset --> ");
      Serial.println(utc_offset);
    }
    
}  // end void UpdateTime()