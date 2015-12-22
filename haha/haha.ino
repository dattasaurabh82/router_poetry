#include <Process.h>
#include <FileIO.h>

const int ledPin =  13;

int linkDataTranBtn = 4;
int linkExtBtn = 5;
int poemGenBtn = 6;
int broadCastBtn = 7;

boolean linkExtState, linkDataTranState, poemGenState, broadCastState = LOW;
boolean prevLinkExtState, prevLinkDataTranState, prevPoemGenState, prevBroadCastState = LOW;


int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 100;

void setup() {
  pinMode(linkDataTranBtn, INPUT);
  pinMode(linkExtBtn, INPUT);
  pinMode(poemGenBtn, INPUT);
  pinMode(broadCastBtn, INPUT);

  pinMode(ledPin, OUTPUT);

  Bridge.begin();
  Serial.begin(9600);

  // Wait until a Serial Monitor is connected.
  while (!Serial);
  Serial.println("Serial has been established \n \n");


}

void loop() {
  linkDataTranState = digitalRead(linkDataTranBtn);
  linkExtState = digitalRead(linkExtBtn);
  poemGenState = digitalRead(poemGenBtn);
  broadCastState = digitalRead(broadCastBtn);


  if (linkDataTranState != prevLinkDataTranState) {
    if (linkDataTranState == HIGH) {
      Serial.println("Link data transfer Btn pressed");
    }
    prevLinkDataTranState = linkDataTranState;
  }


  if (linkExtState != prevLinkExtState) {
    if (linkExtState == HIGH) {
      blinker(interval);
      linkExtractor();
    }
    prevLinkExtState = linkExtState;
  }


  if (poemGenState != prevPoemGenState) {
    if (poemGenState == HIGH) {
      blinker(interval);
      poemCreator();
    }
    prevPoemGenState = poemGenState;
  }


  if (broadCastState != prevBroadCastState) {
    if (broadCastState == HIGH) {
      blinker(interval);
      poemReciter();
    }
    prevBroadCastState = broadCastState;
  }


}



void linkExtractor() {
  
  Serial.println("Extracting Links from the captured data from router");
  Serial.println("As my processor is slow this might take some time");

  Process extractorCmd;
  extractorCmd.runShellCommand("cd /mnt/sda1/arduino/Poet && python link_extractor.py");
  while (extractorCmd.running());
  Serial.println("All the links extracted");
}


void poemCreator() {
  
  Serial.println("Getting the texts of the links by visiting them.. So hold on. It might take some time \n");

  Process p;
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && python link_data_extractor.py");
  while (p.running()); // Wait for the thing to finish
  // Removing white spaces and tabs by running a terminal command
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && sed -e 's/[\t]//g;/^$/d' combined_text.txt > combined.txt");
  while (p.running());  // Wait for the thing to finish
  //Combining into one line
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && echo $(cat combined.txt) > corpus.txt");
  while (p.running());  // Wait for the thing to finish
  //removing the "now un - necessary" files
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && rm combined.txt && rm combined_text.txt && rm links.txt");
  while (p.running());  // Wait for the thing to finish
  Serial.println("Corpus is ready now!!");
  delay(2000);

  Serial.println("Firing Markov generator from corpus");

  //Generating random arguments (order and length) for the markov script
  long order = random(2, 30);
  long poemLength = random(400, 2000);
  //Adding them as arguments to my mrkov.py which needs these arguments to generate psudo random texts
  String markovCmd = "cd /mnt/sda1/arduino/Poet && python markov1.py ";
  markovCmd.concat(order);
  markovCmd.concat(" ");
  markovCmd.concat(poemLength);
  Serial.println(markovCmd);

  p.runShellCommand(markovCmd);
  while (p.running());  // Wait for the thing to finish

  //removing corpus text
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && rm corpus.txt");
  while (p.running());
  delay(2000);

  Serial.println("Poem created now");
}

void poemReciter() {
  
  Process recitor;
  recitor.runShellCommand("cd /mnt/sda1/arduino/Poet && espeak -a 20 \"$(cat poem.txt)\"");
  while (recitor.running());
  recitor.runShellCommand("cd /mnt/sda1/arduino/Poet && rm poem.txt");
  while (recitor.running());
  Serial.println("Done!!");
}

void blinker(const long intval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= intval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
}

