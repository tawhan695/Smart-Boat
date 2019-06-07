
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial chat(D5, D6); // RX, TX

char ssid[] = "Tawhan-Studio";
char pass[] = "T0655577688";
char auth[] = "1c30bc20e4ab4a80ac9d80eaa040c23e";

String a ="null";
//17.321266, 104.108045
float lat = 17.321266;
float lon = 104.108045;

//Status
bool PH_sensor =false;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass); 

  chat.begin(19200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop()
{
  Blynk.run();
  Map_GPS((lat),lon);
  Data_nano();
  
Blynk.setProperty(V9, "url", "http://192.168.1.2:86/line.html");
}

void Data_nano(){
     a = chat.readString();  //อ่าน Serial และนำไปเก็บในตัวแปร A
   Serial.print("Data : "); 
   Serial.println(a); 
   pH("7");
 chat.write("Do:10:ph:13:ec:50:tm:90");
}
WidgetMap myMap(V20);
void Map_GPS(float lat,float lon){
  
  //MAP
myMap.location(1, lat, lon, "ตำแหน่งเรือ");
}
void pH(String ph){
  /*ค่า pH
   คือค่าความเป็นเกรดเป็นเบส
   โดยค่า pH มีอยู่ 1-14 
   โดยค่ากลางคือ 7 
   หากค่าที่วัดได้ต่ำกว่า 7 คือเกรด สูงกว่า 7 คือเบส
  */
  int pH=ph.toInt();
  Blynk.virtualWrite(V30, pH);
  if(pH==14){
    Blynk.setProperty(V30, "color", "#92278F");
  }else if(pH==13){
     Blynk.setProperty(V30, "color", "#662E93");
  }
  else if(pH==12){
    Blynk.setProperty(V30, "color", "#272264");
  }
  else if(pH==11){
     
     Blynk.setProperty(V30, "color", "#283890");
  }
  else if(pH==10){
     Blynk.setProperty(V30, "color", "#0E76BD");
    
  }
  else if(pH==9){
    
     Blynk.setProperty(V30, "color", "#25AAE3");
  }
  else if(pH==8){
    
      Blynk.setProperty(V30, "color", "#3AB54A");
  }
  else if(pH==7){
     Blynk.setProperty(V30, "color", "#8DC73F");
  }
  else if(pH==6){
      Blynk.setProperty(V30, "color", "#F3DA35");
  }
  else if(pH==5){
    
     Blynk.setProperty(V30, "color", "#FCB040");
  }
  else if(pH==4){

     Blynk.setProperty(V30, "color", "#F7941D");
  }
  else if(pH==3){
      Blynk.setProperty(V30, "color", "#F03F37");
    
  }
  else if(pH==2){
     Blynk.setProperty(V30, "color", "#F03F37");
    
  }
  else if(pH==1){
     Blynk.setProperty(V30, "color", "#BF1E2E");
    
  }
}
