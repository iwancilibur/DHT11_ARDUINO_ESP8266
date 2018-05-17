#include<dht.h>
#include<SoftwareSerial.h>
#define DHT_SENSOR_PIN 2
#define BAUDRATE 115200

dht DHT;
SoftwareSerial serial(3,4);
String server = "http://iot-project.laurensius-dede-suhardiman.com/index.php/api/save_data/";
//String server = "http://192.168.0.102/iotserver/index.php/api/save_data/";
  
int respon_dht11 = 0;
char inString[32];
char charFromWeb[9];
boolean startRead = false; 


void setup() {
  Serial.println("-------------------------------------------------");
  Serial.println("Setup Serial Baudrate . . .");
  Serial.begin(BAUDRATE);
  Serial.println("Setup ESP8266 Baudrate . . .");
  serial.begin(BAUDRATE);
  Serial.println("Setup Sensor DHT11 . . .");
  inisialisasi_dht11();
  Serial.println("-------------------------------------------------");
}

void loop() {
  String data = ambil_data_dht11();
  String request = server + data;  
  serial.print(request); 
  delay(1000);
  String response = bacaWebText();
  Serial.print("RESPONSE : ");
  Serial.println(response);
  delay(2000);
  Serial.println();  
  delay(4000);
}

void inisialisasi_dht11(){
  respon_dht11 = DHT.read11(2);
  switch (respon_dht11){
    case DHTLIB_OK:  
      Serial.println("Status: OK");
      Serial.println("Setup berhasil!");
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.println("Status: Checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.println("Status: Time out error");
      break;
    case DHTLIB_ERROR_CONNECT:
      Serial.print("Status: Connect error");
      break;
    case DHTLIB_ERROR_ACK_L:
      Serial.print("Status: Ack Low error");
      break;
    case DHTLIB_ERROR_ACK_H:
      Serial.print("Status: Ack High error");
      break;
    default: 
      Serial.println("Status: Unknown error"); 
      break;
  }
  delay(2000);
}

String ambil_data_dht11(){
  String data;
  Serial.print("Temperatur : ");
  Serial.println(DHT.temperature);
  Serial.print("Kelembaban : ");
  Serial.println(DHT.humidity);
  delay(1000);
  String temperature = String(DHT.temperature);
  String humidity = String(DHT.humidity);
  data = temperature + "/" + humidity + "/" ;
  return data;
}

String bacaWebText(){
  unsigned int time;
  Serial.println("Baca respon dari server . . . "); 
  Serial.println("Mohon menunggu . . . ");
  time = millis();
  Serial.print("Timer Millis () : ");
  Serial.println(time);
  int stringPos = 0;  
  int unvailable_ctr = 0;
  while(true){
    if (serial.available()) {
      char c = serial.read();
//      Serial.print(c);
      if (c == '#' ) { 
//        Serial.println("Menemukan start key # dengan isi : ");
        startRead = true;  
      }else if(startRead){
        if(c != '^'){ 
          inString[stringPos] = c;
          stringPos ++;
        }else{
          startRead = false;
          Serial.println("Baca respon dari server selesai!");
          return inString;
        }
      }
    }else{
       delay(50);
       unvailable_ctr++;
       if(unvailable_ctr == 25){
         Serial.println("Koneksi mengalami time out");
         Serial.println("Sambungan diputuskan . . . ");
         Serial.println("Reset...");
         return inString;
       }
    }
  }
}
