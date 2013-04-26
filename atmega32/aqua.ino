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

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 0

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//Пин подключен к ST_CP входу 74HC595
int latchPin = 16;
//Пин подключен к SH_CP входу 74HC595
int clockPin = 17;
//Пин подключен к DS входу 74HC595
int dataPin = 15;

const byte first[] = {0b11011110, 0b01010000, 0b01101110, 0b01111100, 0b11110000, 0b10111100, 0b10111110, 0b01011000, 0b11111110, 0b11111100, 0b00000000, 0b00000000};
const byte second[] = {0b01000110, 0b10000110, 0b11000100, 0b11000010};

// Пины подключенные к сдвиговому регистру
int digit1 = 9; //8
int digit2 = 10; //9
int digit3 = 14; //13
int digit4 = 15; //14

int segA = 1; //Display pin 11
int segB = 2; //Display pin 7
int segC = 4; //Display pin 4
int segD = 5; //Display pin 2
int segE = 6; //Display pin 1
int segF = 3; //Display pin 10
int segG = 7; //Display pin 5

int point = 13; // Point pin 3

int count = 0;
int temp = 0;
String stringDisplay = "1234";

void setup() {
   //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
  sensors.begin();

}


void loop() {
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  while(count<2000) {
    if(temp>15&&temp<36){
      displayNumber(temp);
    }
    count++;
  }
  count = 0;
}

void displayNumber(char toDisplay) {
 int dis;
  for(int digit = 3 ; digit >= 0 ; digit--) {

    switch(digit) {
      case 0:
        stringDisplay.setCharAt(digit, toDisplay);
        break;
      case 1:
        dis = toDisplay%10;
        stringDisplay.setCharAt(digit, toDisplay);
        break;
      case 2:
        dis = toDisplay%10;
        stringDisplay.setCharAt(digit, toDisplay);
        break;
      case 3:
        dis = toDisplay%10;
        stringDisplay.setCharAt(digit, toDisplay);
        break;
    }
    toDisplay /= 10;
  }

  displayString(stringDisplay);
}

void displayString(String str)
{
   for(int digit = 3 ; digit >= 0 ; digit--)
   {
       registerWrite(str.charAt(digit), digit);
   }
}

void registerWrite(char firstId, int secondId) {
  int fst = firstId - '0';

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, first[fst]);
    shiftOut(dataPin, clockPin, MSBFIRST, second[secondId]);
    digitalWrite(latchPin, HIGH);
}
