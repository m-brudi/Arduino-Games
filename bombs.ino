#include <LiquidCrystal.h>
#include "pitches.h"
LiquidCrystal lcd(2,3,4,5,6,7);

const int buzzer = 9;

int playerPos = 0;
int playerRow = 0;
int currentBulletPos;
int currentBulletRow;

int bulletsPos[10];
int bulletsRow[10];
int numOfBullets = 0;

int score = 0;
String scoreTxt = "SCORE: ";
bool death = false;

byte player[]={
  B01110,
  B01110,
  B00100,
  B01110,
  B10101,
  B00100,
  B01010,
  B01010
};

byte bullet[]={
  B00000,
  B00000,
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B00000
};

void setup() {
  Serial.begin(9600);
  pinMode(8,INPUT_PULLUP);
  lcd.createChar(0,player);
  lcd.createChar(1,bullet);
  lcd.begin(16,2);
  lcd.setCursor(playerPos,0);
  lcd.write(byte(0));
  for (size_t i = 0; i < 10; i++)
  {
    bulletsPos[i] = -1;
    bulletsRow[i] = -1;
  }
}

void PlayTune(int type){
  //0 - step soung
  //1 - switch lane
  //2- death
  switch(type){
    case 0:
      tone(buzzer, NOTE_C7, 200);
    return;
    case 1:
      tone(buzzer, NOTE_B0, 200);
    return;
    case 2:
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

void ChangeLane(){
  playerRow = (playerRow == 0)? 1:0;
}

void ShootBullet(){
  int rand = random(1,100);
  if(rand > 75){
    if (numOfBullets<10){
      for (size_t i = 0; i < 10; i++){
        if(bulletsPos[i]== -1){
          PlayTune(0);
          bulletsPos[i] = 15;
          bulletsRow[i] = (rand %2 == 0) ? 0:1;
          numOfBullets++;
          break;
        }
      }
    }
  }
}

void DestroyBullet(){
  for (size_t i = 0; i < 10; i++){
    if(bulletsPos[i] == 0){
      bulletsPos[i] = -1;
      bulletsRow[i] = -1;
      numOfBullets--;
      score++;
    }
  }
}


void CalculateBulletsPos(){
  bool done = false;
  lcd.clear();
  lcd.setCursor(playerPos,playerRow);
  lcd.write(byte(0));
  while(!done){
    for (size_t i = 0; i < 10; i++)
    {
      if(bulletsPos[i] != -1){
        bulletsPos[i] -= 1;
        lcd.setCursor(bulletsPos[i],bulletsRow[i]);
        if(bulletsPos[i] == 0 && bulletsRow[i] == playerRow){
          death = true;
        }
        lcd.write(byte(1));
      }
      if( i == 9) done = true;
    }
  }
  
}

void loop() {
  if(digitalRead(8) == LOW){
    ChangeLane();
    PlayTune(1);
  }
  CalculateBulletsPos();
  DestroyBullet();
  ShootBullet();
  if(death){
    Death();
    PlayTune(2);
    delay(1000);
  }
  delay(400);
}

void Death(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("GAME OVER!");
  lcd.setCursor(4,1);
  String scoreString = scoreTxt + (score-1);
  lcd.print(scoreString);
  Reset();
  death = false;
}

void Reset(){
  score = 0;
  for (size_t i = 0; i < 10; i++)
  {
    bulletsPos[i] = -1;
    bulletsRow[i] = -1;
  }
}
