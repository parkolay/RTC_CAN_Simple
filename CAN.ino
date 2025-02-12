/*
*   Handles the CAN data Tx using MCP2515
*
*  //  SCK                 --> D13
*  //  MiSO                --> D12  
*  //  MOSI                --> D11 
*  //  SS                  --> D10   
*
*/

void SetupCAN()
{
  // if (!CAN.begin(CAN_BAUDRATE)) 
  // {
  //   Serial.println("Error initializing MCP2515.");
  //   while(1) delay(10);
  // }
  Serial.println("MCP2515 chip found");
   while( CAN.begin(CAN_BAUDRATE) )  // init can bus : baudrate = CAN_BAUDRATE defined at top of file
   {   
        Serial.println("MCP2515 init failed");
        Serial.println("Please try again");
        //delay(3000);
    }
    Serial.println("MCP2515 init ok!\n");
}

void Send_CAN_TDS()
{
  // if (Protocol_State == true) //set CAN ID based on Switch state
  // {
  //   CAN_MSG_ID = 0x1f4010f2;
  // }
  // else 
  // {
  //   CAN_MSG_ID = 0x18DA10f2;
  // }

 
  Serial.println("Sending extended packet ... ");
   unsigned char TimeData[8] = {seconds, minutes, hours, days, months, years, 0x00, 0x00};
   //uint8_t DFRobot_MCP2515::sendMsgBuf(uint32_t id, uint8_t ext, uint8_t len, uint8_t *buf) --> where ext is flagg for extended message ID
   CAN.sendMsgBuf(CAN_MSG_TDS_ID, true, 8, TimeData);
   Serial.println(seconds);
//  mcp.beginExtendedPacket(CAN_MSG_ID);//  mcp.beginExtendedPacket(0x18DA10f2); //T2 MSG ID
  // mcp.write(seconds);
  // mcp.write(minutes);
  // mcp.write(hours);
  // mcp.write(days);
  // mcp.write(months);
  // mcp.write(years);
  // mcp.write(0x00);
  // mcp.write(0x00);
  // mcp.endPacket();
  Serial.println("RTC Data Sent ");
}

void Send_CAN_Temp()
{
  Serial.println("Sending extended packet ... ");
   unsigned char CANData[8] = {CAN_Temperature, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
   CAN.sendMsgBuf(CAN_MSG_ENV_ID, true, 8, CANData);
   Serial.println(CAN_Temperature);
  Serial.println("Temp Data Sent ");
}
