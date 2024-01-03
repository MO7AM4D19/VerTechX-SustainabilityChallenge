#include <Adafruit_GFX.h>
#include <Adafruit_ILI9486.h>
#include <TouchScreen.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h> 

#define SLAVE1_ADDRESS 8
#define SLAVE2_ADDRESS 9
#define SLAVE3_ADDRESS 10

int value1[3], value2[3], value3[3];

#define  Piezo_PIN          13
#define  Pump_PIN           11
#define  Extractor_PIN      12

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8
Adafruit_ILI9486 tft = Adafruit_ILI9486(TFT_CS, TFT_DC, TFT_RST);

#define TS_CS 4
#define TS_IRQ 3
TouchScreen ts = TouchScreen(TS_CS, TS_IRQ);

#define SD_CS 6

float temperature = 0;
float humidity = 0; 
int moistureValue = 0;

File imageFile;

void setup() 
{
  pinMode(Piezo_PIN,OUTPUT);
  pinMode(Pump_PIN,OUTPUT);
  pinMode(Extractor_PIN,OUTPUT);
  
  tft.begin();
  tft.setRotation(1);
  SD.begin(SD_CS);

  displayImage("image1.bmp");
  Serial.begin(9600);
}

void loop() 
{
  TSPoint point = ts.getPoint();
  if (point.z > 10) 
  {
    displayImage("image2.bmp");
    while (ts.getPoint().z > 10); 
  }
  TSPoint point = ts.getPoint();
  while(point.z < 10)
  {
    TSPoint point = ts.getPoint();
  }  
  
  int x = map(point.x, 0, 480, 0, tft.width());
  int y = map(point.y, 0, 320, 0, tft.height());
  if (point.z > 10) {
    if (point.x < 240/2)
    {
      displayImage("image3.bmp");
      /*Values of senssors should be print*/
      TSPoint point = ts.getPoint();
      while(point.z < 10)
      {
       TSPoint point = ts.getPoint();
      }
      int x = map(point.x, 0, 480, 0, tft.width());
      int y = map(point.y, 0, 320, 0, tft.height());
      if(point.y<80)
      {
        readValues(SLAVE1_ADDRESS, value1);
        
        if(point.x<120)
        {
          displayImage("image5.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value1[0]);
        }
        elseif((point.x>120)&&(point.x<240))
        {
          displayImage("image7.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value1[1]);
        }
        else
        {
          displayImage("image9.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value1[2]);         
        }
      }
      elseif((point.y>80)&&(point.y<160))
      {
        readValues(SLAVE2_ADDRESS, value2);
        
        if(point.x<120)
        {
          displayImage("image11.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value2[0]);
        }
        elseif((point.x>120)&&(point.x<240))
        {
          displayImage("image13.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value2[1]);
        }
        else
        {
          displayImage("image15.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value2[2]);          
        }
      }
      else
      {
        readValues(SLAVE3_ADDRESS, value3);
        if(point.x<120)
        {
          displayImage("image17.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value3[0]);
        }
        elseif((point.x>120)&&(point.x<240))
        {
          displayImage("image19.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value3[1]);
        }
        else
        {
          displayImage("image21.bmp");
          tft.setTextSize(3);
          tft.setTextColor(ILI9341_BLACK);
          tft.setCursor(230, 180);
          tft.print(value3[2]);
        }
      }
    }
     
    else 
    {
      /**Control Piezo , Pump , extractor**/
      displayImage("image4.bmp");
      TSPoint point = ts.getPoint();
      while(point.z < 10)
      {
       TSPoint point = ts.getPoint();
      }
      int x = map(point.x, 0, 480, 0, tft.width());
      int y = map(point.y, 0, 320, 0, tft.height());
      if(point.x<160)
      {
        /*Control Piezo*/
        displayImage("image6.bmp");      /*ON OR OFF*/
        TSPoint point = ts.getPoint();
        while(point.z < 10)
        {
          TSPoint point = ts.getPoint();
        }
        int x = map(point.x, 0, 480, 0, tft.width());
        int y = map(point.y, 0, 320, 0, tft.height());
        if(point.x<240)
        {
          /*Piezo OFF*/
          digitalWrite(Piezo_PIN,LOW);
          displayImage("image8.bmp");  
        }
        else
        {
          /*Piezo ON*/
          digitalWrite(Piezo_PIN,HIGH);
          displayImage("image10.bmp");  
        }
      }
      elseif((point.x<160)&&(point.x<320))
      {
        /*Control PUMP*/
        displayImage("image12.bmp");     /*ON OR OFF*/
        TSPoint point = ts.getPoint();
        while(point.z < 10)
        {
          TSPoint point = ts.getPoint();
        }
        int x = map(point.x, 0, 480, 0, tft.width());
        int y = map(point.y, 0, 320, 0, tft.height());
        if(point.x<240)
        {
          /*PIMP OFF*/
          digitalWrite(Pump_PIN,LOW);
          displayImage("image14.bmp");
        }
        else
        {
          /*PUMP ON*/
          digitalWrite(Pump_PIN,HIGH);
          displayImage("image16.bmp");
        }        
      }
      else
      {
        /*Control Extractor*/
        displayImage("image18.bmp");      /*ON OR OFF*/  
        TSPoint point = ts.getPoint();
        while(point.z < 10)
        {
          TSPoint point = ts.getPoint();
        }
        int x = map(point.x, 0, 480, 0, tft.width());
        int y = map(point.y, 0, 320, 0, tft.height());
        if(point.x<240)
        {
          /*Extractor OFF*/
          digitalWrite(Extractor_PIN,LOW);
          displayImage("image20.bmp");
        }
        else
        {
          /*Extractor ON*/
          digitalWrite(Extractor_PIN,HIGH);
          displayImage("image22.bmp");
        }      
      }
    }
    while (ts.getPoint().z > 10); 
  }
}




void displayImage(const char* filename) {
  imageFile = SD.open(filename);
  if (imageFile) {
    uint32_t seekOffset = 0;
    seekOffset += 10; 
    seekOffset += 4; 
    uint32_t height = imageFile.read(); 
    height |= imageFile.read() << 8;
    height |= imageFile.read() << 16;
    height |= imageFile.read() << 24;

    seekOffset += 4; // skip image height
    seekOffset += 2; // skip image planes
    uint32_t bitsPerPixel = imageFile.read(); 
    bitsPerPixel |= imageFile.read() << 8;

    seekOffset += 4; 
    seekOffset += 4; 
    seekOffset += 4; 
    seekOffset += 4; 
    seekOffset += 4; 
    seekOffset += 4; 

    uint32_t rowSize = (bitsPerPixel * 240 + 31) / 32 * 4;
    uint8_t rowBuffer[rowSize];
    uint16_t colorBuffer[240];
    uint32_t pixelOffset = 0;

    for (uint32_t y = 0; y < height; y++) {
      imageFile.seek(seekOffset + (height - y - 1) * rowSize);
      imageFile.read(rowBuffer, rowSize);
      for (uint32_t x = 0; x < 240; x++); x++) {
        uint32_t bitOffset = x * bitsPerPixel;
        uint32_t byteOffset = bitOffset / 8;
        uint8_t byteValue = rowBuffer[byteOffset];
        uint8_t bitMask = 0x80 >> (bitOffset % 8);
        uint8_t pixelValue = (byteValue & bitMask) ? 0xFFFF : 0x0000;
        colorBuffer[x] = pixelValue;
      }
      tft.drawRGBBitmap(0, y, colorBuffer, 240, 1);
    }

    imageFile.close();
  }
}


void readValues(byte address, int* values) {
  Wire.beginTransmission(address);
  Wire.requestFrom(address, sizeof(int) * 3);
  if (Wire.available() == sizeof(int) * 3) {
    values[0] = Wire.read();
    values[0] |= Wire.read() << 8;
    values[0] |= Wire.read() << 16;
    values[0] |= Wire.read() << 24;
    values[1] = Wire.read();
    values[1] |= Wire.read() << 8;
    values[1] |= Wire.read() << 16;
    values[1] |= Wire.read() << 24;
    values[2] = Wire.read();
    values[2] |= Wire.read() << 8;
    values[2] |= Wire.read() << 16;
    values[2] |= Wire.read() << 24;
  }
  Wire.endTransmission();
}