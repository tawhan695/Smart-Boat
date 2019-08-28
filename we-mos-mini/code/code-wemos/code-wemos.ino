#define BLYNK_PRINT Serial
#include <Servo.h>


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
Servo esc; 
Servo myservo;  
int contro=0; 
int test=0;
int Se=D0;
int Status=0;
int pump=D3;
int lavel=0;
int DoC=D8;
SoftwareSerial chat(D5, D6); // RX, TX
//SoftwareSerial Gps(D7, D8); // RX, TX

WidgetMap myMap(V20);
WidgetLCD lcd(V50);
WidgetLCD lcd2(V51);
//int SensorDO=A0;
char ssid[] = "boat";
char pass[] = "258628701504";
////
//char ssid[] = "3bb-wlan";
//char pass[] = "T0655577688";

//char ssid[] = "Not";
//char pass[] = "12345678";

char auth[] = "a3af61a696ed4f49abaf0c569dd7feb9";

String a ="";
//17.321266, 104.108045
//float lat = 17.321266;
//float lon = 104.108045;

//Status
bool PH_sensor =false;
//=============================================================
void setup()
{
//     byte modeIndex = 3;
//    doCalibration(modeIndex);
   lcd.clear();
   lcd2.clear();
      digitalWrite(DoC,0);
pinMode(Se,OUTPUT);
pinMode(pump,OUTPUT);
pinMode(DoC,OUTPUT);
digitalWrite(pump,0); 
myservo.attach(D4);
esc.attach(D1);
esc.writeMicroseconds(1000);
delay(10000);
esc.writeMicroseconds(2000);
delay(2000);
esc.writeMicroseconds(0);
delay(1000);
  pinMode(D7, INPUT_PULLUP);
  Serial.begin(4800);
  Blynk.begin(auth, ssid, pass); 

  chat.begin(19200);
  // Gps.begin(38400);
   while (!Serial) {
    ; 
  }

   Blynk.virtualWrite(V17, 0);
   
}
void loop()
{
// DOSENSOR();
 //  Serial.println(Status);
  Blynk.run();
 // Map_GPS();
 //a = chat.readString();
  //Serial.println(digitalRead(D7));
 if((digitalRead(D7)==0)&&(digitalRead(Se)==1)){
Blynk.setProperty(V38, "color", "#ff0000");
Blynk.setProperty(V38, "label", "Please wait.");
 Blynk.virtualWrite(V31, "--");
    Blynk.virtualWrite(V34, "--");
     Blynk.virtualWrite(V33, "--");
     Blynk.virtualWrite(V32, "--");
     Blynk.virtualWrite(V30, "--");
      Blynk.virtualWrite(V31, "--");
 }else if((digitalRead(D7)==1)&&(digitalRead(Se)==1)){
   Blynk.setProperty(V38, "color", "#02d700");
Blynk.setProperty(V38, "label", "Sensor start.");
    a = chat.readString();
   //==============================================================
   String  Do =a.substring(0, a.indexOf(","));
  // Serial.print("DO : ");Serial.println(Do);
    Blynk.virtualWrite(V31, Do);
   //==============================================================
   String D1=a.substring(a.indexOf(",")+1,a.length());
   String  PH= D1.substring(0,D1.indexOf(","));
  // Serial.print("pH : ");Serial.println(PH);
   Blynk.virtualWrite(V30, PH);
   //==============================================================
   String D2=D1.substring(D1.indexOf(",")+1,D1.length());
   String  EC= D2.substring(0,D2.indexOf(","));
  // Serial.print("EC : ");Serial.println(EC);
   Blynk.virtualWrite(V32, EC);
   //==============================================================
   String D3=D2.substring(D2.indexOf(",")+1,D2.length());
   String  ORP= D3.substring(0,D3.indexOf(","));
  //  Serial.print("ORP : ");Serial.println(ORP);
     Blynk.virtualWrite(V33, ORP);
   //==============================================================
   String D4=D3.substring(D3.indexOf(",")+1,D3.length());
   String  TEMP= D4.substring(0,D4.indexOf(","));
  //  Serial.print("TEMP : ");Serial.println(TEMP);
     Blynk.virtualWrite(V34, TEMP);
   //==============================================================
   String D5=D4.substring(D4.indexOf(",")+1,D4.length());
   String  Lat= D5.substring(0,D5.indexOf(","));
   // Serial.print("lat : ");Serial.println(Lat);
     String D6=D5.substring(D5.indexOf(",")+1,D5.length());
   String  Lng= D6.substring(0,D6.indexOf(","));
  //  Serial.print("lng : ");Serial.println(Lng);
//    
     myMap.location(1, Lat, Lng, "ตำแหน่งเรือ");
   //==============================================================
 }else if(digitalRead(Se)==0){
    Blynk.virtualWrite(V31, "--");
    Blynk.virtualWrite(V34, "--");
     Blynk.virtualWrite(V33, "--");
     Blynk.virtualWrite(V32, "--");
     Blynk.virtualWrite(V30, "--");
      Blynk.virtualWrite(V31, "--");
     
  }
 Blynk.virtualWrite(V4, lavel);
 
  if(contro==2){
     myservo.write(20); 
   }else if(contro==1){
      myservo.write(98);
   }else{
      myservo.write(60);
    }
//digitalWrite(pump,0);
    
}
//BLYNK_CONNECTED() {
//    Blynk.syncAll();
//}
BLYNK_WRITE(V26)
{
  int pinData = param.asInt();
   contro=pinData;
}

BLYNK_WRITE(V38)
{
  int pinData = param.asInt();
   digitalWrite(Se,pinData);
   pinData=Status;
   
}
BLYNK_WRITE(V11)
{
  int pinData = param.asInt();
   
   if((pinData==1)&&(lavel ==0)){
    for(int i=0;i<=100;i++){
       Blynk.virtualWrite(V4, lavel);
       digitalWrite(pump,pinData);
       lavel+=1;
       delay(300);
    }
    if(lavel==100){
      Blynk.virtualWrite(V4, "value","0");
       digitalWrite(pump,0);
    }
    }
    else{
      digitalWrite(pump,0);
    }  
}
BLYNK_WRITE(V29)
{
  int pinData = param.asInt();
 esc.writeMicroseconds(pinData);

}

BLYNK_WRITE(V40)
{
  int pinData = param.asInt();
  if(pinData==2){
   digitalWrite(DoC,1);
     delay(1000);
    digitalWrite(DoC,0);
  lcd.clear();
  lcd.print(0,0,"ใส่เซ็นเซอร์ลงใน");
  lcd.print(0,1,"น้ำ ออกชิเจน 0 ");
  delay(20000);
  lcd.clear();
  lcd.print(0,0,"สำเร็จ");
  digitalWrite(DoC,0);
  }
  else if(pinData==1){
  digitalWrite(DoC,1);
  delay(1000);
   digitalWrite(DoC,0);
  lcd.clear();
  lcd.print(0,0,"ใส่เซ็นเซอร์ลงใน");
  lcd.print(0,1,"น้ำ ออกชิเจนอิ่มตัว");
  delay(20000);
    lcd.clear();
  lcd.print(0,0,"สำเร็จ");
  delay(2000);
  lcd.clear();
  lcd.print(0,0,"stap 2 ");
  delay(2000);
 
  }
  
}
