#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial chat(D5, D6); // RX, TX
//SoftwareSerial Gps(D7, D8); // RX, TX

WidgetMap myMap(V20);

char ssid[] = "Tawhan-Studio";
char pass[] = "T0655577688";
char auth[] = "1c30bc20e4ab4a80ac9d80eaa040c23e";

String a ="null";
//17.321266, 104.108045
//float lat = 17.321266;
//float lon = 104.108045;

//Status
bool PH_sensor =false;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass); 

  chat.begin(19200);
  // Gps.begin(38400);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

   Blynk.virtualWrite(V17, 0);
}

void loop()
{
  Blynk.run();
 // Map_GPS();
  Data_nano();
  
Blynk.setProperty(V9, "url", "http://192.168.1.2:86/line.html");
}

void Data_nano(){
     a = chat.readString();  //อ่าน Serial และนำไปเก็บในตัวแปร A
  if(a.length()>5)
  {
   //==============================================================
   String  Do =a.substring(0, a.indexOf(","));
   Serial.print("DO : ");Serial.println(Do);
    Blynk.virtualWrite(V31, Do);
   //==============================================================
   String D1=a.substring(a.indexOf(",")+1,a.length());
   String  PH= D1.substring(0,D1.indexOf(","));
   Serial.print("pH : ");Serial.println(PH);
   pH(PH);
   //==============================================================
   String D2=D1.substring(D1.indexOf(",")+1,D1.length());
   String  EC= D2.substring(0,D2.indexOf(","));
   Serial.print("EC : ");Serial.println(EC);
   Blynk.virtualWrite(V32, EC);
   //==============================================================
   String D3=D2.substring(D2.indexOf(",")+1,D2.length());
   String  ORP= D3.substring(0,D3.indexOf(","));
    Serial.print("ORP : ");Serial.println(ORP);
     Blynk.virtualWrite(V33, ORP);
   //==============================================================
   String D4=D3.substring(D3.indexOf(",")+1,D3.length());
   String  TEMP= D4.substring(0,D4.indexOf(","));
    Serial.print("TEMP : ");Serial.println(TEMP);
     Blynk.virtualWrite(V34, TEMP);
   //==============================================================
   String D5=D4.substring(D4.indexOf(",")+1,D4.length());
   String  Lat= D5.substring(0,D5.indexOf(","));
    Serial.print("lat : ");Serial.println(Lat);
     String D6=D5.substring(D5.indexOf(",")+1,D5.length());
   String  Lng= D6.substring(0,D6.indexOf(","));
    Serial.print("lng : ");Serial.println(Lng);
    
     myMap.location(1, Lat, Lng, "ตำแหน่งเรือ");
   //============================================================== 
  }
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
BLYNK_WRITE(V17)
{
  int pinData = param.asInt(); 
  chat.println(pinData);
  Serial.print("servo :");
  Serial.println(pinData);
}
