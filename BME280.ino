/*
*   Handles the BME
*
*   
*
*/


//setup for and adveraging method for input did not see great improvement over direct method
#define RH_SAMPLES 5
#define PSI_SAMPLES 5
#define TEMP_SAMPLES 5
#define DEWPT_SAMPLES 5


float BME280_Humidity;
float BME280_Pressure;
float BME280_Temperature;
float BME280_Altitude;
float BME280_DewPoint;

unsigned char CAN_Temperature;

int Humidity[RH_SAMPLES];
int Pressure[PSI_SAMPLES];
int Temperature[TEMP_SAMPLES];
int DewPoint[DEWPT_SAMPLES];

void SetupBME280()
{
  bme280.setI2CAddress(0x76); //Connect to a second sensor
  if (bme280.beginI2C() == false) 
    {
      Serial.println("BME280 Sensor connect failed");
    }

    // unsigned status;
    
//     // default settings
//     // (you can also pass in a Wire library object like &Wire2)
//     status = bme280.begin();  
//     if (!status) 
//     {
//         Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
//         Serial.print("SensorID was: 0x"); Serial.println(bme280.sensorID(),16);
//         Serial.print("   ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
//         Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
//         Serial.print("   ID of 0x60 represents a BME 280.\n");
//         Serial.print("   ID of 0x61 represents a BME 680.\n");
//         while (1);
//     }
  if (bme280.beginI2C() == true) 
  {
    Serial.println("BME280 has begun...");
  }  
}

void UpdateBME280 ()
{
   UpdateHumidity();
   UpdatePressure();
   UpdateTemperature();
   UpdateDewPoint();
}

void UpdateHumidity()
{
  uint8_t i;
  //float average; //--> changed to TempSensorAverage above
 
  // take N samples in a row, with a slight delay
  for (i=0; i< RH_SAMPLES; i++) 
  {
     Humidity[i] = bme280.readFloatHumidity();
     delay(10);
  }
  // average all the samples out - you could leave this one in and add + 1 to the division
  // BME280_Humidity = 0;
   
  for (i=0; i< RH_SAMPLES; i++) 
  {
     BME280_Humidity += Humidity[i];
  }
  
  BME280_Humidity /= (RH_SAMPLES + 1);

  //the older non-averaging method
  //BME280_Humidity = bme280.readFloatHumidity();
}

void UpdatePressure()
{
  BME280_Pressure = (bme280.readFloatPressure()/1000);
}

void UpdateTemperature()
{
  BME280_Temperature = bme280.readTempC();

  CAN_Temperature = (BME280_Temperature + 40); //this gives and offset of 40 for -40*c to equal 0
  Serial.print("Temp in *c ");
  Serial.println(CAN_Temperature);
  if (CAN_Temperature >= 254)
  {
    CAN_Temperature = 255; //this forces the last good value to 255
  }

  if (CAN_Temperature <= 0)
  {
    CAN_Temperature = 0; //this forces the lowest good value to 0
  }
}

void UpdateDewPoint()
{
   BME280_DewPoint = bme280.dewPointF();
}

void UpdateSerialBME280 ()
{
    BME280_Humidity = bme280.readFloatHumidity();
    Serial.print("Humidity = ");
    Serial.print(BME280_Humidity);
    Serial.println(" %");

    BME280_Pressure = (bme280.readFloatPressure()/1000);
    Serial.print("Pressure = ");
    Serial.print(BME280_Pressure);
    Serial.println(" kPa");

    BME280_Temperature = bme280.readTempC();
    Serial.print("Temperature = ");
    Serial.print(BME280_Temperature);
    Serial.println(" *C");
    Serial.print("Temperature = ");
    Serial.print((BME280_Temperature * 9 / 5) + 32);
    Serial.println(" *f");
    
    
    BME280_DewPoint = (bme280.dewPointF(),2);
    Serial.print(" Dewpoint: ");
    Serial.print(bme280.dewPointC(), 2);
    Serial.println(" *C");
    Serial.print(" Dewpoint: ");
    Serial.print(BME280_DewPoint, 2);
    Serial.println(" *f");

    BME280_Altitude = bme280.readFloatAltitudeMeters();
    Serial.print("Approx. Altitude = ");
    Serial.print(BME280_Altitude);
    Serial.println(" m");
//
  Serial.println();
}

void DebugBMEData()
{
  Serial.print("Humidity: ");
  Serial.print(bme280.readFloatHumidity(), 0);

  Serial.print(" Pressure: ");
  Serial.print(bme280.readFloatPressure(), 0);
  
  Serial.print(" Alt: ");
  //Serial.print(bme280.readFloatAltitudeMeters(), 1);
  Serial.print(bme280.readFloatAltitudeFeet(), 1);
  
  Serial.print(" Temp: ");
  //Serial.print(bme280.readTempC(), 2);
  Serial.print(bme280.readTempF(), 2);

  Serial.print(" Dewpoint: ");
  //Serial.print(bme280.dewPointC(), 2);
  Serial.print(bme280.dewPointF(), 2);

  Serial.println();

}
