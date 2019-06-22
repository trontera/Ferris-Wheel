#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 48;

// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 11, 10, 9, 8);

//Pins of motor buttons
const int clockwisePin = 2;
const int antiClockwisePin = 3;

//Reads state of motor buttons
int clockwiseState = 0;
int previousClockwiseState = 0; //Previous reads the state before the current one
int antiClockwiseState = 0;
int previousAntiClockwiseState = 0;

//stores value of speed of motor
int motorSpeed = 0;

//Checks if the motor is running
int onClockwise = 0;
int onAntiClockwise = 0;

//Pin for the LEDs
int LEDS = 12;
//For the melody playing in piezo
int speakerPin = 6; //pin of piezo
int length = 26; //number of notes
int tempo = 20; //tempo of music
char notes[] = "gagfefgdefefggagfefgdgec"; //the letter representation of notes
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 3}; //beats for each note
int i = 0; //used for looping through the music

//Function to play the tone while piezo is on
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

//Function to play the tone according to the note
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' }; //Note names
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };//tones for the notes in C major

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
//Plays the melody of London Bridges when piezo is on
void playMelody() {
  //Go through each note in the array
  if (i < length) {
    //call playNote to play the notes according to beat and tempo
    playNote(notes[i], tempo * beats[i]);
    i++;   //increment
  }
  //when i goes beyond length of the notes, start again
  else {
    i = 0;
  }

}
//Function to set the speed of the motor acording to potentiometer value
void getSpeed()
{
  //Low 300s is max speed, so while it's value is less than 300, the speed is the value
  if (motorSpeed < 300) {
    myStepper.setSpeed(motorSpeed);
  }
  //If it goes beyond, then set it to a constant of 310rpm
  else {
    motorSpeed = 310;
    myStepper.setSpeed(motorSpeed);
  }
}
void setup() {
  //Set pins as input or output pins
  pinMode(clockwisePin, INPUT_PULLUP);
  pinMode(antiClockwisePin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(LEDS, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //Read states of motor buttons
  clockwiseState = digitalRead(clockwisePin);
  antiClockwiseState = digitalRead(antiClockwisePin);

  //if the previous state is not the current state
  if (previousClockwiseState != clockwiseState) {
    //then check if button is not pressed
    if (clockwiseState == HIGH) {
      //if it's true then set this value to opposite value
      onClockwise = !onClockwise;
      //and turn leds off
      digitalWrite(LEDS, LOW);
    }
  }
  //Do the same for the anti-clockwise button
  if (previousAntiClockwiseState != antiClockwiseState) {
    if (antiClockwiseState == HIGH) {
      onAntiClockwise = !onAntiClockwise;
      digitalWrite(LEDS, LOW);
    }
  }

  //if onClockWise is original value
  if (onClockwise) {
    //Read value of potentiometer as the speed of motor
    motorSpeed = analogRead(A0); 
    //Call function getSpeed
    getSpeed();

    //Call function to play the song
    playMelody();
    //turn leds on
    digitalWrite(LEDS, HIGH);
    
    //Step motor moves clockwise
    myStepper.step(stepsPerRevolution);

  }
  //Same for the antiClockwise value
  if (onAntiClockwise) {
    motorSpeed = analogRead(A0);
    getSpeed();
    Serial.println(motorSpeed);
    playMelody();
    digitalWrite(LEDS, HIGH);
    //Negative represents the reverse direction
    myStepper.step(-stepsPerRevolution);
  }
  //Update the state of motor buttons before loop repeats 
  previousClockwiseState = clockwiseState;
  previousAntiClockwiseState = antiClockwiseState;
}
