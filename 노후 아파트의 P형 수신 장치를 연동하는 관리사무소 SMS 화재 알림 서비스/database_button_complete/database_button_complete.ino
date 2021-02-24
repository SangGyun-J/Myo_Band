#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
// 버튼 설정 
const int button1 = 14;
const int button2 = 12;
int switch1 = 0;
const int togle = 13;

// 와이파이 설정 
const char* ssid = "우옹";
const char* password = "12340000";

// lcd 설정
LiquidCrystal_I2C lcd(0x27,16,2);

void setup () {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  Wire.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
    
  }
  lcd.init(); // LCD 설정
  lcd.clear();      // LCD를 모두 지운다.
  lcd.backlight();  // 백라이트를 켠다. 
// Arduino LCD, Welcome 표시 
  lcd.setCursor(0,0);
  lcd.print("Arduino LCD");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("Welcome");
  delay(1000);  
  lcd.clear();
 
}
 
void loop() {

/*------------------------0 일때 php 전송------------------------*/
 if(digitalRead(button1) == LOW && digitalRead(togle) == LOW){
  Serial.println("0 버튼 눌림");
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://www.all-tafp.org/apartA.php");  //Specify request destination
    int httpCode = http.GET(); 
    Serial.println("begin 시작됨");

    //lcd 출력 부분, 데이터베이스 전송 완료.
    lcd.setCursor(0,0);
    lcd.print("completed");
    delay(1000);  
    lcd.clear();
    Serial.println("데이터베이스 전송 완료");
    http.end();   //Close connection
    
   
  }
 
  }
/*------------------------1 일때 php 전송------------------------*/
  if(digitalRead(button1) == LOW && digitalRead(togle) == HIGH){
  Serial.println("1 버튼 눌림");
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://www.all-tafp.org/apartB.php");  //Specify request destination
    int httpCode = http.GET(); 
    Serial.println("begin 시작됨");
     //lcd 출력 부분, 데이터베이스 전송 완료 
    lcd.setCursor(0,0);
    lcd.print("completed");
    delay(1000);  
    lcd.clear();
    Serial.println("데이터베이스 전송 완료");
    
    http.end();   //Close connection
   
    
     }
  }
}
