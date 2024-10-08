#include <IRremote.h>
#include <Servo.h>

#define on 0xB  // Код для включения
#define minus 0x7  // Код для отключения
#define plus 0x1B  // 
#define red 0x0  // 
#define green 0x3  // 
#define blue 0x1  // 
#define close 0x9  // 
#define square 0x5  // 
#define trangle 0x2  // 
#define leftUp 0xA  // 
#define up 0x6  // поворот вверх
#define rightUp 0x1A  // 
#define left 0x8  // поворот влево
#define play 0x1C  // запуск сервы
#define right 0x19  // поворот вправо
#define leftDown 0x18  // 
#define down 0x1D  // поворот вниз
#define rightDown 0x4  // 
#define letterX 0xC  // 
#define letterY 0xD  // 
#define letterZ 0xE  // 

Servo fireServo;
Servo xServo;
Servo yServo;

int currentAngle = 45;
const int lazer = 8;   // Пин, к которому подключен лазер
const int recv_pin = 9;   // Пин, к которому подключен ИК-приемник
const int axisX_pin = 10;   // Пин, к которому подключен сервопривод оси X
const int axisY_pin = 11;   // Пин, к которому подключен сервопривод оси Y
const int fire_pin = 12;   // Пин, к которому подключен сервопривод пушки
const int led_pin = 13;   // Пин, к которому подключен светодиод


int pos = 65; // Начальная позиция (середина)
bool ledState = false; // Состояние светодиода

int dartsFired = 0;
int pitchServoVal = 100;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(recv_pin);  // Инициализация ИК-приемника без флага ENABLE_LED_FEEDBACK
  pinMode(led_pin, OUTPUT);  // Устанавливаем пин светодиода как выход

  fireServo.attach(fire_pin); // Привязываем сервопривод пушки к пину
  xServo.attach(axisX_pin); // Привязываем сервопривод оси X к пину
  yServo.attach(axisY_pin); // Привязываем сервопривод оси Y к пину
  yServo.write(pos); // Устанавливаем начальную позицию
}

void loop() {
  if (IrReceiver.decode()) {  // Проверяем, есть ли данные от ИК-приемника

    // Обрабатываем команды
    switch (IrReceiver.decodedIRData.command) {
      case on:
        Serial.println("on");
        ledState = !ledState;  // Переключаем состояние светодиода
        if (ledState) {
          LightOn();
        } else {
          LightOff();
        }
        break;

      case minus:
        Serial.print("minus");
        shakeHeadNo(3);
        break;
      
      case plus:
        Serial.print("плюс");
        shakeHeadYes(3);
        break;

      case red:
        Serial.print("red");
        break;

      case green:
        Serial.print("green");
        break;

      case blue:
        Serial.print("blue");
        randomRoulette();
        break;

      case close:
        Serial.print("close");
        break;

      case square:
        Serial.print("square");
        break;

      case trangle:
        Serial.print("trangle");
        break;

      case leftUp:
        Serial.print("leftUp");
        break;

      case up:
        Serial.print("up");
        Up();
        break;

      case rightUp:
        Serial.print("rightUp");
        break;

      case left:
        Serial.print("left");
        Left(1);
        break;
        
      case play:
        Serial.print("play");
        Fire();
        break;
        
      case right:
        Serial.print("right");
        Right(1);
        break;
        
      case leftDown:
        Serial.print("leftDown");
        break;
        
      case down:
        Serial.print("down");
        Down();
        break;
        
      case rightDown:
        Serial.print("rightDown");
        break;
        
      case letterX:
        Serial.print("letterX");
        break;
        
      case letterY:
        Serial.print("letterY");
        break;
        
      case letterZ:
        Serial.print("letterZ");
        break;

      default:
        Serial.print("Неизвестная команда: ");
        Serial.println(IrReceiver.decodedIRData.command, HEX);
        break;
    }

    IrReceiver.resume();  // Готовность к следующему сигналу
  }
}

void LightOn() {
  digitalWrite(led_pin, HIGH);  // Включаем светодиод
  digitalWrite(lazer, HIGH);  // Включаем лазер
}

void LightOff() {
  digitalWrite(led_pin, LOW);  // Отключаем светодиод
  digitalWrite(lazer, LOW);  // Отключаем светодиод
}

void Fire() {
  fireServo.write(0);  // Поворот пушки по часовой
  delay(88);         // Ждем, пока сервопривод достигнет этой позиции
  fireServo.write(90); // Останавливаем сервопривод
  dartsFired++;
}

