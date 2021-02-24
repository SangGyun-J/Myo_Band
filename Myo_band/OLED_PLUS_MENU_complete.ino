#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
/* adafruit SSD1306 라이브러리, 라이브러리 추가하기에도 있습니다.
 *  https://github.com/adafruit/Adafruit_SSD1306 
 *  OLED Arduino pin Setting (OLED 핀 세팅)
 * SDA = A4 
 * SCL = A5
 * VCC = 5V
 * GND = GND
 
 * AND Button 2,3,4,5,6,(A0, 근전도) 
*/
int SetCursor = 10; int CursorCount = 0; //커서 위치.
int page = 0; //페이지 번호
int save = 0;// Maxdata와 비교해서 최대값 저장하는 변수
int CustomNum = 0; // Custom Setting 시간 설정
int CustomNum2 = 0;
int count = 0; //타이머 몇번 반복했는지 표시하는 변수

int Maxdata = 0; // EMG 값 비교 위해 받아오는 변수
int Maxdata2 = 0;// EMG 최대값 저장 변수
int Mindata = 0;// 최소값
double MinSet = 0.1;// 10% 씩 최소값 설정

// 버튼 핀 설정
int Down_button = 2;
int Ok_button = 3;
int Exit_button = 4;
int Up_button = 5;
int GPIO_Output = 6;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();
  display.setTextColor(WHITE);

 pinMode(Down_button, INPUT_PULLUP);
 pinMode(Ok_button, INPUT_PULLUP);
 pinMode(Exit_button, INPUT_PULLUP);
 pinMode(Up_button, INPUT_PULLUP);
 
 pinMode(GPIO_Output, OUTPUT);
 digitalWrite(GPIO_Output, HIGH);
 //digitalWrite(GPIO_Output, LOW);
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
    if(CursorCount == 4){ClearCount();}
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
   if(CursorCount == 3 && page == 1){ ClearCount(); page+=5; delay(200); }
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
  
//button 
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
    digitalWrite(GPIO_Output, HIGH); // Mircrostim HIGH, 버튼작동 멈춤
    int val = analogRead(A0);
    count = 0; //5, 10 custom 반복횟수 초기화
    delay(100);
    display.setCursor(50,0);
    display.print("MENU");
 
    display.setCursor(10,10);
    display.print("5 Sec");

    display.setCursor(10,25);
    display.print("10 Sec");

    display.setCursor(10,40);
    display.print("Custom Setting");
    
    display.setCursor(10,55);
    display.print("Sensor Measure");

    display.setCursor(0,SetCursor);
    display.print(">");    
  } 

//5 sec//////////////////////////
  if(page == 2){ 
  for(int i = 50; i>0; i--){
    int val = analogRead(A0);
    display.display();
    display.clearDisplay();
    display.setCursor(30,0);
    display.print("5 Sec MENU");

    display.setCursor(28,14);
    display.print("count = ");
    display.setCursor(75,14);
    display.print(count);
      
    display.setCursor(28,32);
    display.print("Remain   sec");
    display.setCursor(69,32);
    display.print(i/10);
    
    display.setCursor(24,50);
    display.print("muscle =  ");
    display.setCursor(80,50);
    display.print(val);
    
    //Min값과 Max값 사이에 val(근전도 신호)가 잡히면, 5초동안 Microstim 실행
       if(Mindata < val && Maxdata > val){digitalWrite(GPIO_Output, LOW);}
     else{digitalWrite(GPIO_Output, HIGH);}
    delay(100);
    //나가기 버튼, ClearCount()함수 실행, ( > 커서 위치 초기화)
    if(digitalRead(Exit_button) == LOW){break; ClearCount(); page=1;}
    }
    count++;
  } 

//10 sec//////////////////////////
  if(page == 3){    
  for(int i = 100; i>0; i--){
    int val = analogRead(A0);
    display.display();
    display.clearDisplay();
    display.setCursor(30,0);
    display.print("10 Sec MENU");

    display.setCursor(28,14);
    display.print("count = ");
    display.setCursor(75,14);
    display.print(count);
      
    display.setCursor(28,32);
    display.print("Remain   sec");
    display.setCursor(69,32);
    display.print(i/10);

    display.setCursor(24,50);
    display.print("muscle =  ");
    display.setCursor(80,50);
    display.print(val);
    //Min값과 Max값 사이에 val(근전도 신호)가 잡히면, 10초동안 Microstim 실행
    if(Mindata < val && Maxdata > val){digitalWrite(GPIO_Output, LOW);}
     else{digitalWrite(GPIO_Output, HIGH);}
    delay(100);
    if(digitalRead(Exit_button) == LOW){break; ClearCount(); page=1;delay(200);}
   }
   count++;
   if(digitalRead(Exit_button) == LOW){ ClearCount(); page=1;}
  }   
  
