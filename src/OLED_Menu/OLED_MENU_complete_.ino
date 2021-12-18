#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
/* OLED Arduino pin Setting 
 * SDA = A4 
 * SCL = A5
 * VCC = 5V
 * GND = GND
 
 * AND Button 2,3,4,5 
*/

int SetCursor = 10; int CursorCount = 0; //커서 위치
int page = 0; //페이지 번호

int CustomNum = 0; // 커스텀세팅 시간 설정

// 버튼 핀 설정
int Down_button = 2;
int Ok_button = 3;
int Exit_button = 4;
int Up_button = 5;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

 pinMode(Down_button, INPUT_PULLUP);
 pinMode(Ok_button, INPUT_PULLUP);
 pinMode(Exit_button, INPUT_PULLUP);
 pinMode(Up_button, INPUT_PULLUP);
 
}//setup
///////////Button/////////////////////////////////
/// 페이지 커서 초기화 함수 ///
void ClearCount(){ 
   CursorCount = 0;
   SetCursor = 10;
}

/// 다운 버튼 ///
void Down(){
    SetCursor+=15;
    CursorCount+=1; // 메뉴 번호 카운트

    delay(200);
    if(CursorCount == 4){
      CursorCount = 0; SetCursor = 10;
      }
}

/// 업 버튼 ///
void Up(){
    SetCursor-=15;
    CursorCount-=1; // 메뉴 번호 카운트
    if(CursorCount == -1){
      CursorCount = 3; SetCursor = 55;
    } 
   delay(200);  
}
/// OK 버튼 ///
void Ok(){
   if(CursorCount == 0 && page == 1){ ClearCount(); page+=1; delay(200); }
   if(CursorCount == 1 && page == 1){ ClearCount(); page+=2; delay(200); }
   if(CursorCount == 2 && page == 1){ ClearCount(); page+=3; delay(200); }
   if(CursorCount == 3 && page == 1){ ClearCount(); page+=4; delay(200); }
}
/// 나가기 버튼 ///
void Exit_1(){
   if(page <= 2){
   page=1;
   ClearCount(); 
   delay(200); }
}


//////////////////////////////////////////////////
void loop(){
  
//clear display
  display.display();
  display.clearDisplay();

//display Loading time
  display.setTextSize(1);
  display.setCursor(28,32);
  
 
 if(digitalRead(Down_button) == LOW){Down();}
 if(digitalRead(Up_button) == LOW){Up();} 
 if(digitalRead(Ok_button) == LOW){Ok();}
 if(digitalRead(Exit_button) == LOW){Exit_1();}  
/////////////////////////////////////////////////

//Loading page///////////////////
 if(page == 0){ 
  for(int i = 5; i>0; i--){
      display.display();
      display.clearDisplay();
      display.setCursor(28,32);
      display.print("Loading   sec");
      display.setCursor(76,32);
      display.print(i);
      delay(100);
     }
  page++;
 }
 
//main page//////////////////////////
 if(page == 1){
    //display.display();
    //display.clearDisplay();
    display.setCursor(50,0);
    display.print("MENU");
    
    display.setCursor(10,10);
    display.print("10 Sec");

    display.setCursor(10,25);
    display.print("20 Sec");

    display.setCursor(10,40);
    display.print("30 Sec");

    display.setCursor(10,55);
    display.print("Custom Setting");

    display.setCursor(0,SetCursor);
    display.print(">");
  } 
  
//10 sec//////////////////////////
  if(page == 2){ 
    //display.display();
    //display.clearDisplay();   
  for(int i = 10; i>0; i--){
    display.display();
    display.clearDisplay();
    display.setCursor(30,0);
    display.print("10 Sec MENU");
      
    display.setCursor(28,32);
    display.print("Remain   sec");
    display.setCursor(69,32);
    display.print(i);
    delay(1000);
      if(digitalRead(Exit_button) == LOW){ClearCount();break;}
     }
    page = 1;
  } 

//20 sec//////////////////////////
  if(page == 3){ 
    //display.display();
    //display.clearDisplay();   
  for(int i = 20; i>0; i--){
    display.display();
    display.clearDisplay();
    display.setCursor(30,0);
    display.print("20 Sec MENU");
      
    display.setCursor(28,32);
    display.print("Remain   sec");
    display.setCursor(69,32);
    display.print(i);
    delay(1000);
      if(digitalRead(Exit_button) == LOW){ClearCount();break;}
     }
    page = 1;
  }   
  
//30 sec//////////////////////////
  if(page == 4){ 
    //display.display();
    //display.clearDisplay();   
  for(int i = 30; i>0; i--){
    display.display();
    display.clearDisplay();
    display.setCursor(30,0);
    display.print("30 Sec MENU");
      
    display.setCursor(28,32);
    display.print("Remain   sec");
    display.setCursor(69,32);
    display.print(i);
    delay(1000);
      if(digitalRead(Exit_button) == LOW){ClearCount();break;}
     }
    page = 1;
  } 

//custom page/////////////////////
 if(page == 5){ 
    //display.display();
    //display.clearDisplay();
  display.setCursor(30,0);
  display.print("Custom time");
  display.setCursor(10,10);
  display.print("1 Sec +");

  display.setCursor(75,10);
  display.print("Remain");

  display.setCursor(10,25);
  display.print("5 Sec +");

  display.setCursor(10,40);
  display.print("10 Sec +");

  display.setCursor(10,55);
  display.print("Start");
  
  display.setCursor(89,25);
  display.print(CustomNum);
  
  display.setCursor(0,SetCursor);
  display.print(">");
   if(digitalRead(Ok_button) == LOW && CursorCount == 0 ){CustomNum+=1; delay(200);}
   if(digitalRead(Ok_button) == LOW && CursorCount == 1 ){CustomNum+=5; delay(200);}
   if(digitalRead(Ok_button) == LOW && CursorCount == 2 ){CustomNum+=10; delay(200);}
   if(digitalRead(Ok_button) == LOW && CursorCount == 3 ){page = 6; delay(200);}  
   if(digitalRead(Exit_button) == LOW){Exit_1();
     ClearCount();
     CustomNum = 0; 
     page = 1;  
     }

  }
//custom time page/////////////////////
 if(page == 6){ 
    //display.display();
    //display.clearDisplay();   
  for(CustomNum; CustomNum>0; CustomNum--){
    display.display();
    display.clearDisplay();
    display.setCursor(30,0);
    display.print("Custom MENU");
      
    display.setCursor(28,32);
    display.print("Remain    sec");
    display.setCursor(69,32);
    display.print(CustomNum);
    delay(1000);
      if(digitalRead(Exit_button) == LOW){ClearCount();break;}
     }
    ClearCount();
    CustomNum = 0; 
    page = 1; 
  } 

}//loop
