#include <LiquidCrystal.h>
#include "pitches.h"
LiquidCrystal lcd(2,3,4,5,6,7);
const int buzzer = 9;

byte playerRight[8]={
  B11111,
  B11111,
  B11111,
  B00010,
  B10010,
  B01111,
  B10000,
  B00000
};
byte playerLeft[8]={
  B00000,
  B10000,
  B01111,
  B10010,
  B00010,
  B11111,
  B11111,
  B11111
};
byte woodRight[8]={
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte woodLeft[8]={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte branchRight[8]={
  B11111,
  B11111,
  B11111,
  B01110,
  B00110,
  B00111,
  B00111,
  B00011
};
byte branchLeft[8]={
  B00011,
  B00111,
  B00111,
  B00110,
  B01110,
  B11111,
  B11111,
  B11111
};
byte deathSymbol[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

int treeArr[16][2];

int playerPos; // 0 - left 1- right
bool buttonPressed = false;
bool death = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(8,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  lcd.createChar(0, playerLeft);
  lcd.createChar(1, playerRight);
  lcd.createChar(2, woodLeft);
  lcd.createChar(3, woodRight);
  lcd.createChar(4, branchLeft);
  lcd.createChar(5, branchRight);
  lcd.createChar(6, deathSymbol);

  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 2; j++){
      treeArr[i][j] = 0;
    }
  }
  
  SetupGameArray();
  DisplayTree();
}

void SetupGameArray(){
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 2; j++){
      int rand = random(1,100);
      if(rand < 15){
        treeArr[i][j] = 1;
      }
    }
  }
}

void DisplayTree(){
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 2; j++){
      lcd.setCursor(i,j);
      if(j == 0){
        if(treeArr[i][j] == 0){
          lcd.write(byte(2));
        }else{
          lcd.write(byte(4));
        }
      }else{
        if(treeArr[i][j] == 0){
          lcd.write(byte(3));  
        }else{
          lcd.write(byte(5));
        }
      }
    }
  }
  if(treeArr[0][playerPos] == 1){
    Death();
  }else{
    lcd.setCursor(0,playerPos);
    lcd.write(byte(playerPos)); 
  }
}

void CutTree(){
  for(int i = 1; i < 16; i++){
    for(int j = 0; j < 2; j++){
      treeArr[i-1][j] = treeArr[i][j];
    }
  }
  int rand = random(1,100);
  if(rand > 15){
    treeArr[15][0] = 0;
    int rand2 = random(1,100);
    if (rand2 > 20){
      treeArr[15][1] = 0;
    }else{
      treeArr[15][1] = 1;
    }
  }else{
    treeArr[15][0] = 1;
    treeArr[15][1] = 0;
  }
  DisplayTree();
}

void Death(){
  death = true;
  PlayTune(1);
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 2; j++){
      treeArr[i][j] = 0;
      lcd.setCursor(i,j);
      lcd.write(6);
      delay(100);
    }
  }
  delay(300);
  death = false;
  SetupGameArray();
  DisplayTree();
}

void PlayTune(int type){
  //0 - tree cut
  //1 - death
  switch(type){
    case 0:
      tone(buzzer, NOTE_C7, 50);
      delay(50);
    return;
    case 1:
      tone(buzzer, NOTE_C3,500);
      delay(520);
      noTone(buzzer);
      tone(buzzer, NOTE_G2,500);
      delay(520);
      noTone(buzzer);
      tone(buzzer, NOTE_A2,500);
      delay(520);
      noTone(buzzer);
    return;
  }
}

void loop() {
  if(!death){
    if(digitalRead(8) == LOW){
      playerPos = 0;
      CutTree();
      PlayTune(0);
      delay(250);
    }
    if(digitalRead(10) == LOW){
      playerPos = 1;
      CutTree();
      PlayTune(0);
      delay(250);  
    }
  }
}