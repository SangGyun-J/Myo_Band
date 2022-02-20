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
//Piezo 
int piezo_output = 10;
int num_tones = 8;
int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};

// 칼만필터 라이브러리//////////////////////////////////////////////////////
// Sample for Kalman filter for Arduino library https://github.com/nut-code-monkey/KalmanFilter-for-Arduino
#include "KalmanFilter.h"
KalmanFilter kalmanFilter;

const int numReadings = 15; // 이동평균을 구할 갯수를 지정, 숫자가 변하지 않게 const int로 지정

int readings[numReadings];      // 신호값을 읽는 배열을 지정, 배열의 크기는 위의 값으로 정함
int readIndex = 0;              // 몇번째 신호인지를 표시하는 인덱스 변수
int total = 0;                  // 합계 변수
int average = 0;                // 평균값 변수

int inputPin = A0; // 입력핀 정의

double getSomeValue()  {
  int emg = analogRead(inputPin); // 센서 입력값을 읽어온다, 정상 작동하지 않을 수 있어서 먼저 변수에 저장한 뒤 읽어오도록 수정
  total = total - readings[readIndex]; //가장 오래된 data를 합계에서 빼서 없앤다
  
  //readings[readIndex] = analogRead(inputPin); //원본코드
  readings[readIndex] = emg; // 센서 입력값을 배열에 저장
  
  total = total + readings[readIndex]; // 읽은 값을 합계에 더한다
  
  readIndex = readIndex + 1; // 신호값을 읽은 인덱스를 1 증가 시킨다

  if (readIndex >= numReadings) { // 만약 신호를 읽는 인덱스의 값이 평균갯수보다 커지면
    readIndex = 0; // 0으로 만들어 처음부터 다시 시작한다
  }

  average = total / numReadings; // 평균값을 구한다  
  return average;
}

////////////////////////////////////////////////////////////////////////

int SetCursor = 10; int CursorCount = 0; //커서 위치.
int page = 0; //페이지 번호
int save = 0;// Maxdata와 비교해서 최대값 저장하는 변수
int CustomNum = 0; // Custom Setting 시간 설정
int CustomNum2 = 0;
int count = 0; //타이머 몇번 반복했는지 표시하는 변수
int calcount = 0; // FES신호 들어올때 판별하기 위한 카운트

int Maxdata = 0; // EMG 값 비교 위해 받아오는 변수
int Maxdata2 = 0;// EMG 최대값 저장 변수

int Mindata = 1000;// 최소값 비교위해 받아오는 변수
int Startdata = 0; // 최소값 저장 변수
int Start10_1 = 0; // 최소값 10분의 1 저장변수
int Startsave = 0; // 최소값 계산용 저장 변수
double MinSet = 0;// 10% 씩 최소값 설정
int percent = 0; //퍼센트 값

// 버튼 핀 설정
int Down_button = 2;
int Ok_button = 3;
int Exit_button = 4;
int Up_button = 5;
int GPIO_Output = 6;

/////////////////////////////////////// 셋업 //////////////////////////
void setup() {
  
  pinMode(piezo_output, OUTPUT);
  
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
 digitalWrite(GPIO_Output, LOW);


 //kalman
     kalmanFilter.setState( getSomeValue() );   

    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      readings[thisReading] = 0;
    }
  tone(piezo_output, tones[0]);
  delay(400);
  tone(piezo_output, tones[2]);
  delay(400);
  tone(piezo_output, tones[4]);
  delay(400);
  tone(piezo_output, tones[7]);
  delay(400);
  noTone(piezo_output);
  delay(1400);
    
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
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100);
   // delay(200);
    if(CursorCount == 4){ClearCount();}
}

/// 업 버튼 ///
void Up(){
    SetCursor-=15;
    CursorCount-=1; // 메뉴 번호 카운트
    if(CursorCount == -1){
      CursorCount = 3; SetCursor = 55;
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100);
    } 
   //delay(200);  
}
/// OK 버튼 ///
void Ok(){
   if(CursorCount == 0 && page == 1){ 
    ClearCount(); page+=1; 
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100); 
    //delay(200); 
    }
   if(CursorCount == 1 && page == 1){
    ClearCount(); page+=2; 
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100);
    //delay(200); 
    }
   if(CursorCount == 2 && page == 1){
    ClearCount(); page+=3; 
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100);
    //delay(200); 
    }
   if(CursorCount == 3 && page == 1){
    ClearCount(); page+=5; 
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100);    
    
    //delay(200);
    }
}
/// 나가기 버튼 ///
void Exit_1(){
   if(page <= 2){
   page=1;
   ClearCount();
   tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100); 
   //delay(200); 
   }
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

