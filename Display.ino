/*
*   Handles the display update
*
*  //  SCL                 --> A04 Tied HIGH W/ 10k Ohm resistor
*  //  SDA                 --> A05 Tied HIGH W/ 10k Ohm resistor  
*
*/
void UpdateDisplay()
{
  GetLine1();
  GetLine2();
  GetLine3();
  GetLine4();
  GetLine5();
  GetLine6();
}

void GetLine1()
{
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.setCursor(0,13);
    if (hours <=9) {u8g2.print("0");}   //see comments below in getline2()
    u8g2.print(hours); 
    u8g2.print(":"); 
    if (minutes <=9){u8g2.print("0");}
    u8g2.print(minutes); 
    u8g2.print(":"); 
    if (seconds <=9){u8g2.print("0");}
    u8g2.print(seconds); 
    u8g2.setCursor(64,13);              //start calendar in the middle
    if (days <=9){u8g2.print("0");}
    u8g2.print(days); 
    u8g2.print("/"); 
    if (months <=9){u8g2.print("0");}
    u8g2.print(months); 
    u8g2.print("/"); 
    u8g2.print(years); 
    u8g2.print(" ");  
}

void GetLine2()
{
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(0,24);
    u8g2.print("Time Date Stamp");
}

void GetLine3()
{
    u8g2.setCursor(0,34);
    u8g2.print("CAN ID = 0x");
    u8g2.print(CAN_MSG_TDS_ID,HEX);   
}

void GetLine4()
{
    u8g2.setCursor(0,44);
    u8g2.print("Temperature");
}

void GetLine5()
{
    u8g2.setCursor(0,54);
    u8g2.print("CAN ID = 0x");
    u8g2.print(CAN_MSG_ENV_ID,HEX); 
}

void GetLine6()
{
    // u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(0,64);
    u8g2.print("CAN kBaud Rate = ");
    u8g2.print("500"); //    u8g2.print(CAN_BAUDRATE); --> that was an ENUM --> 15
}