//custom page/////////////////////
 if(page == 4){ 
  display.setCursor(30,0);
  display.print("Custom time");
  display.setCursor(10,10);
  display.print("1 Sec +");

  display.setCursor(75,10);
  display.print("Remain");

  display.setCursor(10,25);
  display.print("5 Sec +");

  display.setCursor(10,40);
  display.print("5 Sec -");

  display.setCursor(10,55);
  display.print("Start");
  
  display.setCursor(89,25);
  display.print(CustomNum);
  
  display.setCursor(0,SetCursor);
  display.print(">");
   if(digitalRead(Ok_button) == LOW && CursorCount == 0 ){CustomNum+=1; delay(200);}// 1 Sec Setting Button
   if(digitalRead(Ok_button) == LOW && CursorCount == 1 ){CustomNum+=5; delay(200);}// 5 Sec Setting Button
   if(digitalRead(Ok_button) == LOW && CursorCount == 2 ){CustomNum-=5; delay(200);}// -5 Sec Setting Button
   if(CustomNum < -1) {CustomNum = 0;}
   if(digitalRead(Ok_button) == LOW && CursorCount == 3 ){page = 5; delay(200);}// Start Button
   if(digitalRead(Exit_button) == LOW){Exit_1();// 나가기 버튼, 첫번째 메뉴로 돌아감
     ClearCount();
     CustomNum = 0; 
     page = 1;  
     }

  }
//custom time page/////////////////////
 if(page == 5){
  CustomNum2 = CustomNum;
  for(CustomNum*=10; CustomNum>0; CustomNum--){
    int val = analogRead(A0);
    Serial.println(val);
    display.display();
    display.clearDisplay();
    display.setCursor(30,0);
    display.print("Custom MENU");

    display.setCursor(28,14);
    display.print("count = ");
    display.setCursor(75,14);
    display.print(count);
      
    display.setCursor(28,32);
    display.print("Remain    sec");
    display.setCursor(69,32);
    display.print(CustomNum/10);

    display.setCursor(24,50);
    display.print("muscle =  ");
    display.setCursor(80,50);
    display.print(val);
    
    if(Mindata < val && Maxdata > val){digitalWrite(GPIO_Output, LOW);}
     else{digitalWrite(GPIO_Output, HIGH);}
    delay(100);
      if(digitalRead(Exit_button) == LOW){ClearCount(); CustomNum = 0; break; page=1; delay(200);}
     }
     if(digitalRead(Exit_button) == LOW){ ClearCount(); page=1;}
     if(CustomNum < CustomNum2){CustomNum = CustomNum2;} 
    count++;  
  } 
  
//Sensor Measure/////////////////////
if(page == 6){ 
    display.setCursor(25,0);
    display.print("Sensor Measure");

    display.setCursor(10,10);
    display.print("Measure Test");
    
    display.setCursor(10,25);
    display.print("MIN 1/10");
    
    display.setCursor(10,40);
    display.print("SAVE");
    
    display.setCursor(10,55);
    display.print("Min=  ");
    display.setCursor(40,55);
    display.print(Mindata);
    
    display.setCursor(70,55);
    display.print("Max=  ");
    display.setCursor(100,55);
    display.print(Maxdata);
   
    display.setCursor(0,SetCursor);
    display.print(">");
    // 1번째 커서 - measure start (측정 시작)  
    if(digitalRead(Ok_button) == LOW && CursorCount == 0 ){page = 7;}
    
    // 2번째 커서 - Maxdata(최대값)의 10분의 1씩 커지면서 Mindata(최소값) 설정 - 2번째 커서 
    if(digitalRead(Ok_button) == LOW && CursorCount == 1 && Maxdata > 0){
      Mindata = Maxdata * MinSet;
      MinSet+=0.1;
      if(MinSet > 1.1){Mindata = 0; MinSet = 0.1;}
      delay(200);
     }
     
    // 3번째 커서 - 최대값, 최소값 저장 후 첫번째 페이지로 이동(측정 범위 저장하기 위해서 꼭 해야함. 아니면 초기화)
    if(digitalRead(Ok_button) == LOW && CursorCount == 2){
      Maxdata = Maxdata; Mindata = Mindata;
      ClearCount(); page = 1;
      delay(200);
    }
   if(CursorCount == 3){ClearCount();}// 커서 위치 조정, 첫번째로 돌아감
   if(digitalRead(Exit_button) == LOW){// 나가기 버튼, 첫번째 메뉴로 돌아감
     ClearCount();
     CustomNum = 0;
     Maxdata = 0;
     Mindata = 0;
     Maxdata2 = 0; 
     page = 1; } 
  } 

//test start/////////////////////
 if(page == 7){ 
  int a = 30; //반복문 설정시간 10 = 1초
  Maxdata = 0;
  for(a; a>0; a--){
    int val = analogRead(A0);
    Serial.println(val);
    display.display();
    
    display.clearDisplay();
    display.setCursor(25,0);
    display.print("measure start..");
      
    display.setCursor(25,26);
    display.print("Remain   sec");
    display.setCursor(66,26);
    display.print(a/10);
   
    display.setCursor(25,46);
    display.print("muscle =  ");
    display.setCursor(80,46);
    display.print(val);
    
    Maxdata = max(val, Maxdata2);
    if(val > Maxdata2){ Maxdata2 = val; }
    delay(100);
     if(a == 1){
          ClearCount();
          CustomNum = 0; 
          page = 6; 
      }   
    if(digitalRead(Exit_button) == LOW){ClearCount(); page = 6; break;}
   }  
      
 } 
}//loop