//Loading page//////////////////////////////////////////////
 if(page == 0){ 
  for(int i = 5; i>0; i--){
      display.display();
      display.clearDisplay();
      display.setCursor(28,32);
      display.print(F("Loading   sec"));
      display.setCursor(76,32);
      display.print(i);
      delay(500);
     }
  page++;
 }
 
//main page//////////////////////////////////////////////
 if(page == 1){
    digitalWrite(GPIO_Output, LOW); // Mircrostim HIGH, 버튼작동 멈춤

    delay(100);
    display.setCursor(50,0);
    display.print(F("MENU"));
 
    display.setCursor(10,10);
    display.print(F("Info"));

    display.setCursor(10,25);
    display.print(F("Continuous"));

    display.setCursor(10,40);
    display.print(F("Custom Setting"));
    
    display.setCursor(10,55);
    display.print(F("Sensor Measure"));

    display.setCursor(0,SetCursor);
    display.print(">");    
  } 

//Info page//////////////////////////
  if(page == 2){ 
    display.setCursor(50,0);
    display.print(F("Info"));

    display.setCursor(15,35);
    display.print(F("Rodem technology"));

  }//page2 

//////////////// Continuous //////////////////////////////////////////////
  if(page == 3){    
  for(int i = 0; ; i++){
//    int val = analogRead(A0);
    double value = getSomeValue();
    kalmanFilter.correct(value);
    int val = kalmanFilter.getState();
    display.display();
    display.clearDisplay();
    display.setCursor(30,0);
    display.print(F("Continuous"));

    display.setCursor(28,14);
    display.print(F("Sensor= "));

    display.setCursor(10,32);
    display.print(F("val=  "));
    display.setCursor(40,32);
    display.print(val);

    display.setCursor(50,32);
    display.print(F("        sec"));
    display.setCursor(75,32);
    display.print(i/10);

    display.setCursor(10,50);
    display.print(F("min=  "));
    display.setCursor(40,50);
    display.print(Startsave);

    display.setCursor(70,50);
    display.print(F("Max=  "));
    display.setCursor(100,50);
    display.print(Maxdata);
    
    //Min(Startsave)값과 Max값 사이에 val(근전도 신호)가 잡히면,Microstim 실행
    if(Startsave < val && Maxdata > val){digitalWrite(GPIO_Output, HIGH);
      display.setCursor(75,14);
      display.print("ON");
      
      int FES = min(Mindata, val); // 전기 들어오고의 최소값
      int FEScal = 0;              // 5번 평균 내기위한 변수
      for(int i=0; i<5; i++){
         FEScal += val;            // 현재 들어오는 val값 쌓기
         if(i == 4){
            FEScal = FEScal / 5;
            if(FEScal > FES && FEScal <= FES+10){ // 5번 평균값이 최소값 ~ 최소값 +10 사이에 있으면
              calcount++;                         // 카운트 증가
            }
            else{calcount = 0;}                   // 범위의 값이 지속적으로 안들어오면 카운트 초기화
         }
      }//for
        if(calcount >= 5){ //카운트가 5 이상이면 전기 차단, val 값 초기화 및 카운트 초기화
          digitalWrite(GPIO_Output, LOW);
          val = Startdata - 5; // LOW시켜주고 완전하게 꺼지기 위해서 min 값보다 더 작게만들기
          calcount = 0;
         }
    }// HIGH
    
     else{digitalWrite(GPIO_Output, LOW);
      display.setCursor(75,14);
      display.print("OFF");
     }
    delay(100);
    if(digitalRead(Exit_button) == LOW){break; ClearCount(); page=1;delay(200);}
   }//for
   
   count++;
   if(digitalRead(Exit_button) == LOW){ ClearCount(); page=1;}
  }//page3   

