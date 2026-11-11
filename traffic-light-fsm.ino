
enum State //enum data type for state of lights 
{
  GREEN,
  YELLOW,
  RED,
  REDGO
};

const int SECOND = 1000; // easy way to have delayed seconds

byte lightSetup[] = //array of bytes that represent light configuration X X X G Y R WALK STOP <--- configuration of bits
{
  B00010001, // green light, ped STOP
  B00001001, //yellow light, ped STOP
  B00000101, // red light, ped STOP
  B00000110 //red light, ped GO
};

byte seven_seg_digits[10] = { // digits for seven seg display DP G F E D C B A
  B00111111, // 0 = A B C D E F
  B00000110, // 1 = B C
  B01011011, // 2 = A B D E G
  B01001111, // 3 = A B C D G
  B01100110, // 4 = B C F G
  B01101101, // 5 = A C D F G
  B01111101, // 6 = A C D E F G
  B00000111, // 7 = A B C
  B01111111, // 8 = A B C D E F G
  B01101111  // 9 = A B C D F G
};
const int clockPin = 18; // pins will output signals for 74HC595 pins
const int latchPin =19;
const int dataPin = 21;
const int buttonPin = 4; // input pin for pedestrian button

State lightState; // enum data type that signifies which position light byte is in 
bool lightPressed = 0; // flag for when pedestrian presses button 

void countdown() // function simulates pedestrian walk countdown 
{
  // STOP bit along with seven segment display will blink and countdown simultaneously
  
  for(int count = 8; count >= 0; count--)
  {
   
    digitalWrite(latchPin, LOW); // active low latch needs to be set before any shifting of bytes
    shiftOut(dataPin, clockPin, MSBFIRST, seven_seg_digits[count + 1]); // first byte will go to second 74HC595 on the breadboard
    shiftOut(dataPin, clockPin, MSBFIRST, B00000101);
    digitalWrite(latchPin, HIGH);
    delay(500); //shift out number on seven seg
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0); // then will shift out nothing to simulate a blinking display
    shiftOut(dataPin, clockPin, MSBFIRST, B00000100);
    digitalWrite(latchPin, HIGH);
    delay(500);
    
    
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, seven_seg_digits[0]);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000101);
  digitalWrite(latchPin, HIGH); // will end with zero 
  delay(1000);
}



void setup() {
  // establishes all pins
  Serial.begin(115200);
  pinMode(clockPin,OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void changeLights(State lightState) // function changes light state by shifting out respective bytes in the array of light bytes
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin,clockPin, MSBFIRST, B00000000);
  shiftOut(dataPin, clockPin, MSBFIRST, lightSetup[lightState]);
  digitalWrite(latchPin, HIGH);
}

void loop() {
  //this loops. 
  lightState = GREEN; //light starts as green
  changeLights(lightState);
  unsigned long startTime = millis(); // constantly checking for a button press to see if a pedestrian is here 
  while(millis() - startTime < 10000)
  {
    if(digitalRead(buttonPin) == LOW && !lightPressed)
    {
      lightPressed = true;
      Serial.println("Pedestrian request active!");
    }
  }

  lightState = YELLOW;
  changeLights(lightState);

  delay(3 * SECOND);

  if(lightPressed) // light will turn red with no pedestrian go signal if buttton was not pressed
  {
    lightState = REDGO;
  }
  else
  {
    lightState = RED;
  }
  changeLights(lightState);
  delay(20 * SECOND);

  if(lightState == REDGO) // if button was pressed countdown will also occur
  {
    countdown();
    lightPressed = false;
  }
}
