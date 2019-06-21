//### ส่งข้อมูล ไปยัง Arduino wemos mini d1#############
#include <SoftwareSerial.h>
SoftwareSerial sensor(5, 6); // RX, TX
//SoftwareSerial Gps2(7, 8); // RX, TX
//##################################################

//############### Sensor config ####################
String g="";
int DO =A0;
int PH =A1;
//int ORP =A2;
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

//############### ORP Sensor ####################
#define VOLTAGE 5.00 //system voltage
#define OFFSET 0 //zero drift voltage
//#define LED 13 //operating instructions
double orpValue;
#define ArrayLenth 40 //times of collection
#define orpPin A2 //orp meter output,connect to Arduino controller ADC pin
int orpArray[ArrayLenth];
int orpArrayIndex=0;

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
                    int Do1=map(IDo,0,1023,1,100);
                    String Do=String(Do1);
                    //****************
                    int Iph=analogRead(A1);
                    int pH=map(Iph,0,1023,1,14);
                     String Ph=String(pH);
                     //****************
                    String Temp=String(temp());
                     //****************
                    int ec=analogRead(A3);
                    int Ec=map(ec,0,1023,1,20);//Measuring range: 1ms/cm--20ms/cm
                    String eC=String(Ec);
                     //****************
                
                    String orp=String(ORP_1());
                     //****************

                     
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

//############### ORP Sensor ####################
double avergearray(int* arr, int number){
int i;
int max,min;
double avg;
long amount=0;
if(number<=0){
printf("Error number for the array to avraging!/n");
return 0;
}
if(number<5){ //less than 5, calculated directly statistics
for(i=0;i<number;i++){
amount+=arr[i];
}
avg = amount/number;
return avg;
}else{
if(arr[0]<arr[1]){
min = arr[0];max=arr[1];
}
else{
min=arr[1];max=arr[0];
}
for(i=2;i<number;i++){
if(arr[i]<min){
amount+=min; //arr<min
min=arr[i];
}else {
if(arr[i]>max){
amount+=max; //arr>max
max=arr[i];
}else{
amount+=arr[i]; //min<=arr<=max
}
}//if
}//for
avg = (double)amount/(number-2);
}//if
return avg;
}

int ORP_1() {
static unsigned long orpTimer=millis(); //analog sampling interval
static unsigned long printTime=millis();
if(millis() >= orpTimer)
{
orpTimer=millis()+20;
orpArray[orpArrayIndex++]=analogRead(orpPin); //read an analog value every 20ms
if (orpArrayIndex==ArrayLenth) {
orpArrayIndex=0;
}
orpValue=((30*(double)VOLTAGE*1000)-(75*avergearray(orpArray, ArrayLenth)*VOLTAGE*1000/1024))/75-OFFSET;

//convert the analog value to orp according the circuit
}
if(millis() >= printTime) //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
{
printTime=millis()+800;
//Serial.println((int)orpValue);
return ((int)orpValue);
}
}