//custom page//////////////////////////////////////////////
 if(page == 4){ 
  display.setCursor(30,0);
  display.print(F("Custom time"));
  display.setCursor(10,10);
  display.print(F("1 Sec +"));

  display.setCursor(75,10);
  display.print(F("Remain"));

  display.setCursor(10,25);
  display.print(F("5 Sec +"));

  display.setCursor(10,40);
  display.print(F("5 Sec -"));

  display.setCursor(10,55);
  display.print(F("Start"));

  display.setCursor(89,25);
  display.print(CustomNum);
  
  display.setCursor(0,SetCursor);
  display.print(">");
   if(digitalRead(Ok_button) == LOW && CursorCount == 0 ){
    CustomNum+=1; 
    //delay(200);
  
    }// 1 Sec Setting Button
   if(digitalRead(Ok_button) == LOW && CursorCount == 1 ){
    CustomNum+=5; 
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100); 
    
    }// 5 Sec Setting Button
   if(digitalRead(Ok_button) == LOW && CursorCount == 2 )
   {
    CustomNum-=5;
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100);  

   }// -5 Sec Setting Button
   if(CustomNum < -1) {CustomNum = 0;}
   if(digitalRead(Ok_button) == LOW && CursorCount == 3 )
   {page = 5; 
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100);
   
   }// Start Button
   if(digitalRead(Exit_button) == LOW){Exit_1();// 나가기 버튼, 첫번째 메뉴로 돌아감
     ClearCount();
     CustomNum = 0; 
     page = 1; 
    tone(piezo_output, tones[2]);
    delay(100);
    noTone(piezo_output);
    delay(100); 
     }
  }//page4
  
//custom time page//////////////////////////////////////////////
 if(page == 5){
  CustomNum2 = CustomNum;
  for(CustomNum*=10; CustomNum>0; CustomNum--){
    
    double value = getSomeValue();
    kalmanFilter.correct(value);
    int val = kalmanFilter.getState();

    display.display();
    display.clearDisplay();
    
    display.setCursor(30,0);
    display.print(F("Custom MENU"));
    display.setCursor(28,14);
    display.print(F("Sensor= "));

    display.setCursor(10,32);
    display.print(F("val=  "));
    display.setCursor(40,32);
    display.print(val);
      
    display.setCursor(50,32);
    display.print(F("        sec"));
    display.setCursor(75,32);
    display.print(CustomNum/10);

    display.setCursor(10,50);
    display.print(F("min=  "));
    display.setCursor(40,50);
    display.print(Startsave);

    display.setCursor(70,50);
    display.print(F("Max=  "));
    display.setCursor(100,50);
    display.print(Maxdata);

    if(Startsave < val && Maxdata > val){digitalWrite(GPIO_Output, HIGH);
      display.setCursor(75,14);
      display.print("ON");
      }
     else{digitalWrite(GPIO_Output,LOW);
      display.setCursor(75,14);
      display.print("OFF");
     }
    delay(100);
      if(digitalRead(Exit_button) == LOW){ClearCount(); CustomNum = 0; break; page=1; delay(200);}
     }
     if(digitalRead(Exit_button) == LOW){ ClearCount(); page=1;}
     if(CustomNum == 0){ClearCount(); CustomNum = 0; page = 1;}   
  }//page5 
  
//Sensor Measure//////////////////////////////////////////////
if(page == 6){ 
    display.setCursor(25,0);
    display.print(F("Sensor Measure"));

    display.setCursor(10,10);
    display.print(F("Measure Test"));
    
    display.setCursor(10,25);
    display.print(F("MIN   %"));

    display.setCursor(30,25);
    display.print(percent);
    
    display.setCursor(10,40);
    display.print(F("SAVE"));
    
    display.setCursor(10,55);
    display.print(F("Min=  "));
    display.setCursor(45,55);
    display.print(Startsave);
    
    display.setCursor(70,55);
    display.print(F("Max=  "));
    display.setCursor(100,55);
    display.print(Maxdata);
   
    display.setCursor(0,SetCursor);
    display.print(F(">"));

    // 1번째 커서 - measure start (측정 시작)  
    if(digitalRead(Ok_button) == LOW && CursorCount == 0 ){page = 7;}

    // 2번째 커서 - Maxdata(최대값)의 10분의 1씩 커지면서 Mindata(최소값) 설정 - 2번째 커서 
    if(digitalRead(Ok_button) == LOW && CursorCount == 1 && Maxdata > 0){
      // Maxdata, Startdata 최대 최소   
      // 10분의 1 = (최대값 - 최소값) * 10분의 1 씩 
      if(MinSet >= 0.1){percent+=10;}
      if(MinSet >= 1.0){MinSet = 0; percent = 0;}
      Start10_1 = (Maxdata - Startdata) * MinSet;
      Startsave = Startdata + Start10_1;
      display.setCursor(45,55);
      display.print(Startsave);
      MinSet+=0.1;
      
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
     ClearCount(); CustomNum = 0; Maxdata = 0; Mindata = 1000; Maxdata2 = 0; 
     page = 1; delay(200); } 
  }//page6 

