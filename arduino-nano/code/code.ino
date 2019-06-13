//### ส่งข้อมูล ไปยัง Arduino wemos mini d1#############
#include <SoftwareSerial.h>
SoftwareSerial sensor(5, 6); // RX, TX
//SoftwareSerial Gps2(7, 8); // RX, TX
//##################################################

//############### Sensor config ####################
String g="";
int DO =A0;
int PH =A1;
int ORP =A2;
int EC =A3;
//int TMP =D4;
float lnt =17.321266;
float loa =104.108045;
//###################################################

//####################temp###########################
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//##################################################3

//################ The TinyGPS++ object #############
#include <TinyGPS++.h>
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
//##################################################3

void setup() {
  Serial.begin(57600);
  while (!Serial) {
    ; 
  }


  Serial.println("Run .....");
  sensors.begin();
  sensor.begin(19200);
  //Gps2.begin(38400);
  ss.begin(GPSBaud);
  
}

void loop() { // run over and over

                       //****************
                    int IDo=analogRead(A0);
                    int Do1=map(IDo,0,1023,1,14);
   String Do=String(Do1);
                    //****************
                    int Iph=analogRead(A1);
                    int pH=map(Iph,0,1023,1,14);
   String Ph=String(pH);
                     //****************
                   // int It=analogRead(A2);
                   // int Tmp=map(It,0,1023,1,14);
  String Temp=String(temp());
                     //****************
                    int ec=analogRead(A3);
                    int Ec=map(ec,0,1023,1,14);
  String eC=String(Ec);
                     //****************
                       //****************
                    int Orp=analogRead(A3);
                    int ORp=map(Orp,0,1023,1,14);
  String orp=String(ORp);
    String data;
    data +=Do;data +=",";
    data +=Ph;data +=",";
    data +=eC;data +=",";
    data +=orp;data +=",";
    data +=Temp;data +=",";
    data +=GPS();data +=",";
    
    g=sensor.readString();
   Serial.print("data : ");
   //Serial.println(GG);
    Serial.println(data);
    sensor.print(data);
//     delay(10);
//     String gpps =GPS();
//     Gps2.print(gpps); 
//     Serial.println(gpps);
}

 float temp()
{
  sensors.requestTemperatures(); 
 return sensors.getTempCByIndex(0);
}
String GPS(){
  String Location;
        gps.encode(ss.read());
        if (gps.location.isUpdated()){
        Serial.print("Latitude= ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(" Longitude= ");
        Serial.println(gps.location.lng(), 6);
          
           }
            Location =String("17.321266,104.108045");
       return Location;
       delay(10);
}
