

int pinRead;
char pinAssignments[6] = {'A0','A1','A2','A3','A4','A5'};
byte PadNote[6] = {60,72,84,96,108,120};         // MIDI notes from 0 to 127 (Mid C = 60)
int PadCutOff[6] = {20,20,20,20,20,20};           // Minimum Analog value to cause a drum hit
int MaxPlayTime[6] = {200,200,200,200,200,200};               // Cycles before a 2nd hit is allowed
#define  midichannel 1;                              // MIDI channel from 0 to 15 (+1 in "real world")
boolean VelocityFlag  = true;                           // Velocity ON (true) or OFF (false)

//*******************************************************************************************************************
// Internal Use Variables
//*******************************************************************************************************************
boolean activePad[6] = {0,0,0,0,0,0};                   // Array of flags of pad currently playing
int PinPlayTime[6] = {0,0,0,0,0,0};                     // Counter since pad started to play
byte status1;

int pin = 0;     
int hitavg = 0;
//*******************************************************************************************************************
// Setup
//*******************************************************************************************************************
void setup() 
{
  Serial.begin(115200);                                  // connect to the serial port 115200
}
//*******************************************************************************************************************
// Main Program
//*******************************************************************************************************************
void loop() 
{
  for(int pin=0; pin < 6; pin++) {
    hitavg = analogRead(pinAssignments[pin]);  
    // read the input pin
    if((hitavg > PadCutOff[pin])) {
      Serial.println(hitavg);
      if((activePad[pin] == false)) {
        if(VelocityFlag == true) {
          //hitavg = 127 / ((1023 - PadCutOff[pin]) / (hitavg - PadCutOff[pin]));    // With full range (Too sensitive ?)
          hitavg = (hitavg / 8) - 1;                                                 // Upper range
        } else {
          hitavg = 127;
        }
        MIDI_TX(0x90, PadNote[pin], hitavg); //note on

        PinPlayTime[pin] = 0;
        activePad[pin] = true;
      } else {
        PinPlayTime[pin] = PinPlayTime[pin] + 1;
      }
    } else if((activePad[pin] == true)) {
      PinPlayTime[pin] = PinPlayTime[pin] + 1;
      if(PinPlayTime[pin] > MaxPlayTime[pin]) {
        activePad[pin] = false;
        MIDI_TX(0x90, PadNote[pin], 0); 
      }
    }
  }
}

//*******************************************************************************************************************
// Transmit MIDI Message
//*******************************************************************************************************************
void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY) 
{
  status1 = MESSAGE + midichannel;
  Serial.print("Vai enviar (byte1, byte2, byte3): ");
 Serial.print(status1);
Serial.print(", ");
Serial.print(PITCH);
Serial.print(", ");
Serial.println(VELOCITY);
  Serial.write(status1);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}