//test start & Min page//////////////////////////////////////////////
 if(page == 7){ 
  int a = 80; //반복문 설정시간 10 = 1초
  Maxdata = 0;
   
    tone(piezo_output, tones[2]);
    delay(200);
    noTone(piezo_output);
    delay(200);
    
  for(a; a>0; a--){
//    int val = analogRead(A0);
    double value = getSomeValue();
    kalmanFilter.correct(value);
    int val = kalmanFilter.getState();
    Serial.println(val);
    display.display();
    
    display.clearDisplay();
    display.setCursor(25,0);
    display.print(F("Min measure.."));
      
    display.setCursor(25,26);
    display.print(F("Remain   sec"));
    display.setCursor(66,26);
    display.print(a/10);
   
    display.setCursor(25,46);
    display.print(F("min =  "));
    display.setCursor(80,46);
    display.print(val);

    //최소값 구하는 식
    Startdata = min(Mindata, val);
    delay(100);
    
     if(a == 1){
          ClearCount();
          CustomNum = 0; 
          page = 8;
      }   
    if(digitalRead(Exit_button) == LOW){ClearCount(); page = 6; break;}
   }//for
   
   Startdata = Startdata + 2; // 최소값 수치 조정
 }//page7
 
//max page//////////////////////////////////////////////
 if(page == 8){ 
  int a = 80; //반복문 설정시간 10 = 1초
    tone(piezo_output, tones[2]);
    delay(200);
    noTone(piezo_output);
    delay(200);
  for(a; a>0; a--){
//    int val = analogRead(A0);
    double value = getSomeValue();
    kalmanFilter.correct(value);
    int val = kalmanFilter.getState();
    Serial.println(val);
    display.display();
    
    display.clearDisplay();
    display.setCursor(25,0);
    display.print(F("Max measure.."));
      
    display.setCursor(25,26);
    display.print(F("Remain   sec"));
    display.setCursor(66,26);
    display.print(a/10);
   
    display.setCursor(25,46);
    display.print(F("max =  "));
    display.setCursor(80,46);
    display.print(val);
    
// 최대값 구하는 식    
    Maxdata = max(val, Maxdata2);
    if(val > Maxdata2){ Maxdata2 = val; }

    delay(100);
     if(a == 1){
          ClearCount();
          CustomNum = 0; 
          page = 9;
      }   

    if(digitalRead(Exit_button) == LOW){ClearCount(); page = 6; break;}
   }  
  tone(piezo_output, tones[0]);
  delay(250);
  tone(piezo_output, tones[2]);
  delay(250);
  tone(piezo_output, tones[4]);
  delay(250);
  tone(piezo_output, tones[7]);
  delay(250);
  noTone(piezo_output);
  delay(1000);
 }//page8   

// FES page//////////////////////////////////////////////
 if(page == 9){
   int a = 50; //반복문 설정시간 10 = 1초
//   unsigned long timetime = millis();
   digitalWrite(GPIO_Output, HIGH);
   for(a; a>0; a--){
    double value = getSomeValue();
    kalmanFilter.correct(value);
    int val = kalmanFilter.getState();
    Serial.println(val);
    display.display();
    
    display.clearDisplay();
    display.setCursor(25,0);
    display.print(F("FES measure.."));
      
    display.setCursor(25,26);
    display.print(F("Remain   sec"));
    display.setCursor(66,26);
    display.print(a/10);
   
    display.setCursor(25,46);
    display.print(F("max =  "));
    display.setCursor(80,46);
    display.print(val);
    
// 최대값 구하는 식    
    int FES = 0;
    FES = min(Mindata, val);
    delay(100);
     if(a == 1){
          ClearCount();
          CustomNum = 0; 
          page = 6;
          digitalWrite(GPIO_Output, LOW);
      }   
    if(digitalRead(Exit_button) == LOW){ClearCount(); page = 6; digitalWrite(GPIO_Output, LOW); break;}
   }//for  
 }//page9
 
}//loop
