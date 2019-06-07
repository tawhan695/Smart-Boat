//nano
#include <SoftwareSerial.h>

SoftwareSerial sensor(5, 6); // RX, TX
String g="";
int DO =A0;
int PH =A1;
int TMP =A2;
int EC =A3;
int ORP =A4;
float lnt =17.321266;
float loa =104.108045;

void setup() {
  Serial.begin(57600);
  while (!Serial) {
    ; 
  }


  Serial.println("Goodnight moon!");
  sensor.begin(19200);
 // Gps.begin(19200);
  
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
                    int It=analogRead(A2);
                    int Tmp=map(It,0,1023,1,14);
  String Temp=String(Tmp);
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
    data +=Temp;data +=",";
    data +=eC;data +=",";
    data +=orp;data +=",";
    
    g=sensor.readString();
   Serial.print("data : ");
   //Serial.println(GG);
    Serial.println(data);
    sensor.print(data);
     delay(100);

 
}
