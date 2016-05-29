/*
  Piezo MIDI glove
  
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried, Phillip Burgess, & Becky Stern for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
  
*/
#define LED     7 // Pin for heartbeat LED (shows code is working)
#define CHANNEL 1  // MIDI channel number

int note;
const int indexFinger = A9; // the piezo is connected to analog pin 9 (aka D9)
const int middleFinger = A7; // the piezo is connected to analog pin 7 (aka D6)
const int thumb = A10; // the piezo is connected to analog pin 10 (aka D10)
const int pinkyFinger = A11; // the piezo is connected to analog pin 11 (aka D12)

const int pins[] = {thumb, indexFinger, middleFinger, pinkyFinger};

//char Keys[] =   {'z','x','c','v'};

boolean currentPressed[] = {false, false, false, false};

const int threshold = 40;  // threshold value to decide when the detected sound is a knock or not

void setup()
{
   pinMode(LED, OUTPUT);
  // We dont have Serial or Keyboard available in MIDI mode!!!
  //while (!Serial)
  //Serial.begin(115200);
  //Serial.println("start");
  //Keyboard.begin();
}


void loop()                    
{ 
  for (int i=0;i<4;i++) {
    delay(1);
    long total1 = 0;
    long start = millis();
    long total =  analogRead(pins[i]);

    // check if we are sensing that a finger is touching
    // and that it wasnt already pressed
    if ((total > threshold) && (! currentPressed[i])) {
      //Serial.print("Key pressed #"); Serial.print(i);
      //Serial.print(" ("); Serial.print(Keys[i]); Serial.println(")");
      currentPressed[i] = true;
      //Keyboard.press(Keys[i]);
       //note = LOWNOTE + (i & 0x0F) * 8;
      usbMIDI.sendNoteOn(60+i, 127, CHANNEL);
    } 
    else if ((total <= threshold) && (currentPressed[i])) {
      // key was released (no touch, and it was pressed before)
      //Serial.print("Key released #"); Serial.print(i);
      //Serial.print(" ("); Serial.print(Keys[i]); Serial.println(")");
      currentPressed[i] = false;
      //Keyboard.release(Keys[i]);
      usbMIDI.sendNoteOff(60+i, 0, CHANNEL);
    }
    
    delay(5);
  }
  while(usbMIDI.read()); // Discard incoming MIDI messages
}
