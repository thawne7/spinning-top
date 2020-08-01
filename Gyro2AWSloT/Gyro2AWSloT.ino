#include "sea_esp32_qspi.h"
#include "SimpleDHT.h"
#include "string.h"

#include "AWS_IOT.h"
#include "WiFi.h"

// include the library:
#include <spartan-edge-esp32-boot.h>
#include "ESP32IniFile.h"

AWS_IOT hornbill;

#define Limit 500
#define Last_t 10000

char WIFI_SSID[]="UKRAINE";
char WIFI_PASSWORD[]="wxb8217121wkn";
char HOST_ADDRESS[]="aalsfearl6d4j-ats.iot.us-east-1.amazonaws.com";
char CLIENT_ID[]= "MyTestClient";
char TOPIC_NAME[]= "$aws/things/myTestThing/shadow/update";


int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0,tick1=0;
char payload[512];
char rcvdPayload[512];
uint8_t data1[4] = {41,42,43,44};
uint8_t data2[32];
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

// initialize the spartan_edge_esp32_boot library
spartan_edge_esp32_boot esp32Cla;

const size_t bufferLen = 80;
char buffer[bufferLen];
char buffer1[bufferLen];
// the setup routine runs once when you press reset:

void setup()
{
  // initialization 
  esp32Cla.begin();

  // check the .ini file exist or not
  const char *filename = "/board_config.ini";
  IniFile ini(filename);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(filename);
    Serial.println(" does not exist");
    return;
  }
  Serial.println("Ini file exists");

  // check the .ini file valid or not
  if (!ini.validate(buffer, bufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    return;
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Overlay_List_Info", "Overlay_Dir", buffer, bufferLen)) {
    Serial.print("section 'Overlay_List_Info' has an entry 'Overlay_Dir' with value ");
    Serial.println(buffer);
  }
  else {
    Serial.print("Could not read 'Overlay_List_Info' from section 'Overlay_Dir', error was ");
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Board_Setup", "overlay_on_boot", buffer1, bufferLen)) {
    Serial.print("section 'Board_Setup' has an entry 'overlay_on_boot' with value ");
    Serial.println(buffer1);
  }
  else {
    Serial.print("Could not read 'Board_Setup' from section 'overlay_on_boot', error was ");
  }

  // Splicing characters
  strcat(buffer,buffer1);
  
  // XFPGA pin Initialize
  esp32Cla.xfpgaGPIOInit();

  // loading the bitstream
  esp32Cla.xlibsSstream(buffer);

  
  Serial.begin(115200);
  SeaTrans.begin();
  //SeaTrans.write(0, data1, 4);
  //SeaTrans.read(0, data2, 4);
  //Serial.printf("%d %d %d %d\r\n",data2[0],data2[1],data2[2],data2[3]);
  delay(2000);

    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // wait 5 seconds for connection:
        delay(5000);
    }

    Serial.println("Connected to wifi");

    if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==hornbill.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfully");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }

    delay(2000);
  
}

int16_t read(int ad)
{
    uint8_t a[2];
    int16_t *b;
    SeaTrans.read(ad,a,2);
    b = (int16_t*)a;
    return *b;
}

int spin()
{
    int16_t temp = read(0);
    if(temp - 120 > Limit)
      return 1;
    else if (temp + 120 < -Limit)
      return -1;
    else
      return 0;
}

void loop()
{
    uint8_t i;
    int count = 0;
    int pre = 0;
    int cur = 0;
    Serial.print("\r\n");
    Serial.println("请开始摇动");
    unsigned long start_t = millis();
    while(millis() - start_t < Last_t)
    {
        cur = spin();
        if(((cur == 1)||(cur == -1))&&(cur!=pre))
          count++;
        pre = cur;
    }
    
    //SeaTrans.read(0, data2, 16);
    
    sprintf(payload,"在%d秒内摇动次数为%d",Last_t/1000,count/2);
    Serial.println(payload);
    if(hornbill.publish(TOPIC_NAME,payload) == 0)
    {        
      Serial.println("Publish successfully!");
      //Serial.println(payload);
    }
    else
    {
      Serial.println("Publish failed!");
    }
    msgReceived = 0;
    vTaskDelay(1000 / portTICK_RATE_MS); 
}
