#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "VLHCeBQpa6nMihPhMrxdKmUON6dOZmsc";
//char ssid[] = "3bb-wlan";
//char pass[] = "T0655577688";

char ssid[] = "boat";
char pass[] = "258628701504";


int v1,v2,v3,v4,v5,v6;


const int in1Pin = D5;  // H-Bridge input pins

const int in2Pin = D6;


String v;
SoftwareSerial mega(D1, D2); // RX, TX


WidgetMap myMap(V10);
WidgetLCD lcd(V50);
WidgetLCD lcd2(V51);

void setup() {
  mega.begin(19200);
  Serial.begin(4800); // opens serial port, sets data rate to 9600 bps
  Blynk.begin(auth, ssid, pass);
  
  pinMode(in1Pin, OUTPUT);

  pinMode(in2Pin, OUTPUT);

        digitalWrite(in1Pin,1);
      digitalWrite(in2Pin,1);
}

void loop() {
  Blynk.run();
if (mega.available() > 0) {
 

 /*  data +=Do;data +=",";
    data +=Ph;data +=",";
    data +=eC;data +=",";
    data +=orp;data +=",";
    data +=Temp;data +=",";
    data +=displayInfo();data +=",";*/

    
   String data = mega.readString();
  
   String  DO =data.substring(0, data.indexOf(","));
   
 //  Serial.println("DO "+DO);
   String data2 =data.substring(data.indexOf(",")+1,data.length());
//    Serial.println("DATA "+data2);
   String  PH =data2.substring(0, data2.indexOf(","));
 //  Serial.println("PH "+PH);
   String data3 =data2.substring(data2.indexOf(",")+1,data2.length());
   String  ORP =data3.substring(0, data3.indexOf(","));
 //  Serial.println("ORP "+ORP);
   String data4 =data3.substring(data3.indexOf(",")+1,data3.length());
   String  TEMP =data4.substring(0, data4.indexOf(","));
 //  Serial.println("TEMP "+TEMP);
   String data5 =data4.substring(data4.indexOf(",")+1,data4.length());
   String  La =data5.substring(0, data5.indexOf(","));
 ///  Serial.println("lat "+La);
   String data6 =data5.substring(data5.indexOf(",")+1,data5.length());
   //float la =La.toFloat();
   String  Ln =data6.substring(0, data6.indexOf(","));
 //  Serial.println("lnt "+Ln);

//   Serial.println("lat "+La);

   Blynk.virtualWrite(V31, DO);
   Blynk.virtualWrite(V30, PH);
   Blynk.virtualWrite(V33, ORP);
   Blynk.virtualWrite(V34, TEMP);
  int index = 1;
  myMap.location(index, La, Ln, "ตำแหน่งเรือ");
  
}
}
BLYNK_WRITE(V1) 
{
  v1 = param.asInt();
  if(v1==1){
      digitalWrite(in1Pin,0);
      digitalWrite(in2Pin,0);
  }
  if(v1==0){
      digitalWrite(in1Pin,1);
      digitalWrite(in2Pin,1);
  }
   
}
BLYNK_WRITE(V3){
  v3 = param.asInt();
    if(v3==1){
      digitalWrite(in1Pin,1);
      digitalWrite(in2Pin,0);

  }
  if(v3==0){
     digitalWrite(in1Pin,1);
     digitalWrite(in2Pin,1);

  }
   
}
BLYNK_WRITE(V4){
  v4 = param.asInt();
    if(v4==1){
      digitalWrite(in1Pin,0);
      digitalWrite(in2Pin,1);

  }
  if(v4==0){
      digitalWrite(in1Pin,1);
      digitalWrite(in2Pin,1);

  }  
}

BLYNK_WRITE(V5){
  v5 = param.asInt();
    if(v5==1){
    mega.println("500");
  }
  if(v5==0){
  }  
}

BLYNK_WRITE(V6){
  v6 = param.asInt();
    if(v6==1){
    mega.println("600");
  }
  if(v6==0){
  }  
}

BLYNK_WRITE(V40)
{
  int pinData = param.asInt();
  if(pinData==2){
    mega.println("999");
     delay(1000);
   // digitalWrite(DoC,0);
  lcd.clear();
  lcd.print(0,0,"ใส่เซ็นเซอร์ลงใน");
  lcd.print(0,1,"น้ำ ออกชิเจน 0 ");
  delay(20000);
  lcd.clear();
  lcd.print(0,0,"สำเร็จ");
  //digitalWrite(DoC,0);
  }
  else if(pinData==1){
   mega.println("888");
  delay(1000);
   //digitalWrite(DoC,0);
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
