#include <DIO2.h>

// This is now the DUE version 

#include <Encoder.h>

Encoder myEnc(2,3); // pick your pins, reverse for sign flip

bool pulsing = false;
byte freq = 30; // Pulse at 30 Hz (default)
unsigned int ontime = 20000; // On for 10 ms
unsigned long cycletime = 1000000 / freq;
// unsigned int sweep_start;
unsigned long int pulsetime = 0;
bool onoff = false;
unsigned long int tnow;
unsigned int step_size = 100; // in micros
long pos;
byte m, n;



void setup() {
  Serial.begin(9600);
  //SerialUSB.begin(115200); // for real-time feedback
  myEnc.write(0);
  pos = myEnc.read();
  
  pinMode2(12, OUTPUT);
  pinMode2(13, OUTPUT);
  digitalWrite2(12, LOW);
  digitalWrite2(13, LOW);
  
  /*
  Serial.print("Frequency (Hz): ");
  Serial.println(freq);
  Serial.print("Cycle time (us): ");
  Serial.println(cycletime);
  Serial.print("On time (us): ");
  Serial.println(ontime);
  Serial.print("Step size (us): ");
  Serial.println(step_size);
  */
  
  
}

void loop() {
//  sweep_start = micros();
  
  if (Serial.available() >= 2){
    // Read 2 bytes
    m  = Serial.read();
    n = Serial.read();

    /*
    if (m == 5){
      // Give position
      Serial.write((byte *) &pos, 4);
    }
    */
    if (m == 2){
      // Set frequency
      cycletime = 1000000 / n;
    }
    else if (m == 1){
      // Start pulsing
      pulsing = true;
      pulsetime = micros();

      myEnc.write(0);    // zero the position
      pos = 0;
    }
    else if (m == 0){
      // End pulsing
      pulsing = false;
    }
  }

  tnow = micros();

  pos = myEnc.read();
  
  if (pulsing){
    if ((tnow - pulsetime) >= cycletime){
      if (~onoff){
        pulsetime = micros();
        Serial.write((byte *) &pos, 4);
        digitalWrite2(12, HIGH);
        onoff = true;
      }
    }
    else if ((tnow - pulsetime) >= ontime){
      if (onoff){
        digitalWrite2(12, LOW);
        onoff = false;
      }
    }
  }
  else {
    if (onoff){
      digitalWrite2(12, LOW);
      onoff = false;
    }
  }

  delayMicroseconds(step_size);
//  Serial.println(micros() - sweep_start);
}
  
