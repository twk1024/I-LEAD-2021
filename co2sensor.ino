#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <MHZ19.h>
#include <MHZ19PWM.h>
#include <dht11.h>         

char auth[] = "Blynk Auth Token";
char ssid[] = "Wifi SSID";
char pass[] = "Wifi PW";

SoftwareSerial ss(D7,D8);
MHZ19 mhz(&ss);

LiquidCrystal_I2C lcd(0x27, D1, D2);

dht11 DHT11;      

#define DHT11PIN D5                     

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  Serial.println(F("Starting..."));
  ss.begin(9600);
    
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  pinMode(D0, OUTPUT);
  pinMode(D4, OUTPUT);

}

double Fahrenheit(double celsius)                    // Fahrenheit라는 함수를 만듬. 함수의 입력값은 celsius에 저장
{
  return 1.8 * celsius + 32;                         // 입력된 celsius값에 1.8를 곱하고 32를 더하여 출력
}

void loop()
{
  MHZ19_RESULT response = mhz.retrieveData();
  if (response == MHZ19_RESULT_OK)
  {
    Serial.print(F("CO2: "));
    Serial.print(mhz.getCO2());
    Serial.println("ppm");
  }
  else
  {
    Serial.print(F("Error, code: "));
    Serial.println(response);
  }

//dht11
  Serial.println("\n");                           
  int chk = DHT11.read(DHT11PIN);                   
  Serial.print("Humidity: ");                 
  Serial.print((float)DHT11.humidity, 2);
  Serial.println("%");       
  Serial.print("Temperature): ");              
  Serial.print((float)DHT11.temperature, 2);
  Serial.println("C");  
  delay(2000);


  // Blynk 알림 전송
   if (mhz.getCO2() > 800)
  {
    Blynk.notify("CO2 - 800ppm 초과");
    }

    if (DHT11.humidity > 80)
  {
    Blynk.notify("습도 - 80% 초과");
    }

     if (DHT11.temperature > 30)
  {
    Blynk.notify("온도 - 30C 초과");
    }
  
  lcd.clear();
  lcd.print("CO2:");
  lcd.setCursor(5,0);
  lcd.print(mhz.getCO2());
  lcd.setCursor(9,0);
  lcd.print("ppm");
  
  lcd.setCursor(0,1);
  lcd.print("Temp:");
  lcd.setCursor(6,1);
  lcd.print((float)DHT11.temperature, 0);
  lcd.setCursor(9,1);
  lcd.print("C");

  lcd.setCursor(13,0);
  lcd.print("Humi:");
  lcd.setCursor(13,1);
  lcd.print((float)DHT11.humidity, 0);

  Blynk.virtualWrite(V0, mhz.getCO2()); // Blynk로 센서 데이터값 전송
  Blynk.virtualWrite(V1, DHT11.humidity);
  Blynk.virtualWrite(V2, DHT11.temperature);



  if (mhz.getCO2() > 800)
  {
    digitalWrite(D0, LOW);
    digitalWrite(D4, HIGH);
                
  }
  else
  {
    digitalWrite(D0, HIGH);
    digitalWrite(D4, LOW);
   
  }
  
  delay(13000);
  
}