// void Left() {
//   xServo.write(111);  // Поворот вправо на средней скорости (диапазон 91-180)
//   delay(88);         // Ждем, 88 милисекунд
//   xServo.write(90); // Останавливаем сервопривод
// }

void Left(int moves){
    for (int i = 0; i < moves; i++){
        xServo.write(90 + 90); // adding the servo speed = 180 (full counterclockwise rotation speed)
        delay(90); // stay rotating for a certain number of milliseconds
        xServo.write(90); // stop rotating
        delay(5); //delay for smoothness
        Serial.println("LEFT");
  }
}

// void Right() {
//   xServo.write(69);  // Поворот влево на средней скорости (диапазон 89-0)
//   delay(88);         // Ждем, 88 милисекунд
//   xServo.write(90); // Останавливаем сервопривод
// }

void Right(int moves){ // function to move right
  for (int i = 0; i < moves; i++){
      xServo.write(90 - 90); //subtracting the servo speed = 0 (full clockwise rotation speed)
      delay(90);
      xServo.write(90);
      delay(5);
      Serial.println("RIGHT");
  }
}

void Up() {
  pos -= 10; // Поворачиваем сервопривод влево
  if (pos < 35) pos = 35; // Ограничиваем угол
  yServo.write(pos); 
}

void Down() {
  pos += 10; // Поворачиваем сервопривод вправо
  if (pos > 120) pos = 120; // Ограничиваем угол
  yServo.write(pos); 
}

void shakeHeadYes(int moves) {
      Serial.println("YES");
    int startAngle = 70; // Current position of the pitch servo
    int lastAngle = 70;
    int nodAngle = startAngle + 20; // Angle for nodding motion

    for (int i = 0; i < moves; i++) { // Repeat nodding motion three times
        // Nod up
        for (int angle = startAngle; angle <= nodAngle; angle++) {
            yServo.write(angle);
            delay(7); // Adjust delay for smoother motion
        }
        delay(50); // Pause at nodding position
        // Nod down
        for (int angle = nodAngle; angle >= startAngle; angle--) {
            yServo.write(angle);
            delay(7); // Adjust delay for smoother motion
        }
        delay(50); // Pause at starting position
    }
}

void shakeHeadNo(int moves) {
    Serial.println("NO");
    int startAngle = pos; // Current position of the pitch servo
    int lastAngle = pos;
    int nodAngle = startAngle + 60; // Angle for nodding motion

    for (int i = 0; i < moves; i++) { // Repeat nodding motion three times
        // rotate right, stop, then rotate left, stop
        xServo.write(140);
        delay(190); // Adjust delay for smoother motion
        xServo.write(90);
        delay(50);
        xServo.write(40);
        delay(190); // Adjust delay for smoother motion
        xServo.write(90);
        delay(50); // Pause at starting position
    }
}

void fireAll(){ // this function fires all of the darts by spinning the barrel
    fireServo.write(90 + 90);//start rotating the servo
    delay(165 * 6); //time for 360 degrees of rotation
    fireServo.write(90);//stop rotating the servo
    delay(5); // delay for smoothness
    Serial.println("FIRING ALL DARTS");
    dartsFired = 6;
  }

void randomRoulette() {
  Serial.println("ENTERING ROULETTE MODE");
  //unsigned long startTime = millis();
  dartsFired = 0;
  while(dartsFired < 6){ //while we still have darts, stay within this while loop
    if (dartsFired < 6){ // if we still have darts do stuff (this is redundancy to help break out of the while loop in case something weird happens)
      //pitchServoVal = 110;
      yServo.write(75); // set PITCH servo to flat angle
      xServo.write(145); // set YAW to rotate slowly
      delay(400); // rotating for a moment
      xServo.write(90); // stop
      delay(400 * random(1,4)); //wait for a random delay each time

      if(random(3) == 0){ // you have a 1 in six chance of being hit
        delay(700); 
        if(random(2) == 0){ // 50% chance to either shake the head yes before firing or just fire
          shakeHeadYes(3);
          delay(150);
          Fire(); // fire 1 dart
          delay(100);
        //} else if(random(6) == 0){
        //   shakeHeadYes(3);
        //   delay(50);
        //   fireAll(); // fire all the darts
        //   delay(50);
        } else {
          Fire(); // fire 1 dart
          delay(50);
        }
      }else{
        if(random(6) == 5){
          delay(700);
          shakeHeadNo(3);
          delay(300);
        } else{
          delay(700);
        }
      }
    } else{
      xServo.write(90); // redundancy to stop the spinning and break out of the while loop
      return;
    }
  }
  xServo.write(90); // finally, stop the yaw movement
}
