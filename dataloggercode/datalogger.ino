/* Arduino temperature logger with SD card, DS18B20 sensor and DS3231 (DS1307) RTC.
 * Time, date and temperature are displayed on 20x4 LCD.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 */
 
#include <SD.h>               // include Arduino SD library
#include <RTClib.h>           // include Adafruit RTC library

#include <OneWire.h>                 //Se importan las librerías
#include <DallasTemperature.h>


#define onewirePin  2                      //Se declara el pin donde se conectará la DATA

//onewire setup 
OneWire ourWire(onewirePin);                //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); 

DeviceAddress address[] = {
  {0x28, 0xFF, 0x8C, 0x24, 0x20, 0x17, 0x03, 0x28},
  {0x28, 0xDE, 0x86, 0x2C, 0x06, 0x00, 0x00, 0x2D},
  {0x28, 0xDD, 0x03, 0x2C, 0x06, 0x00, 0x00, 0x6D},
  {0x28, 0x6B, 0xE3, 0x2A, 0x06, 0x00, 0x00, 0x0B},
  {0x28, 0x86, 0x67, 0x2C, 0x06, 0x00, 0x00, 0xB5},
  {0x28, 0x82, 0xB1, 0x2B, 0x06, 0x00, 0x00, 0x2D},
  {0x28, 0x73, 0xA0, 0x2C, 0x06, 0x00, 0x00, 0x5F},
  {0x28, 0xAD, 0x76, 0x2A, 0x06, 0x00, 0x00, 0x43},
  {0x28, 0xEA, 0xEF, 0x2A, 0x06, 0x00, 0x00, 0xF7},
  {0x28, 0x73, 0x3B, 0x2B, 0x06, 0x00, 0x00, 0x1E}
};


 
// inicializando RTC DS3231
RTC_DS3231 rtc;
DateTime   now;

const int chipSelect = 10;  //cs de la terjeta SD
File dataLog;


String a2digitos(int num);
String filename;
void setup()
{

 
  rtc.begin();          // initialize RTC chip
  //rtc.adjust(DateTime(2000 + year, month, day, hour, minute, 0)); is uno quiere actualizar la hora

  // open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print("Buscando SD ...");
 
  // initialize the SD card
   if (!SD.begin(chipSelect)) {
    Serial.println("ERROR DE SD ");
    // don't do anything more:
    while (1);
  }
  else {   // initialization OK
     now = rtc.now(); // leendo el tiempo con el fin de 
    
    Serial.println("inicializando..");
         
    filename = a2digitos(now.day())+a2digitos(now.month())+now.year();
    filename = filename + ".txt";
 
    Serial.println(filename);
    
   if( SD.exists(filename) == 0 )   // revisar si existe el archivo
    {  
      Serial.print("\r\nCreando "); // imprimir creando archivo
      Serial.print(filename);
      Serial.println(" file");
      
      dataLog = SD.open(filename, FILE_WRITE);   // abrir archivo 
     
      if(dataLog) {                 // escribiendo cabecera           
        Serial.println("OK");       // poner un indicador led si es posible
        dataLog.println("    Fecha    |    Tiempo  | Temperatura");
        dataLog.close();   // close the file
      }
      else
        Serial.println("ERROR creando archivo");
    }
  }
  //////// END SD card setup
  
 

}
 
// main loop
int controlador;
int i;
void loop()
{

now = rtc.now();  // read current time and date from the RTC chip
if(controlador != now.minute()){
   controlador = now.minute();
            dataLog=SD.open(filename,FILE_WRITE);  // Print date and time    
            
            dataLog.println();
            dataLog.print(now.day(), DEC);
            dataLog.print("/");
            dataLog.print(now.month(), DEC);
            dataLog.print("/");
            dataLog.print(now.year(), DEC);
            dataLog.print(" ");
            dataLog.print(now.hour(), DEC);
            dataLog.print(":");
            dataLog.print(now.minute(), DEC);
            dataLog.print(":");
            dataLog.print(now.second(), DEC);
            dataLog.print("; ");  
                       
            sensors.requestTemperatures(); // Command all devices on bus to read temperature 
            for(i=0;i<=6;i++){
              Serial.println(sensors.getTempC(address[i]));
              dataLog.print(sensors.getTempC(address[i]));
              delay(10);
              dataLog.print("; ");  
              }
           
            dataLog.close(); 
 
}
 
delay(1000); 

} // end main loop

String a2digitos(int num){ // con el fin de hacer los nombre entendibes utilizamos dos digitos
  if (String(num).length()==1){
    return "0"+String(num);
    }
  else if(String(num).length()==2){}
   return String(num);
  }
//// end of code.
