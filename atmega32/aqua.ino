// ATMEL ATMEGA32
//
//                   +---\/---+
//           (D 0) PB0  1|        |40  PA0 (AI 0 / D24)
//           (D 1) PB1  2|        |39  PA1 (AI 1 / D25)
//      INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D26)
//       PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D27)
//    PWM/SS (D 4) PB4  5|        |36  PA4 (AI 4 / D28)
//      MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D29)
//  PWM/MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D30)
//   PWM/SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D31)
//                 RST  9|        |32  AREF
//                 VCC 10|        |31  GND
//                 GND 11|        |30  AVCC
//               XTAL2 12|        |29  PC7 (D 23)
//               XTAL1 13|        |28  PC6 (D 22)
//      RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
//      TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
// RX1/INT0 (D 10) PD2 16|        |25  PC3 (D 19) TMS
// TX1/INT1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
//      PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
//      PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
//      PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
//                   +--------+
//

/*
   PCINT15-8: D7-0  : bit 1
   PCINT31-24: D15-8  : bit 3
   PCINT23-16: D23-16 : bit 2
   PCINT7-0: D31-24   : bit 0
   */

//Пин подключен к ST_CP входу 74HC595
int latchPin = 16;
//Пин подключен к SH_CP входу 74HC595
int clockPin = 17;
//Пин подключен к DS входу 74HC595
int dataPin = 15;

/*
void setup() {
  //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // отсчитываем от 0 до 255  и отображаем значение на светодиоде
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // устанавливаем синхронизацию "защелки" на LOW
    digitalWrite(latchPin, LOW);
    // передаем отсчет для вывода на зеленые светодиоды
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
    // передаем обратный отсчет  для вывода на красные светодиоды
    shiftOut(dataPin, clockPin, MSBFIRST, 255-numberToDisplay);
    //"защелкиваем" регистр, тем самым устанавливая значения на выходах
    digitalWrite(latchPin, HIGH);
    // пауза перед следующей итерацией
    delay(500);
  }
}
*/
char inputString[2];

void setup() {
   //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
}

void loop() {
  // проходим циклом по всем 16 выходам двух регистров
  for (int thisLed = 0; thisLed < 16; thisLed++) {
    // записываем сигнал в регистр для очередного светодиода
    registerWrite(thisLed, HIGH);
    // если это не первый светодиод, то отключаем предыдущий
    if (thisLed > 0) {
      registerWrite(thisLed - 1, LOW);
    }
    // если это первый светодиод, то отключаем последний
    else {
      registerWrite(15, LOW);
    }
    // делаем паузу перед следующией итерацией
    delay(250);
  }

}

// этот метод отсылает бит на сдвиговый регистр

void registerWrite(int whichPin, int whichState) {
  // для хранения 16 битов используем unsigned int
  unsigned int bitsToSend = 0;

  // выключаем светодиоды на время передачи битов
  digitalWrite(latchPin, LOW);

  // устанавливаем HIGH в соответствующий бит
  bitWrite(bitsToSend, whichPin, whichState);

  // разбиваем наши 16 бит на два байта
  // для записи в первый и второй регистр
  byte registerOne = highByte(bitsToSend);
  byte registerTwo = lowByte(bitsToSend);

  // "проталкиваем" байты в регистры
  shiftOut(dataPin, clockPin, MSBFIRST, registerTwo);
  shiftOut(dataPin, clockPin, MSBFIRST, registerOne);

  // "защелкиваем" регистр, чтобы биты появились на выходах регистра
  digitalWrite(latchPin, HIGH);
}

// Пины подключенные к сдвиговому регистру
int digit1 = 12; //PWM Display pin 12
int digit2 = 11; //PWM Display pin 9
int digit3 = 10; //PWM Display pin 8
int digit4 = 9; //PWM Display pin 6

int segA = 2; //Display pin 11
int segB = 4; //Display pin 7
int segC = 6; //Display pin 4
int segD = 7; //Display pin 2
int segE = 8; //Display pin 1
int segF = 3; //Display pin 10
int segG = 5; //Display pin 5

int point = 22; // Point pin 3

int count = 0;

void setup() {
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);

  pinMode(13, OUTPUT);
}

void loop() {
  long startTime = millis();
  while( (millis() - startTime) < 100) {
    displayNumber(count);
  }
  count ++;
  if (count == 10000)
    count = 0;
}

void displayNumber(int toDisplay) {
#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  long beginTime = millis();

  for(int digit = 4 ; digit > 0 ; digit--) {

    switch(digit) {
      case 1:
        digitalWrite(digit1, DIGIT_ON);
        break;
      case 2:
        digitalWrite(digit2, DIGIT_ON);
        break;
      case 3:
        digitalWrite(digit3, DIGIT_ON);
        break;
      case 4:
        digitalWrite(digit4, DIGIT_ON);
        break;
    }

    lightNumber(toDisplay % 10);
    toDisplay /= 10;
    lightNumber(10);
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);

  }
}
void lightNumber(int numberToDisplay) {

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH

  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
}