/*  This sketch sets up a small OLED display (SSD1306), BME280 (RH && Temp && Press) and an RTC (DS3231) with CAN output (MCP2515). 
 * 
 *  Requirements:
 *
 *  * Nano or other Arduino board
 *  * OLED --> ssd1306
 *  * RTC  --> DS3231
 *  * CAN  --> MP2515
 *  * Arduino 1.8.8+ IDE
 *  * Library:  U8g2lib.h --> u8g2 by oliver was used
 *              RTCLib_by_NeiroN
 *              DFRobot_MCP2515
 *              SparkFun_BME280_Arduino_Library
 *
 *  // Pinout
 *
 *  //  MQ135 analog Output --> A00
 *  //  DST Interrupt       --> D02 with INT0
 *  //  Data Tx             --> TX_PIN 7
 *  //  Data Rx             --> RX_PIN 8
 *  //  RTC Update pin      --> D09
 *  //  SCL                 --> A04 Tied HIGH W/ 3k3 Ohm resistor
 *  //  SDA                 --> A05 Tied HIGH W/ 3k3 Ohm resistor
 *  //  SCK                 --> D13
 *  //  MiSO                --> D12  
 *  //  MOSI                --> D11 
 *  //  SS                  --> D10
 *   
 *   Created: June 26, 2024 by Dan Parkolay 
*/

#include "Arduino.h"
#include "Wire.h"
#include "U8g2lib.h"
#include "DFRobot_MCP2515.h"
#include "RTClib.h"
#include "SparkFunBME280.h"   //modified library defines or the following --> MODE_SLEEP, MODE_NORMAL, MODE_FORCED

#define switched                            true // value if the button switch has been pressed
#define triggered                           true // controls interrupt handler
#define interrupt_trigger_type            RISING // interrupt triggered on a RISING input
#define debounce                              10 // time to wait in milli secs

// Set CAN bus baud rate
#define CAN_BAUDRATE (CAN_500KBPS)        //the CAN PCB used has 8Mhz crystal and it is believed the library is configured for 16Mhz

unsigned long CAN_MSG_TDS_ID = 0x18FEE616;  //0x18DA10f2;
unsigned long CAN_MSG_ENV_ID = 0x18FEE716;  //0x18DA10f2;

const long VERSION = 8875;     // stardate from google calendar can be more than 65535...
const int RTC_UPDATE_PIN  = 9;  // internal pull up that can be sent to ground to set RTC to PC during flash
const int DSTSwitchPin    = 2;  // the number of the pushbutton pin
const int CAN_CS          = SS;//10;
int DSTSwitch_mode        = 1;
int utc_offset = -4;  //this accounts for DST and updated with DST switch

unsigned char seconds = 0;  // varible to store the seconds from clock
unsigned char minutes = 0;  // varible to store the minutes from clock
unsigned char hours   = 0;  // varible to store the hours from clock
unsigned char days    = 0;  // varible to store the seconds from clock
unsigned char months  = 0;  // varible to store the minutes from clock
unsigned char years   = 0;  // varible to store the hours from clock

//int DelayPeriod = 1000;           //setup for using millis() instead of delay()
//unsigned long Current_Time = 0; //the "other part" of millis() (roll over around 50 days which might not work in this application)

volatile  bool interrupt_process_status = 
{
  !triggered                                     // start with no switch press pending, ie false (!triggered)
};
bool initialisation_complete =            false; // inhibit any interrupts until initialisation is complete

//Initialize SSD1306 Display
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //U8G2_R0 was 13

//Initialize RTC
DS3231 rtc;

//Initialize BME280
BME280 bme280;

//Initialize MCP2515 CAN device
//Adafruit_MCP2515 mcp(CAN_CS); //was set to SS which should be the same thing
DFRobot_MCP2515 CAN(CAN_CS);

void setup(void) 
{
  pinMode(RTC_UPDATE_PIN, INPUT_PULLUP);    //set pin D09 as input with pullup to allow for RTC update during flash

  Wire.begin();
  Serial.begin(57600);
  Serial.println("initializing RTC CAN...");
  Serial.println("Version ");
  Serial.println(VERSION);

  // SetupRTC();
  //  Serial.println("initializing RTC DS3231...");

  //  SetupBME280();

  // u8g2.begin();
  //  Serial.println("initializing OLED SSD1306...");
   
  SetupCAN();
  Serial.println("initializing CAN MCP2515...");

  // // connect AREF to 3.3V and use that as VCC, less noisy!  
  // analogReference(EXTERNAL);  

  // attachInterrupt(digitalPinToInterrupt(DSTSwitchPin),
  //                 button_interrupt_handler,
  //                 interrupt_trigger_type);
  // initialisation_complete = true; // open interrupt processing for business

}

void loop(void) 
{
    
  // // test buton switch and process if pressed
  // if (read_button() == switched) 
  // {
  //   // button on/off cycle now complete, so flip LED between HIGH and LOW
  //   // Check DSTSwitchPin to see if it's pressed.
  //   if (DSTSwitch_mode == 1) 
  //   {
  //     // Button was up before, but is pressed now. Set the button to pressed
  //     // and report that to the serial monitor.
  //     DSTSwitch_mode = 0;  // Button is pressed.
  //       Serial.println("Button has been pressed.");
  //   }
 
  //   else if (DSTSwitch_mode == 0) 
  //   {
  //     // Button was down before, but is released now. Set the button to
  //     // released and report that to the serial monitor.
  //     DSTSwitch_mode = 1;  // Button is released.
  //     Serial.println("Button has been released");
  //   }
  // } 
  // else 
  // {

  // int CanSeconds = seconds; //set CanSeconds to seconds
  // // UpdateTime();     //Get time date data
  // // UpdateBME280();   //get BME data
  // Serial.println(seconds);
  // if (CanSeconds != seconds)  //only Tx once per second
  // {
    Send_CAN_TDS();
    Send_CAN_Temp();
  // }

  // delay(100);       
  // u8g2.firstPage();
  // do 
  // {

  //   UpdateDisplay();
  // } 
  // while ( u8g2.nextPage() );
  // //delay(1000);
  // }
}
